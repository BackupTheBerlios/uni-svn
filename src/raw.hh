#ifndef _UNI_TOKENS_HH_
#define _UNI_TOKENS_HH_

#include <tree.hh>
#include <style.hh>

#include <list>
#include <utility>

namespace NAMESPACE
{
  typedef std::pair<TermPtr,unsigned int> TOK;

  class Raw : public Tree,
	      private list <TOK>
  {
    DEF_TERM      (Raw);
    DEF_VISITABLE (Raw, raw);

  public:

    virtual TermPtr reduce (Context* context, int flags, TermPtr expected);
    virtual void visit_children (Visitor& visitor);

  protected:

    virtual bool xfreeof (TermPtr term) const;
    virtual TermPtr xsub (TermPtr var, TermPtr sub) const;

  public:

    void append (TermPtr term) { push_back(TOK (term, MAKE_STYLE (PREFIX, PREC_NOR))); }
    void append (const_iterator i, const_iterator j) { insert (end(), i, j); }
    bool empty () const { return list<TOK>::empty(); }

    RawPtr  deoutfix (Context* c);
    TermPtr construct (Context* context, int flags);

  private:

    void _update_style (Context* context);
    bool _rinfix (iterator& max, iterator& prev, iterator& next);
    int  _detach (Context* c, iterator li, iterator ri);
  };
};

#endif
