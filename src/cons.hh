#ifndef _UNI_CONS_HH_
#define _UNI_CONS_HH_

#include <tree.hh>

namespace NAMESPACE
{
  class Cons : public STree<2>
  {
    DEF_TYPEONLY  ();
    DEF_DYNAMIC   (Cons);
    DEF_VISITABLE (Cons, cons);
    TERM_TREEMAP  (left,  0);
    TERM_TREEMAP  (right, 1);

  protected:

    Cons () { assert (false); }
    Cons (TermPtr left, TermPtr right);
  };

  class Consh : public STree<2>
  {
    DEF_TERM      (Consh);
    DEF_VISITABLE (Consh, consh);
    TERM_TREEMAP  (left,  0);
    TERM_TREEMAP  (right, 1);

  public:

    virtual TermPtr type () const { return Consh::create (left()->type(), right()->type()); }

  protected:

    virtual bool xcompat (TermPtr super) const { return (super == Consh::T) || STree<2>::xcompat (super); }

  protected:

    Consh (TermPtr left, TermPtr right);
  };
};

#endif
