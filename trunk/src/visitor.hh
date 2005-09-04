#ifndef _UNI_VISITOR_HH_
#define _UNI_VISITOR_HH_

#include <basicdef.hh>
#include <typedef.hh>

namespace NAMESPACE
{
  class Visitor
  {
  public:

    virtual ~Visitor () { }

    virtual void visit_abs   (AbsPtr   ptr) { }
    virtual void visit_app   (AppPtr   ptr) { }
    virtual void visit_bool  (BoolPtr  ptr) { }
    virtual void visit_cons  (ConsPtr  ptr) { }
    virtual void visit_consh (ConshPtr ptr) { }
    virtual void visit_const (ConstPtr ptr) { }
    virtual void visit_cptr  (CPtrPtr  ptr) { }
    virtual void visit_exc   (ExcPtr   ptr) { }
    virtual void visit_fam   (FamPtr   ptr) { }
    virtual void visit_func  (FuncPtr  ptr) { }
    virtual void visit_int   (IntPtr   ptr) { }
    virtual void visit_proj  (ProjPtr  ptr) { }
    virtual void visit_raw   (RawPtr   ptr) { }
    virtual void visit_ret   (RetPtr   ptr) { }
    virtual void visit_scope (ScopePtr ptr) { }
    virtual void visit_seq   (SeqPtr   ptr) { }
    virtual void visit_space (SpacePtr ptr) { }
    virtual void visit_str   (StrPtr   ptr) { }
    virtual void visit_sym   (SymPtr   ptr) { }
    virtual void visit_temp  (TempPtr  ptr) { }
    virtual void visit_tok   (TokPtr   ptr) { }
    virtual void visit_type  (TypePtr  ptr) { }
    virtual void visit_var   (VarPtr   ptr) { }

    virtual void visit_term  (TermPtr  ptr) { }
  };
};

#endif
