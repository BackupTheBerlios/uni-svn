#include <bool.hh>
#include <type.hh>

namespace NAMESPACE
{
  IMP_TYPE (Bool, "BOOL");

  const TermPtr Bool::TRUE  = Bool::create (true);
  const TermPtr Bool::FALSE = Bool::create (false);
};
