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

  struct ext_t
  {
    TermPtr term;

    ext_t () { }
    ext_t (TermPtr term) : term (term) { }
    ext_t (int a, int s, void* e, TermPtr t) : term (SimpleFunc::create (a, s, e, t)) { }
  };

  typedef std::map<std::string,ext_t> ext_map_t;

  typedef ext_map_t* (*ext_ctor_t)();
  typedef ext_map_t* (*ext_dtor_t)(ext_map_t*);
};

#endif
