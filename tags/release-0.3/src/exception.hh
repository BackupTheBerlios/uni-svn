#ifndef _UNI_EXCEPTION_HH_
#define _UNI_EXCEPTION_HH_

#include <term.hh>

#include <string>

#define E(...) ((TermPtr) Exc::create (__VA_ARGS__))

namespace NAMESPACE
{
  enum exc_t { E_ASSIGNMENT,      /** assign value to a variable of imcompatiable type */
	       E_ARG,
	       E_CAST,
	       E_DUP_NAME,
	       E_HEAD,
	       E_MODE,
	       E_INDEX,
	       E_NO_KEY,
	       E_NO_LIB,
	       E_NO_MATCH,
	       E_FREEVAR,         /** the variable is a free variable in the equation */
	       E_INCONSISTENT,    /** the equation is inconsistent */
	       E_MISSING_SPECIAL,
	       E_OUTFIX_MISMATCH, /** the pair of outfix opt. was not defined as a pair */
	       E_OUTFIX_L,        /** a lonely left outfix opt. no right opt. to match */
	       E_OUTFIX_R,        /** a lonely right outfix opt. no left opt. to match */
	       E_USER             /** identifiers after this can be interpret by users */
  };

  class Exc : public Term
  {
    DEF_TERM      (Exc);
    DEF_VISITABLE (Exc,     exc);
    VAL_PROPERTY  (int,     id);
    VAL_PROPERTY  (TermPtr, term1);
    VAL_PROPERTY  (TermPtr, term2);
    VAL_PROPERTY  (TermPtr, term3);

  public:

    Exc (int id, TermPtr t1 = TermPtr(), TermPtr t2 = TermPtr(), TermPtr t3 = TermPtr())
      : _id(id), _term1(t1), _term2(t2), _term3(t3) { }
  };

  template <typename t>
  shared_ptr<t> TCAST (TermPtr p)
  {
    if (shared_ptr<t> r = dynamic_pointer_cast<t>(p))
      return r;
    else
      throw E (E_CAST, p, p->type(), t::T);
  }
};

#endif
