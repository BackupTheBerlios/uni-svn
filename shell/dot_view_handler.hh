#ifndef _DOT_VIEW_HANDLER_
#define _DOT_VIEW_HANDLER_

#include <handler.hh>
#include <visitor.hh>

namespace NAMESPACE
{
  class DotViewHandler : public ViewHandler
  {
  public:

    DotViewHandler (std::ostream& os) : ViewHandler (os) { }

  public:

    virtual void visit_abs    (AbsPtr ptr);
    virtual void visit_app    (AppPtr ptr);
//     virtual void visit_bool   (BoolPtr ptr);
//     virtual void visit_cons   (ConsPtr ptr);
//     virtual void visit_consh  (ConshPtr ptr);
//     virtual void visit_ext    (ExtPtr   ptr);
//     virtual void visit_fam    (FamPtr   ptr);
//     virtual void visit_func   (FuncPtr  ptr);
    virtual void visit_int    (IntPtr   ptr);
//     virtual void visit_proj   (ProjPtr  ptr);
    virtual void visit_raw    (RawPtr   ptr);
//     virtual void visit_ret    (RetPtr   ptr);
//     virtual void visit_scope  (ScopePtr ptr);
//     virtual void visit_seq    (SeqPtr   ptr);
//     virtual void visit_str    (StrPtr   ptr);
    virtual void visit_sym    (SymPtr   ptr);
//     virtual void visit_temp   (TempPtr  ptr);
    virtual void visit_tok    (TokPtr   ptr);
//     virtual void visit_type   (TypePtr  ptr);
//     virtual void visit_var    (VarPtr   ptr);
//     virtual void visit_e_head (EHeadPtr ptr);
    virtual void visit_term   (TermPtr  ptr);
  };
};

#endif
