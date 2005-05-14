#ifndef _UNI_SYM_HH_
#define _UNI_SYM_HH_

#include <string.hh>

namespace NAMESPACE
{
  class Sym : public MStr
  {
    DEF_TERM      (Sym);
    DEF_VISITABLE (Sym, sym);

  protected:

    virtual bool xequ (TermPtr rhs) const;

  private:

    Sym (const char* name) : MStr(name) { }
  };
};

#endif
