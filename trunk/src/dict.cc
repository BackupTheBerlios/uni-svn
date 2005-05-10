#include <dict.hh>

namespace NAMESPACE
{
  TermPtr
  Dict::get (const string& name) const
  {
    const_iterator iter = find (name);
    if (end() == iter)
      return TermPtr();
    else
      return iter->second;
  }

  void
  Dict::set (const string& name, TermPtr term)
  {
    (*this) [name] = term;
  }

  bool
  Dict::has (const string& name)
  {
    return end() != find (name);
  }

  unsigned int
  Dict::size () const
  {
    return map<string,TermPtr>::size();
  }

  void
  Dict::visit_dict (DictVisitor& visitor) const
  {
    if (! empty()) {
      const_iterator iter = begin();
      visitor.visit_dict_item (iter->first, iter->second);
      for (++iter; iter != end(); ++iter) {
	visitor.visit_dict_break();
	visitor.visit_dict_item (iter->first, iter->second);
      }
    }
  }
};
