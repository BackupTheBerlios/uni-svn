#include <exception.hh>
#include <raw.hh>
#include <term.hh>
#include <type.hh>

namespace NAMESPACE
{
  IMP_TYPE (Term, "ANY");

  unsigned int Term::term_count = 0;

  Term::Term () { ++term_count; }
  Term::Term (TermPtr type) : _type (type) { ++term_count; }
  Term::~Term () { --term_count; }

  TermPtr
  Term::reduce (Context* context, int flags, TermPtr expected)
  {
    return TermPtr();
  }

  bool
  Term::compat (TermPtr super) const
  {
    if (Term::T == super)
      return true;
    else if (Raw::T == super)
      return true;
    else if (this == super.get())
      return true;
    else
      return xcompat (super);
  }

  TermPtr
  Term::sub (TermPtr var, TermPtr sub) const
  {
    if (equ (var)) // if [x/y] x
      return sub;  // [x/y] x = y
    else
      return xsub (var, sub);
  }

  TermPtr
  Term::solve (TermPtr x, TermPtr b) const
  {
    if (equ (b))                       // if equ. is B = B or x = x...
      throw E (E_FREEVAR, x, SELF, b); // x is free variable.
    else if (equ (x))                  // if equ. is x = B...
      return b;                        // the solution is simply x = B.
    else if (b->equ(x))                // if equ. is A = x
      return SELF;                     // the solution is x = A.
    else                               // otherwise...
      return xsolve (x, b);            // ask sub-class to solve it.
  }

  void
  Term::msolve (const TermVec& xs, TermPtr rhs, TermMap& ss) const
  {
    bool solved = false;

    for (TermVec::const_iterator i = xs.begin(); i != xs.end(); ++i) {
      try {
	TermPtr solution = solve (*i, rhs);
	if ((ss.end() != ss.find (*i)) && (! ss[*i]->equ (solution)))
	  throw E (E_INCONSISTENT, *i, SELF, rhs);
	ss [*i] = solution;
	solved = true;
      }
      catch (TermPtr et) {
	if (ExcPtr e = CAST<Exc> (et))
	  if (E_INCONSISTENT == e->id() || E_FREEVAR == e->id())
	    continue;

	throw;
      }
    }

    if (! solved)
      throw E (E_INCONSISTENT, xs.front(), SELF, rhs);
  }

  bool Term::xcompat (TermPtr super) const { return false; }
  bool Term::xequ (TermPtr rhs) const { return false; }
  bool Term::xfreeof (TermPtr term) const { return true; }
  TermPtr Term::xsub (TermPtr var, TermPtr sub) const { return SELF; }
  TermPtr Term::xsolve (TermPtr x, TermPtr b) const { throw E (E_INCONSISTENT, x, SELF, b); }

  TermPtr
  Const::type () const
  {
    if (_typed)
      return Term::type();
    else
      return SELF;
  }

  const TermPtr AMB_T  = Type::create ("AMB");
  const TermPtr DEP_T  = Type::create ("DEP");
  const TermPtr UPR_T  = Type::create ("UPR");
  const TermPtr VOID_T = Type::create ("VOID");
  const TermPtr VOID   = Const::create (VOID_T, "void");
  const TermPtr NIL    = Const::create ("nil");
};
