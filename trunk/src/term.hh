#ifndef _UNI_TERM_HH_
#define _UNI_TERM_HH_

#include <basicdef.hh>
#include <typedef.hh>
#include <visitor.hh>

#include <iostream>
#include <vector>

namespace NAMESPACE
{
  class Context;

  /**
   * Abstract base class for all terms.
   * All the term classes should derive from this class
   * or its derivatives.
   */
  class Term
  {
    DEF_TAG  ();
    DEF_TYPE ();

    VAL_PROPERTY (SrcPtr, src);

  public:

    static unsigned int term_count;

  public:

    /**
     * The type of the term.
     *
     * \return   the type of the term.
     */
    virtual TermPtr type () const { return _type; }

    /**
     * Clone the term, that is, create a new term of the same type,
     * and exactly the same internal content with the term.
     *
     * \return   the clone of the term
     */
    virtual TermPtr clone () const = 0;

    /**
     * Clone the term with a new type.
     * In another word, create a new copy of the term,
     * that is identical with the given term, except
     * its type.
     *
     * \param   type   the type for newly created copy.
     * \return  the clone of the term (with new type).
     */
    virtual TermPtr clone (TermPtr type) const = 0;

    /**
     * Reduce the term into a simpler form.
     *
     * \param   context  the the context in which
     *                   the term is to be reduced.
     *
     * \param   flags    flags for reduction, can be a bit
     *                   combination any flags defined in
     *                   the reduction flags section.
     *
     * \param   expected the expected type to be reduced into.
     *                   if the caller have no expectation,
     *                   then this can be an empty pointer.
     *
     * \return  the result of the reduction.
     *          if the term is not reducible,
     *          the result will be empty pointer.
     */
    virtual TermPtr reduce (Context* context, int flags, TermPtr expected);

    virtual void msolve (const TermVec& unknowns, TermPtr rhs, TermMap& solutions) const;

    virtual void visit (Visitor& visitor) { visitor.visit_term (SELF); }
    virtual void visit_children (Visitor& visitor) { assert (false); }

  public:

    /**
     * Test if the term is compatible with another term.
     * In another word, to see if the term represent a
     * sub-type of a given super type.
     * If the term is not a type object,
     * then the term is compatible with given super type,
     * if they are referencially equal to each other,
     * or the given super type is ANY (Term::T).
     * If the term is a type object (derived from Type),
     * then their relationship will be further checked on
     * the type graph.
     *
     * \retval   true   the term is compatible with the
     *                  super term, and thus can be used
     *                  wherever the super term is used
     *
     * \retval   false  the term is not compatible with
     *                  the given super term.
     */
    bool compat (TermPtr super) const;

    /**
     * Test if the term is syntactically equivalent to a given term.
     * The pointer of the two will be compared, and terms will be
     * assumed to be identical if their pointer is the same.
     * If that is not the case, the type will be compared, if the
     * type of the terms (reference wise) identical, then xequ will
     * be called to let derived class to determine it.
     *
     * \param    rhs    the other term to be compared to.
     * \retval   true   the two terms are syntactically equivalent.
     * \retval   false  the two terms are syntactically different.
     */
    bool equ (TermPtr rhs) const
    {
      return (this == rhs.get()) || (type() == rhs->type() && xequ(rhs));
    }

    /**
     * Test if the term is free of another term.
     * A term is free of another term if and only if
     * they do not equal to each other and the term
     * does not contain the other term as a sub term.
     *
     * \param   term   the other term.
     * \retval  true   the term is free of the other.
     * \retval  false  the term is not free of the other.
     */
    bool freeof (TermPtr term) const { return (!equ (term)) && xfreeof (term); }

    /**
     * Substitute a term or sub-term with another term.
     * In term of lambda calculus, this operation is
     * basically the beta reduction.
     *
     * \param   var   the target term to be replaced.
     * \param   sub   the new term to be filled in.
     * \return  the newly created term in which all the
     *          occurence of term var is substituted by
     *          the term sub.
     */
    TermPtr sub (TermPtr var, TermPtr sub) const;
    TermPtr solve (TermPtr unknown, TermPtr rhs) const;

    TermPtr lock () { assert (! _ptr.expired()); return _ptr.lock(); }

  protected:

    virtual bool xcompat (TermPtr super) const;
    virtual bool xequ (TermPtr rhs) const;
    virtual bool xfreeof (TermPtr term) const;
    virtual TermPtr xsub (TermPtr var, TermPtr sub) const;
    virtual TermPtr xsolve (TermPtr unknown, TermPtr rhs) const;

  protected:

    TermPtr  _type; /** the type of the term */
    TermPtrW _ptr;  /** the weak pointer pointed to the term itself */

  protected:

    Term ();
    Term (TermPtr type);
    virtual ~Term ();
  };

  class Const : public Term
  {
    DEF_DYNAMIC   (Const);
    DEF_VISITABLE (Const, const);
    REF_PROPERTY  (string, name);

  public:

    virtual TermPtr type () const;

  protected:

    Const () { assert (false); }
    Const (const string& name) : Term(Term::T), _name(name), _typed(false) { }
    Const (TermPtr type, const string& name) : Term(type), _name(name), _typed(true) { }

  private:

    bool _typed;
  };

  extern const TermPtr AMB_T;
  extern const TermPtr DEP_T;
  extern const TermPtr UPR_T;
  extern const TermPtr VOID_T;
  extern const TermPtr VOID;
  extern const TermPtr NIL;
};

#endif
