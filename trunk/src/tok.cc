#include <machine.hh>
#include <scope.hh>
#include <style.hh>
#include <sym.hh>
#include <tok.hh>

namespace NAMESPACE
{
  /**
   * \note   Tokens MUST be all resolved to families with
   *         or without the BIND flag. Only with this, one
   *         can implement the 'using' functions.
   *         However, on the other hand, resolve too early
   *         will let lambda function get into troubles.
   *
   *         What the hell are you saying then? Is it a problem
   *         or not? Huh!?
   *
   *         Special token is always reduced immediately,
   *         regardless what the current flags are. And this
   *         is very important. If the special symbol does
   *         not exist, an exception will be thrown.
   */
  TermPtr
  Tok::reduce (Machine* c, int flags, TermPtr expected)
  {
    if (_spec) {
	if (TermPtr r = c->special_sym(str()))
	  return r;
	else {
	  // \todo return a application to [name_get] or [special_get]
	  // \todo special symbol should always exist. if not: exception!
	  return TermPtr();
	}
    }
    else {
      if (TermPtr r = c->scopes()->get_symbol(str()))
	return r;
      else if (BIND & flags)
	return Sym::create (str());
      else
	return TermPtr();
    }
  }

  bool
  Tok::xequ (TermPtr rhs) const
  {
    if (CAST<Tok>(rhs))
      return Str::xequ (rhs);
    else if (CAST<Sym>(rhs))
      return Str::xequ (rhs);
    else
      return false;
  }
};
