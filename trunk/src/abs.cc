#include <abs.hh>
#include <machine.hh>

namespace NAMESPACE
{
  unsigned int BoundVar::_next_id = 0;

  Abs::Abs (TermPtr v, TermPtr b)
  {
    assert (v && b);

    bv (BoundVar::create());
    body (b->sub (v, bv()));
  }

  TermPtr
  Abs::reduce (Machine* m, int flags, TermPtr expected)
  {
    if (m->arg_count())
      return body()->sub (bv(), m->pop());
    else
      return TermPtr();
  }
};
