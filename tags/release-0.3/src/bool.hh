#ifndef _UNI_BOOL_HH_
#define _UNI_BOOL_HH_

#include <term.hh>

namespace NAMESPACE
{
  class Bool : public Term
  {
    DEF_TERM      (Bool);
    DEF_VISITABLE (Bool, bool);
    VAL_PROPERTY  (bool, val);

  public:

    static const TermPtr TRUE;
    static const TermPtr FALSE;

  private:

    Bool (bool f) : _val (f) { }
  };
};

#endif
