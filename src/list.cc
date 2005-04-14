#include <list.hh>

namespace NAMESPACE
{
  void
  List::visit_list (ListVisitor& visitor) const
  {
    if (! empty()) {
      const_iterator iter = begin();
      visitor.visit_list_item (*iter);
      for (++iter; iter != end(); ++iter) {
	visitor.visit_list_break ();
	visitor.visit_list_item (*iter);
      }
    }
  }
};
