#include <context.hh>
#include <proj.hh>

namespace NAMESPACE
{
  IMP_TYPE (Proj, "PROJ");

  TermPtr
  Proj::reduce (Context* c, int flags, TermPtr expected)
  {
    from (c->reduce (from(), c->current_flags()));
    to   (c->reduce (to(),   c->current_flags()));
    return TermPtr();
  }

  bool
  Proj::xcompat (TermPtr super) const
  {
    if (super->equ (Type::T))
      return true;
    else if (ProjPtr ps = CAST<Proj>(super))
      return from()->compat (ps->from()) && to()->compat (ps->to());
    else
      return false;
  }
};
