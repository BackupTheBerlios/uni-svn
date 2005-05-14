#ifndef _UNI_TREE_HH_
#define _UNI_TREE_HH_

#include <term.hh>
#include <exception.hh>

namespace NAMESPACE
{
  /**
   * Abstract base class for all terms that has sub-terms
   */
  class Tree : public Term
  {
  protected:

    virtual bool xfreeof (TermPtr term) const = 0;
    virtual TermPtr xsub (TermPtr var, TermPtr sub) const = 0;
  };

  class RTree : public Tree
  {
  public:

    virtual unsigned int size() const = 0;
    virtual void elem (unsigned int i, TermPtr e) = 0;
    virtual TermPtr elem (unsigned int i) const = 0;

  protected:

    virtual bool xcompat (TermPtr super) const;
    virtual bool xequ (TermPtr rhs) const;
    virtual bool xfreeof (TermPtr term) const;
    virtual TermPtr xsub (TermPtr var, TermPtr sub) const;
    virtual TermPtr xsolve (TermPtr x, TermPtr rhs) const;
  };

  template <unsigned int N>
  class STree : public RTree
  {
    typedef STree<N> _CLASS;
    typedef shared_ptr<_CLASS> _PTR;

  protected:

    virtual bool xequ (TermPtr rhs) const
    {
      return CAST<_CLASS>(rhs) ? RTree::xequ (rhs) : false;
    }

    virtual TermPtr xsolve (TermPtr x, TermPtr rhs) const
    {
      if (CAST<_CLASS> (rhs))
	return RTree::xsolve (x, rhs);
      else
	throw E (E_INCONSISTENT, x, SELF, rhs);
    }

  public:

    virtual unsigned int size() const { return N; }
    virtual void elem (unsigned int i, TermPtr e) { _elems[i] = e; }
    virtual TermPtr elem (unsigned int i) const { return _elems[i]; }

  private:

    TermPtr _elems [N];
  };
};

#endif
