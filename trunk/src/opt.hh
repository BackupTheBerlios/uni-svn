#ifndef _UNI_OPT_HH_
#define _UNI_OPT_HH_

#include <tree.hh>

namespace NAMESPACE
{
  /**
   *
   */
  class AsType : public STree<1>
  {
    DEF_DYNAMIC  (AsType);
    TERM_TREEMAP (body, 0);

  protected:

    AsType () { }
    AsType (TermPtr t, TermPtr b) { _type = t; body (b); }
  };

  /**
   * IdOpt is the type of dummy node.
   * the sole purpose of its existence is to
   * provide a point where update can be done.
   */
  class IdOpt : public Term
  {
    DEF_TERMLET (IdOpt, Term);

  public:

    virtual TermPtr reduce (Machine* machine, int flags, TermPtr expected);
  };

//   class Let : public STree<2>
//   {
//     DEF_DYNAMIC  (Let);
//     TERM_TREEMAP (exp, 0);
//     TERM_TREEMAP (body, 1);
//     VAL_PROPERTY (TermPtr, var);

//   public:

//     virtual TermPtr reduce (Machine* machine, int flags, TermPtr expected);

//   protected:

//     Let () { }
//     Let (TermPtr v, TermPtr e, TermPtr b) : _var (v) { _type = UNK_T; exp (e); body (b); }
//   };
};

#endif
