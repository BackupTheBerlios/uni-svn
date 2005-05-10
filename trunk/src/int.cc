#include <int.hh>
#include <type.hh>

namespace NAMESPACE
{
  IMP_TYPE (Int, "INT");

  int
  Int::to_c (int style, int id) const
  {
    return _val;
  }

  void
  Int::from_c (int style, int val)
  {
    _val = val;
  }

  bool
  Int::xequ (TermPtr rhs) const
  {
    if (IntPtr i = CAST<Int>(rhs))
      return _val == i->val();
    else
      return false;
  }
};
