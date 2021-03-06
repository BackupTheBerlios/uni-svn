#ifndef _UNI_HANDLE_HH_
#define _UNI_HANDLE_HH_

#include <func.hh>
#include <term.hh>

namespace NAMESPACE
{
  class Handle : public Term
  {
    DEF_DYNAMIC  (Handle);
    VAL_PROPERTY (void*, handle);

  protected:

    Handle () { assert (false); }
    Handle (TermPtr type, void* handle) : Term (type), _handle (handle) { }
  };
};

#endif
