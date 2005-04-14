#include <app.hh>
#include <context.hh>
#include <raw.hh>
#include <scope.hh>
#include <seq.hh>
#include <style.hh>
#include <sym.hh>
#include <tok.hh>
#include <type.hh>

#include <algorithm>
#include <stack>

using std::make_pair;
using std::max_element;
using std::stack;

namespace NAMESPACE
{
  IMP_TYPE (Raw, "RAW");

  bool operator < (const TOK& lhs, const TOK& rhs)
  {
    return GET_PRC(lhs.second) < GET_PRC(rhs.second);
  };

  void
  Raw::visit_children (Visitor& visitor)
  {
    for (iterator i = begin(); i != end(); ++i)
      i->first->visit (visitor);
  }

  TermPtr
  Raw::xsub (TermPtr var, TermPtr sub) const
  {
    for (const_iterator i = begin(); i != end(); ++i) {
      TermPtr e = i->first->sub (var, sub);
      if (e != i->first) {
	RawPtr raw = Raw::create();
	raw->insert (raw->end(), begin(), i);
	raw->push_back (make_pair(e, i->second));
	for (++i; i != end(); ++i)
	  raw->push_back (make_pair(i->first->sub (var, sub), i->second));
	return raw;
      }
    }

    return SELF;
  }

  bool
  Raw::xfreeof (TermPtr term) const
  {
    for (const_iterator i = begin(); i != end(); ++i)
      if (i->first->freeof (term))
	return false;

    return true;
  }

  TermPtr
  Raw::reduce (Context* c, int flags, TermPtr expected)
  {
    if (empty())
      return VOID;
    else {
      Raw raw (*this);

      RawPtr  next = raw.deoutfix (c);
      TermPtr curr = raw.construct (c, flags);

      if (next) {
 	SeqPtr seq = Seq::create();
 	seq->append (curr);
	if (! next->empty())
	  seq->append (next);
 	return seq;
      }
      else
	return curr;
    }
  }

  void
  Raw::_update_style (Context* c)
  {
    for (iterator iter = begin(); iter != end(); ++iter) {
      if (TokPtr tok = CAST<Tok> (iter->first)) {
 	if (DUMMY & (iter->second = c->scopes()->get_style (tok->str()))) {
	  iterator i = iter;
	  --iter;
	  erase (i);
	}
      }
      else
	iter->second = MAKE_STYLE (PREFIX, PREC_NOR);
    }
  }

  bool
  Raw::_rinfix (iterator& max, iterator& prev, iterator& next)
  {
    if (end() != prev && end() != next) {
      if (TokPtr t = CAST<Tok>(max->first)) {
	iterator opt = max;
	for (++opt, ++opt; opt != end(); ++opt, ++opt) {
	  if (TokPtr s = CAST<Tok>(opt->first)) {
	    if (s->equ(t))
	      max = opt;
	    else
	      break;
	  }
	}

	prev = max; --prev;
	next = max; ++next;

	return end() != prev && end() != next;
      }
    }

    return false;
  }

  TermPtr
  Raw::construct (Context* c, int flags)
  {
    _update_style (c);

    while (size() > 1) {
      iterator max  = max_element (begin(), end());
      iterator prev = max; --prev;
      iterator next = max; ++next;
      iterator remove_begin = end(); // from where tokens should be removed.
      iterator insert_point = end(); // the point where the app to be inserted;
      iterator rand1, rand2 = end(); // the two rands of the application.
      unsigned int fix = GET_FIX (max->second);

      if ((LINFIX & fix) && end() != prev && end() != next) {
	remove_begin = prev;
	insert_point = next;
	rand1 = prev;
	rand2 = next;
      }
      else if ((RINFIX & fix) && _rinfix (max, prev, next)) {
	remove_begin = prev;
	insert_point = next;
	rand1 = prev;
	rand2 = next;
      }
      else if ((POSTFIX & fix) && end() != prev) {
	remove_begin = prev;
	insert_point = max;
	rand1 = prev;
      }
      else if (PREFIX & fix) {
	if ((end() != next)
	    && ((MAKE_STYLE (PREFIX, PREC_NOR) == next->second)
		|| (MAKE_STYLE (PREFIX, PREC_MIN) == next->second))) {
	  remove_begin = max;
	  insert_point = next;
	  rand1 = next;
	}
	else {
	  max->second = MAKE_STYLE (PREFIX, PREC_MIN);
	  continue;
	}
      }
      else
	assert (false);

      if (TokPtr tok = CAST<Tok> (max->first))
	if (TermPtr term = tok->reduce (c, CONS, TermPtr()))
	  max->first = term;

      TermPtr new_term = App::create (max->first, rand1->first);
      if (end() != rand2)
	new_term = App::create (new_term, rand2->first);

      erase (remove_begin, insert_point);
      insert_point->first = new_term;
      insert_point->second = MAKE_STYLE (PREFIX, PREC_NOR);
    }

    return front().first;
  }

  int
  Raw::_detach (Context* c, iterator li, iterator ri)
  {
    TokPtr ltok = CAST<Tok>(li->first);
    TokPtr rtok = CAST<Tok>(ri->first);

    assert (ltok);
    assert (rtok);

    iterator start = li;
    string   name  = string (ltok->str()) + " " + string (rtok->str());

    if (! c->scopes()->has_outfix (ltok->str(), rtok->str()))
      throw E (E_OUTFIX_MISMATCH, ltok, rtok);

    RawPtr raw = Raw::create();
    raw->splice (raw->end(), *this, start, ri);
    erase (li);

    int fix = GET_FIX (c->scopes()->get_style (name));
    if (GROUP & fix)
      ri->first = raw;
    else
      ri->first = App::create (Tok::create (name.c_str()), raw);
    ri->second = MAKE_STYLE (PREFIX, PREC_NOR);
    return fix;
  }

  RawPtr
  Raw::deoutfix (Context* c)
  {
    iterator        iter;
    unsigned int    fix;
    stack<iterator> s;

    for (iter = begin(); end() != iter; ++iter) {
      if (TokPtr tok = CAST<Tok> (iter->first)) {
	fix = GET_FIX (c->scopes()->get_style (tok->str()));

	if (EOS & fix && s.empty())              // if it's EOS and we are not inside outfix...
	  break;                                 // then done with searching for pairs.
	else if (LOUTFIX & fix)                  // if it's a left operator...
	  s.push (iter);                         // push onto the stack.
	else if (ROUTFIX & fix) {                // if it's a right operator...
	  if (s.empty())                         // if no left opt. yet...
	    throw E (E_OUTFIX_R, tok);           // error, lonely right opt.
	  else {                                 // if there is at least a left opt. ...
	    iterator left = s.top(); s.pop();    // pop the left opt.
	    if (EOS & (_detach (c, left, iter))) // detach the pair, and if pair is also an EOS...
	      break;                             // then done with searching for pairs.
	  }
	}
      }
    }

    if (! s.empty())
      throw E (E_OUTFIX_L, s.top()->first);
    else if (end() != iter) {
      // if the iterator is not at the end of the list,
      // then an end-of-statement separator was encountered.
      // a new raw-list object should be created, where all
      // the terms behind the separator will be moved into,
      // and the new raw-list object will be returned.

      iterator start = iter; ++start; // the beginning of the next statement

      if (EOS == fix)                 // if it is only a EOS operator...
	erase (iter);                 // then deleted it.

      RawPtr raw = Raw::create();
      raw->splice (raw->end(), *this, start, end());
      return raw;
    }

    return RawPtr();
  }
};
