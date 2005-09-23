#ifndef _UNI_HH_
#define _UNI_HH_

#include <abs.hh>
#include <app.hh>
#include <bool.hh>
#include <cons.hh>
#include <machine.hh>
#include <exception.hh>
#include <func.hh>
#include <int.hh>
#include <proj.hh>
#include <context.hh>
#include <scanner.hh>
#include <seq.hh>
#include <string.hh>
#include <style.hh>
#include <sym.hh>
#include <temp.hh>
#include <tok.hh>
#include <type.hh>
#include <opt.hh>

namespace NAMESPACE
{
  struct ext_t
  {
    char*   name;
    TermPtr term;
  };

  typedef ext_t* (*ext_ctor_t)();
}

#define _F(arity, style, entry, type, ...) \
TermPtr (new Envf (arity, style, (void*) entry, type, Envf::MACH_PTR, ##__VA_ARGS__))

#define _S(a, r, e, t, ...) \
TermPtr (new Envf (a, r, (void*) e, t, 0, ##__VA_ARGS__))

#define _A(a, r, e, t) \
_S (a, r, e, t, ALL, ALL, ALL, ALL, ALL, ALL, ALL, ALL)

#endif
