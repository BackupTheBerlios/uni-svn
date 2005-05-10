#include <string.hh>
#include <type.hh>

namespace NAMESPACE
{
  IMP_TYPE (Str,  "STR");

  int
  Str::to_c (int style, int id) const
  {
    return (int) _str;
  }

  void
  Str::from_c (int style, int val)
  {
    assert (false);
  }

  bool
  Str::xequ (TermPtr rhs) const
  {
    if (StrPtr s = CAST<Str>(rhs))
      return 0 == strcmp (_str, s->_str);
    else
      return false;
  }
};
