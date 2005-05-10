#include <exception.hh>
#include <tree.hh>

namespace NAMESPACE
{
  bool
  RTree::xcompat (TermPtr super) const
  {
    if (shared_ptr<RTree> r = CAST<RTree>(super)) {
      if (size() == r->size()) {
	for (unsigned int i = 0; i < size(); ++i)
	  if (! elem(i)->compat (r->elem(i)))
	    return false;

	return true;
      }
    }

    return false;
  }

  bool
  RTree::xequ (TermPtr rhs) const
  {
    if (shared_ptr<RTree> r = CAST<RTree>(rhs)) {
      if (size() == r->size()) {
	for (unsigned int i = 0; i < size(); ++i)
	  if (! elem(i)->equ (r->elem(i)))
	    return false;

	return true;
      }
    }

    return false;
  }

  bool
  RTree::xfreeof (TermPtr term) const
  {
    for (unsigned int i = 0; i < size(); ++i)
      if (! elem(i)->freeof (term))
	return false;

    return true;
  }

  TermPtr
  RTree::xsub (TermPtr var, TermPtr sub) const
  {
    shared_ptr<RTree> n = CAST<RTree> (clone());
    assert (n);
    bool changed = false;
    for (unsigned int i = 0; i < size(); ++i) {
      n->elem (i, elem(i)->sub (var, sub));
      if (n->elem(i) != elem(i))
	changed = true;
    }

    if (changed)
      return n;
    else
      return SELF;
  }

  TermPtr
  RTree::xsolve (TermPtr x, TermPtr rhs) const
  {
    // This function will check if right hand side
    // is also a r-tuple, if it is not, no solution.
    // Then it will solve e(x) = B for each element.
    // If any two (if there is) solutions of any e(x) = B
    // are different from each other, no solution.
    // Otherwise, the solution will be the common solutions.

    shared_ptr<RTree> b = CAST<RTree> (rhs);

    if ((!b) || (size() != b->size()))
      throw E (E_INCONSISTENT, x, SELF, b);

    TermPtr solution, tmp;
    for (unsigned int i = 0; i < size(); ++i) {
      try {
	TermPtr s = elem(i)->solve (x ,b->elem(i)); // solve the sub-equation
	if (solution && (! solution->equ(s)))       // if solutions are different...
	  throw E (E_INCONSISTENT, x, SELF, b);     // inconsistent equation.
	else                                        // if no previous solutions...
	  solution = s;                             // this is the solution.
      }
      catch (TermPtr et) {
	if (ExcPtr e = CAST<Exc> (et))
	  if (E_FREEVAR == e->id())
	    continue;                               // if x is free-var in the sub-equation, ignore it.

	throw;
      }                      
    }
    return solution;
  }
};
