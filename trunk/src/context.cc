#include <app.hh>
#include <context.hh>
#include <exception.hh>
#include <family.hh>
#include <frame.hh>
#include <handler.hh>
#include <raw.hh>
#include <scope.hh>
#include <style.hh>
#include <sym.hh>
#include <term.hh>

namespace NAMESPACE
{
  class Shield : public vector<Frame*>
  {
  public:

    TermPtr exit;

    Shield (TermPtr exit_term) : exit (exit_term) { }
  };

  //// ctor ////////////////////////////////////////////////////////////////////
  Context::Context ()
    : _scopes (new ScopeStack),
      _step_break (false)
  {
    _new_shield();   // create the outmost shield.
    _scopes->push(); // create root scope.

    //===============================================
    _scopes->add_style  (";",   MAKE_STYLE (EOS, PREC_SEP));
    _scopes->add_style  ("( )", MAKE_STYLE (GROUP, PREC_SEP));
    _scopes->add_outfix ("(", ")");
    //===============================================
  }

  Context::~Context()
  {
    assert (_scopes);
    delete _scopes;
  }

  TermPtr
  Context::special_sym (const string& id) const
  {
    try {
      return _scopes->get_symbol (_scopes->get_special (id));
    }
    catch (...) {
      return TermPtr();
    }
  }

  void
  Context::set_attr (const string& id, int val)
  {
    if (id == "step_break")
      step_break (val != 0);
  }

  //// arguments ///////////////////////////////////////////////////////////////
  TermPtr
  Context::arg (unsigned int i)
  {
    assert (HAS_FRAME);
    assert (TOP_FRAME->size() > i);

    AppPtr app = TOP_FRAME->get_root(i);
    assert (app);
    return app->rand();
  }

  AppPtr
  Context::arg_root (unsigned int i)
  {
    assert (HAS_FRAME);
    assert (TOP_FRAME->size() > i);

    AppPtr app = TOP_FRAME->get_root (i);
    assert (app);
    return app;
  }

  TermPtr
  Context::arg_type (unsigned int i)
  {
    assert (HAS_FRAME);
    assert (TOP_FRAME->size() > i);

    return arg(i)->type();
  }

  /**
   * \note   updating the root node after reducing is nessesary
   *         since polymorphism by return type for internal functions
   *         all rely on it.
   */
  TermPtr
  Context::arg_reduce (unsigned int i, int flags, TermPtr expected, bool collect)
  {
    assert (HAS_FRAME);
    assert (TOP_FRAME->size() > i);

    AppPtr app = TOP_FRAME->get_root(i);
    assert (app);
    TermPtr result = reduce (app->rand(), flags, expected, collect);
    if (result)
      app->rand (result);
    return result;
  }

  /**
   * \note   using arg_reduce to reduce the argument is nessesary
   *         since it will update the root, and polymorphism by
   *         return type rely on it.
   *
   * \note   however, sometimes, the root should not be updated.
   *         for instance, if in a family, there is a function
   *         expecting a raw term, then it will not be happy to
   *         see the argument being bound or even reduced just
   *         for trying to match with other family members.
   */
  bool
  Context::arg_compat (unsigned int i, int flags, TermPtr type)
  {
    TermPtr r = arg (i);

    for ( ; ; ) {
      TermPtr got = r->type();
      int f;

      if (Raw::T == type)                         // if expecting a raw term...
	break;                                    // then everything will match.
      else if (got != Raw::T && Term::T == type)  // if expecting a any and not getting raw...
	break;                                    // then it is ok.
      else if (Raw::T == got && (BIND & flags))   // if we got a raw term...
	f = BIND;                                 // then we need to bind it
      else if (DEP_T == got)                      // if we got a dependent term...
	f = ALL_CTXT;                             // then reduced it with context-side-effect
      else if (UPR_T == got)                      // if we got a unpredictable term...
	f = ALL;                                  // then we need to fully reduce it
      else {                                      // otherwise...
	if (got->compat (type))                   // see if the two types are compatible.
	  break;                                  // compatible... then match
	return false;                             // not compatible... then no match
      }

      /** \todo   what about the [flags]? */

      if (! (r = reduce (r, f, type, false)))     // reduce the argument
	return false;                             // cannot match if it fail to be reduced
    }

    TOP_FRAME->get_root(i)->rand (r);             // if it was a match, update the root.
    return true;
  }

  unsigned int
  Context::arg_count () const
  {
    if (HAS_FRAME)
      return TOP_FRAME->size();
    else
      return 0;
  }

  unsigned int
  Context::frame_depth () const
  {
    return TOP_SHIELD->size();
  }

  void
  Context::push (AppPtr app)
  {
    assert (HAS_FRAME);
    TOP_FRAME->push (app);
  }

