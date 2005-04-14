#ifndef _UNI_LIST_HH_
#define _UNI_LIST_HH_

#include <term.hh>

namespace NAMESPACE
{
  class List : public  Term,
	       private list <TermPtr>
  {
    DEF_DYNAMIC   (List);
    DEF_VISITABLE (List, list);

  public:

    void insert_front (TermPtr item) { push_front (item); }
    void insert_back (TermPtr item) { push_back (item); }
    void visit_list (ListVisitor& visitor) const;

  protected:

    List () : Term (Term::T) { }
    List (TermPtr type) : Term (type) { }
  };
};

#endif
