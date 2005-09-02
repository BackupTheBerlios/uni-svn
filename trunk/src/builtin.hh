#ifndef _UNI_BUILTIN_HH_
#define _UNI_BUILTIN_HH_

#include <basicdef.hh>
#include <type.hh>

namespace NAMESPACE
{
  class Machine;

  class Builtin
  {
  public:

    void init (Machine* machine);
    void uninit (Machine* machine);
  };
};

#endif
