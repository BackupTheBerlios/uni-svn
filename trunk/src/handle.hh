#ifndef _UNI_HANDLE_HH_
#define _UNI_HANDLE_HH_

#include <func.hh>
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

    ~Handle () { if (_dtor) _dtor (_handle); }

  protected:

    Handle () : Term (Term::T) { }
    Handle (TermPtr type, void* handle, dtor_t dtor = 0) : Term (type), _handle (handle), _dtor (dtor) { }
  };

  struct ext_t
  {
    int     special : 1;
    int     chained : 1;
    TermPtr term;

    ext_t () { }
    ext_t (int special, TermPtr term) : special (special), term (term) { }
    ext_t (int a, int s, void* e, TermPtr t) : special (0), term (SimpleFunc::create (a, s, e, t)) { }
  };

  typedef std::map<std::string,ext_t> ext_map_t;

  typedef ext_map_t* (*ext_ctor_t)();
  typedef ext_map_t* (*ext_dtor_t)(ext_map_t*);
};

#endif
