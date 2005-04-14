#include <abs.hh>
#include <context.hh>
#include <exception.hh>
#include <sym.hh>
#include <proj.hh>
#include <reflection.hh>

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
  Abs::reduce (Context* c, int flags, TermPtr expected)
  {
    if (c->arg_count())
      return body()->sub (bv(), c->pop());
    else
      return TermPtr();
  }
};
