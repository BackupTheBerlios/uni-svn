#include <cons.hh>
#include <type.hh>

namespace NAMESPACE
{
  IMP_TYPE (Cons, "CONS");
  IMP_TYPE (Consh, "CONSH");

  Cons::Cons (TermPtr l, TermPtr r)
  {
    assert (l);
    assert (r);

    left (l);
    right (r);

    _type = TypeTemplate::create (T, l->type());
  }

  Consh::Consh (TermPtr l, TermPtr r)
  {
    assert (l);
    assert (r);

    left (l);
    right (r);
  }
};
