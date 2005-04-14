#include <context.hh>
#include <family.hh>
#include <exception.hh>
#include <proj.hh>
#include <temp.hh>

namespace NAMESPACE
{
  IMP_TYPE (Family, "FAM");

  void
  Family::BindState::add (TermPtr body, TermPtr type)
  {
    assert (type);

    _leaf = false;
    TermPtr curr = type, next;

    if (ProjPtr p = CAST<Proj>(type)) {
      curr = p->from();
      next = p->to();
    }

    if (next) {
      for (iterator i = begin(); i != end(); ++i)
	if ((**i)._edge->equ (curr))
	  return (**i).add (body, next);
    }

    BindState* f = new BindState;
    f->_edge = curr;

    if (next)
      f->add (body, next);
    else {
      f->_body = body;
      f->_leaf = true;
    }

    if (DEP_T == curr || UPR_T == curr)
      push_front (f);
    else {
      for (iterator i = begin(); i != end(); ++i) {
	if (DEP_T != (*i)->_edge && UPR_T != (*i)->_edge) {
	  if (! (*i)->_edge->compat (curr)) {
	    insert (i, f);
	    return;
	  }
	}
      }

      push_back (f);
    }
  }

  Family::BindState*
  Family::BindState::move (Context* c, int flags, unsigned int argi)
  {
    if (argi >= c->arg_count())
      return this;

    for (iterator i = begin(); i != end(); ++i)
      if (! (**i)._leaf)
	if (c->arg_compat (argi, ALL_PURE, (**i)._edge))
	  if ((**i).size())
	    if (BindState* state = (**i).move (c, flags, argi + 1))
	      return state;

    return this;
  }

  TermPtr
  Family::BindState::move (TermPtr type)
  {
    TermPtr result;

    for (const_iterator i = begin(); i != end(); ++i) {
      if (DEP_T == (*i)->_edge || UPR_T == (*i)->_edge)
	result = (*i)->_body;
      else if (result) {
	if ((*i)->_edge->equ (type))
	  return (**i)._body;
      }
      else if ((*i)->_edge->compat (type))
	return (**i)._body;
    }

    return result;
  }

  //////////////////////////////////////////////////////////////////////////////
  void
  Family::add (TermPtr body, TermPtr type)
  {
    if (shared_ptr<Temp> f = CAST<Temp> (body))
      push_back (f);
    else {
      try {
	_node.add (body, type);
      }
      catch (TermPtr et) {
	if (ExcPtr e = CAST<Exc> (et)) {
	  if (E_DUP_NAME == e->id()) {
	    e->term1 (SELF);
	    throw e;
	  }
	}

	throw;
      }
    }

    if (1 == _node.size())
      _type = type;
    else
      _type = Family::T;
  }

  TermPtr
  Family::reduce (Context* c, int flags, TermPtr expected)
  {
    if (! (BIND & flags))
      throw E_MODE;

    // First, matching term by going through
    // the family automaton.

    if(BindState* state = _node.move (c, flags, 0))
      if (TermPtr result = state->move (expected ? expected : Term::T))
	return result;

    // If there are no matched, then search in the template space, try to
    // instanticate each template against types of arguements on the stack.
    // In case no family node nor template can be found with right type, if we
    // are performing meta reduction, then reduction should stop. Otherwise,
    // a no-match exception will be raised.

    if (0 == c->arg_count())
      throw E_MODE;

    if (! empty())
      for (iterator i = begin(); i != end(); ++i)
	if (TermPtr result = (*i)->reduce (c, flags, expected))
	  return result;

    if (ALL != flags)
      throw E_MODE;
    else {
      TermPtr given = Term::T;
      for (int i = ((int)c->arg_count()) - 1; i >= 0; --i)
	given = Proj::create (c->arg(i)->type(), given);
      throw E (E_NO_MATCH, SELF, given);
    }
  }
};
