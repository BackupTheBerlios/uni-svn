#ifndef _UNI_DICT_HH_
#define _UNI_DICT_HH_

#include <term.hh>

namespace NAMESPACE
{
  class Dict : public Term,
	       private map<string,TermPtr>
  {
    DEF_DYNAMIC   (Dict);
    DEF_VISITABLE (Dict, dict);

  public:

    TermPtr get (const string& name) const;
    unsigned int size () const;
    void set (const string& name, TermPtr term);
    bool has (const string& name);
    void visit_dict (DictVisitor& visitor) const;

  protected:

    Dict () : Term (Term::T) { }
    Dict (TermPtr type) : Term (type) { }
  };
};

#endif
