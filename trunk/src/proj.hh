#ifndef _UNI_PROJ_HH_
#define _UNI_PROJ_HH_

#include <tree.hh>
#include <type.hh>

namespace NAMESPACE
{
  class Proj : public STree<2>
  {
    DEF_TERM      (Proj);
    DEF_VISITABLE (Proj, proj);
    TERM_TREEMAP  (from, 0);
    TERM_TREEMAP  (to,   1);

  public:

    virtual TermPtr reduce (Machine* machine, int flags, TermPtr expected);

  protected:

    virtual bool xcompat (TermPtr super) const;

  protected:

    Proj (TermPtr from, TermPtr to) { elem(0,from); elem(1,to); }
  };
};

#endif
