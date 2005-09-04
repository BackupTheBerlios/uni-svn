#ifndef _UNI_HANDLE_HH_
#define _UNI_HANDLE_HH_

#include <term.hh>

namespace NAMESPACE
{
  class Handle : public Term
  {
    typedef void (*dtor_t)(void*);

    DEF_DYNAMIC  (Handle);
    VAL_PROPERTY (void*, handle);
    VAL_PROPERTY (dtor_t, dtor);

  public:

    ~Handle () { _dtor (_handle); }

  protected:

    Handle () : Term (Term::T) { }
    Handle (TermPtr type, void* handle, dtor_t dtor) : Term (type), _handle (handle), _dtor (dtor) { }
  };
};

#endif
