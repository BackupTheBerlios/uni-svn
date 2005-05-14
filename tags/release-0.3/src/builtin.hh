#ifndef _UNI_BUILTIN_HH_
#define _UNI_BUILTIN_HH_

#include <basicdef.hh>
#include <type.hh>

namespace NAMESPACE
{
  class Context;

  class Builtin
  {
  public:

    void init (Context* context);
    void uninit (Context* context);
  };
};

#endif