  int
  Context::current_flags () const
  {
    assert (HAS_FRAME);
    return TOP_FRAME->flags();
  }

  Frame*
  Context::current_frame () const
  {
    assert (HAS_FRAME);
    return TOP_FRAME;
  }

  void
  Context::pop (unsigned int n)
  {
    assert (HAS_FRAME);
    TOP_FRAME->pop (n);
  }

  TermPtr
  Context::pop ()
  {
    assert (HAS_FRAME);
    assert (TOP_FRAME->size());

    AppPtr app = TOP_FRAME->pop();
    assert (app);
    return app->rand();
  }

  TermPtr
  Context::reduce (TermPtr term, int flags, TermPtr expect, bool collect)
  {
    bool reduced = false;

    assert (term);

    _new_frame (flags);

    if (_step_break && _debugger)
      _debugger->step_break (this, term);

    try {
      for ( ; ; ) {
	for ( ; ; ) {
	  term = _expand_app (term);

	  if (_step_break && _debugger)
	    _debugger->step_break (this, term);

	  if (TermPtr tmp = term->reduce (this, flags, expect)) {
	    term = tmp;
	    reduced = true;

	    if (_step_break && _debugger)
	      _debugger->step_break (this, term);
	  }
	  else
	    break;
	}

	if (0 == TOP_FRAME->size())                       // if nothing left on stack...
	  break;                                          // then we are done.
	else if (TermPtr f = _scopes->get_symbol (" ")) { // if the space opt is defined...
	  term = App::create (f, term);                   // create app with space as head
	  while (TOP_FRAME->size())                       // and append all args lefted
	    term = App::create (term, pop());             // then continue to reduce it
	}
	else                                              // if no space opt is defined...
	  throw E (E_HEAD, term);                         // then this is bad expression.
      }
    }
    catch (exc_t e) {
      if (E_MODE == e) {
	if (collect || reduced) {
	  while (TOP_FRAME->size()) {
	    term = App::create (term, pop());
	    CAST<App>(term)->update_type();
	  }

	  if (_step_break && _debugger)
	    _debugger->step_break (this, term);
	}
	else
	  term.reset();
      }
      else
	throw;
    }

    _del_frame ();

    return term;
  }

  TermPtr
  Context::reduce_in_shield (TermPtr term, int flags, TermPtr exit)
  {
    assert (term);

    _new_shield (exit);

    try {
      TermPtr result = reduce (term, flags);
      _del_shield (flags);
      return result;
    }
    catch (Term& e) {
      TermPtr hdl = special_sym ("exc_handler"); // get handler symbol
      if (! hdl) {                                // if the symbol doesn't exist...
	_del_shield (flags);                      // we are done with this exception shield, delete it
	throw;                                    // forward the exception outward
      }
      TermPtr exc (e.clone());                    // clone it, since it has to be an TermPtr
      TermPtr app = App::create (hdl, exc);       // create application
      reduce (app, flags);                        // execute the handler
      _del_shield (flags);                        // handling is done, delete this exception shield
      return VOID;                                // return value is void
    }
  }

  TermPtr
  Context::eval (RawPtr raw, int flags)
  {
    if (raw->empty())
      return VOID;

    for (RawPtr next; next = raw->deoutfix (this); raw = next) {
      raw->construct (this, CONS);
      reduce_in_shield (raw, flags);
    }

    raw->construct (this, CONS);
    return reduce_in_shield (raw, flags);
  }

  void
  Context::import (const string& lib)
  {
    _importer->import (this, lib);
  }

  void
  Context::_new_frame (int flags)
  {
    assert (HAS_SHIELD);
    TOP_SHIELD->push_back (new Frame (flags));
    assert (HAS_FRAME);
  }

  void
  Context::_del_frame ()
  {
    assert (HAS_SHIELD && HAS_FRAME);
    delete TOP_SHIELD->back();
    TOP_SHIELD->pop_back();
  }

  void
  Context::_new_shield (TermPtr exit)
  {
    push_back (new Shield (exit));
    assert (HAS_SHIELD);
  }

  void
  Context::_del_shield (int flags)
  {
    assert (! empty());

    if (back()->exit)
      reduce (back()->exit, flags);

    delete back();
    pop_back();
  }

  TermPtr
  Context::_expand_app (TermPtr term)
  {
    while (AppPtr app = CAST<App> (term)) {
      push (app);
      term = app->rator();
      assert (term);
    }

    return term;
  }

  AppPtr Context::_pop_root ()    { assert (HAS_FRAME); return TOP_FRAME->pop(); }
  AppPtr Context::_top_root ()    { assert (HAS_FRAME); return TOP_FRAME->top_root(); }
  AppPtr Context::_bottom_root () { assert (HAS_FRAME); return TOP_FRAME->bottom_root(); }
};
