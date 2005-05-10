#include <type.hh>

namespace NAMESPACE
{
  IMP_TYPE (Type, "TYPE");

  bool
  Type::xcompat (TermPtr super) const
  {
    for (const_iterator i = begin(); i != end(); ++i)
      if ((*i)->compat(super))
	return true;

    return false;
  }

  bool
  Type::xequ (TermPtr rhs) const
  {
    return CAST<Type> (rhs) && Str::xequ (rhs);
  }

  //////////////////////////////////////////////////////////////////////////////
  bool
  TypeTemplate::xequ (TermPtr rhs) const
  {
    if (P(TypeTemplate) tt = CAST<TypeTemplate> (rhs))
      return body()->equ (tt->body()) && param()->equ (tt->param());
    else
      return false;
  }

  bool
  TypeTemplate::xcompat (TermPtr super) const
  {
    // A type template is compatiable with another
    // type template if and only if both its head
    // and its argument are compatiable with that
    // of the other.

    if (super->equ (Type::T))
      return true;
    else if (P(TypeTemplate) tt = CAST<TypeTemplate> (super))
      return body()->compat (tt->body()) && param()->compat (tt->param());
    else
      return false;
  }

  TypeTemplate::TypeTemplate (TermPtr b, TermPtr p)
  {
    assert (b);
    assert (p);

    body (b);
    param (p);
  }
};
