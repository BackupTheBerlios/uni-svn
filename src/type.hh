#ifndef _UNI_TYPE_HH_
#define _UNI_TYPE_HH_

#include <string.hh>
#include <tree.hh>

namespace NAMESPACE
{
  class Type : public  MStr,
	       private TermVec
  {
    DEF_STATIC    (Type);
    DEF_VISITABLE (Type, type);

  public:

    /**
     * Add a super type to the type's super-type list.
     * This list is used to check if the type is a
     * sub-type of some other type.
     * Notice that adding a type to its own super-type
     * list will cause a fatal error.
     *
     * \param   super   the super-type
     */
    void add_super (TermPtr super) { assert (super.get() != this); push_back (super); }

  protected:

    virtual bool xcompat (TermPtr super) const;
    virtual bool xequ (TermPtr rhs) const;

  protected:

    Type (const char* name) : MStr(name) { }
  };

  class TypeTemplate : public STree<2>
  {
    DEF_TERMLET  (TypeTemplate, Type);
    TERM_TREEMAP (body, 0);
    TERM_TREEMAP (param, 1);

  protected:

    virtual bool xcompat (TermPtr super) const;
    virtual bool xequ (TermPtr rhs) const;

  protected:

    TypeTemplate (TermPtr body, TermPtr param);
  };
};

#endif
