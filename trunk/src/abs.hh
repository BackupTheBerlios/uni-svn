#ifndef _UNI_ABS_HH_
#define _UNI_ABS_HH_

#include <tree.hh>

namespace NAMESPACE
{
  class BoundVar : public Term
  {
    DEF_TERMLETX  (BoundVar, Term);
    DEF_VISITABLE (BoundVar, var);
    VAL_PROPERTY  (unsigned int, id);

  public:

    BoundVar () : _id (_next_id++) { }

  private:

    static unsigned int _next_id;
  };

  class Abs : public STree<2>
  {
    DEF_TERMLET   (Abs,  Term);
    DEF_VISITABLE (Abs,  abs);
    TERM_TREEMAP  (body, 0);
    TERM_TREEMAP  (bv,   1);

  public:

    virtual TermPtr reduce (Machine* machine, int flags, TermPtr expected);

  protected:

    Abs (TermPtr var, TermPtr body);
  };
};

#endif
