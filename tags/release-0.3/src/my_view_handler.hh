#ifndef _UNI_MY_VIEW_HANDLER_
#define _UNI_MY_VIEW_HANDLER_

#include <handler.hh>
#include <visitor.hh>

namespace NAMESPACE
{
  enum { C_NOR = 0, C_SEP, C_BOOL, C_DICT,
	 C_FAM, C_FUNC, C_CONST,
	 C_INT, C_RET, C_SCOPE, C_STR, C_SYM,
	 C_TEMP, C_TOK, C_TYPE, C_VAR, C_E, C_TERM, C_SIZE };

  class MyViewHandler : public ViewHandler,
			public DictVisitor,
			public ListVisitor
  {
  private:

    static vector <string> _default_cols;

  private:

    const vector <string>& _c;

  public:

    MyViewHandler (std::ostream& os, const vector<string>& cols = _default_cols)
      : ViewHandler (os), _c (cols) { }

  public:

    virtual void visit_abs    (AbsPtr   ptr);
    virtual void visit_app    (AppPtr   ptr);
    virtual void visit_bool   (BoolPtr  ptr);
    virtual void visit_cons   (ConsPtr  ptr);
    virtual void visit_consh  (ConshPtr ptr);
    virtual void visit_const  (ConstPtr ptr);
    virtual void visit_dict   (DictPtr  ptr);
    virtual void visit_exc    (ExcPtr   ptr);
    virtual void visit_fam    (FamPtr   ptr);
    virtual void visit_func   (FuncPtr  ptr);
    virtual void visit_int    (IntPtr   ptr);
    virtual void visit_list   (ListPtr  ptr);
    virtual void visit_proj   (ProjPtr  ptr);
    virtual void visit_raw    (RawPtr   ptr);
    virtual void visit_ret    (RetPtr   ptr);
    virtual void visit_seq    (SeqPtr   ptr);
    virtual void visit_str    (StrPtr   ptr);
    virtual void visit_sym    (SymPtr   ptr);
    virtual void visit_temp   (TempPtr  ptr);
    virtual void visit_tok    (TokPtr   ptr);
    virtual void visit_type   (TypePtr  ptr);
    virtual void visit_var    (VarPtr   ptr);
    virtual void visit_term   (TermPtr  ptr);

    virtual void visit_dict_item (const string& key, TermPtr val);
    virtual void visit_dict_break ();

    void visit_list_item (TermPtr term);
    void visit_list_break ();
  };
};

#endif
