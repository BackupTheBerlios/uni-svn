#ifndef _UNI_SEQ_HH_
#define _UNI_SEQ_HH_

#include <tree.hh>

#include <vector>

namespace NAMESPACE
{
  class Seq : public Tree, private TermVec
  {
    DEF_TERMLET   (Seq, Term);
    DEF_VISITABLE (Seq, seq);

  public:

    virtual TermPtr reduce (Machine* machine, int flags, TermPtr expected);

    virtual unsigned int size() const { return TermVec::size(); }
    virtual void elem (unsigned int i, TermPtr e) { (*this)[i] = e; }
    virtual TermPtr elem (unsigned int i) const { return (*this)[i]; }

  protected:

    virtual bool xequ (TermPtr rhs) const;
    virtual bool xfreeof (TermPtr term) const;
    virtual TermPtr xsub (TermPtr var, TermPtr sub) const;

  public:

    void append (TermPtr term) { assert(term); push_back(term); }
  };

  class Ret : public Term
  {
    DEF_TERM      (Ret);
    DEF_VISITABLE (Ret, ret);
    VAL_PROPERTY  (TermPtr, value);

  protected:

    Ret (TermPtr value) : _value (value) { }
  };
};

#endif
