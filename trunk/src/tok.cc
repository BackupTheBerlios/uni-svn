#include <app.hh>
#include <context.hh>
#include <int.hh>
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
   */
  TermPtr
  Tok::reduce (Context* c, int flags, TermPtr expected)
  {
    if (TermPtr r = c->scopes()->get_symbol(str()))
      return r;
    else if (BIND & flags)
      return Sym::create (str());
    else
      return TermPtr();
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
