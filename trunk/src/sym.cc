#include <sym.hh>
#include <tok.hh>
#include <type.hh>

namespace NAMESPACE
{
  IMP_TYPE (Sym, "SYM");

  bool
  Sym::xequ (TermPtr rhs) const
  {
    if (CAST<Sym>(rhs))
      return Str::xequ (rhs);
    else if (CAST<Tok>(rhs))
      return Str::xequ (rhs);
    else
      return false;
  }
};
