#ifndef _UNI_INT_HH_
#define _UNI_INT_HH_

#include <cval.hh>
#include <term.hh>

#include <stdlib.h>

namespace NAMESPACE
{
  class Int : public Term, public CVal
  {
    DEF_TERMX     (Int);
    DEF_VISITABLE (Int, int);
    VAL_PROPERTY  (int, val);

  public:

    virtual int to_c (int style, int id) const;
    virtual void from_c (int style, int val);

  protected:

    virtual bool xequ (TermPtr rhs) const;

  private:

    Int () : _val(0) { }
    Int (int val) : _val(val) { }
    Int (const std::string& s) : _val(atoi (s.c_str())) { }
  };
};

#endif
