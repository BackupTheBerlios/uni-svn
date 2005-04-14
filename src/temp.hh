#ifndef _UNI_TEMP_HH_
#define _UNI_TEMP_HH_

#include <tree.hh>

namespace NAMESPACE
{
  /*
   * The class of template functions.
   * Template functions are functions whose type signature
   * and/or behavior is depending on the type of the arguments
   * being supplied. In another word, Template function is a
   * generalizations of polymorphic functions.
   */
  class Temp : public Term
  {
    DEF_TERMLET   (Temp,    Term);
    DEF_VISITABLE (Temp,    temp);
    REF_PROPERTY  (string,  name);
    REF_PROPERTY  (TermVec, unknowns);
    VAL_PROPERTY  (TermPtr, type_temp);
    VAL_PROPERTY  (TermPtr, body_temp);

  public:

    virtual TermPtr reduce (Context* context, int flags, TermPtr expected);

  protected:

    Temp (const string& name, TermPtr var, TermPtr type, TermPtr body);
    Temp (const string& name, TermVec& vars, TermPtr type, TermPtr body);
  };

  class Instance : public STree<1>
  {
    DEF_DYNAMIC  (Instance);
    TERM_TREEMAP (body, 0);

  public:

    virtual TermPtr reduce (Context* context, int flags, TermPtr expected);

  protected:

    Instance () { assert (false); }
    Instance (TermPtr t, TermPtr b) { _type = t; body(b); }
  };
};

#endif
