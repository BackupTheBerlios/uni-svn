#include <abs.hh>
#include <app.hh>
#include <cons.hh>
#include <family.hh>
#include <func.hh>
#include <proj.hh>
#include <string.hh>
#include <tok.hh>
#include <sym.hh>
#include <int.hh>
#include <seq.hh>
#include <func.hh>
#include <bool.hh>
#include <temp.hh>

#include <my_view_handler.hh>

namespace NAMESPACE
{
  vector <string> MyViewHandler::_default_cols (C_SIZE);

  void MyViewHandler::visit_abs (AbsPtr ptr)
  {
    *this << _c[C_SEP] << "\\"
	  << ptr->bv()
	  << _c[C_SEP] << "."
	  << ptr->body();
  }

  void MyViewHandler::visit_app (AppPtr ptr)
  {
    ptr->rator()->visit (*this);

    if (CAST<Tree> (ptr->rand())) {
      *this << ' ' << _c[C_SEP] << '(';
      ptr->rand()->visit (*this);
      *this << _c[C_SEP] << ')';
    }
    else {
      *this << ' ';
      ptr->rand()->visit (*this);
    }
  }

  void MyViewHandler::visit_bool (BoolPtr ptr) { *this << _c[C_BOOL] << (ptr->val() ? "true" : "false"); }

  void MyViewHandler::visit_cons (ConsPtr ptr)
  {
    ptr->left()->visit (*this);
    *this << _c[C_SEP] << " | ";
    ptr->right()->visit (*this);
  }

  void MyViewHandler::visit_consh (ConshPtr ptr)
  {
    *this << ptr->left() << _c[C_SEP] << ", " << ptr->right();
  }

  void MyViewHandler::visit_const (ConstPtr ptr)
  {
    *this << _c[C_CONST] << ptr->name();
  }

  void MyViewHandler::visit_exc (ExcPtr ptr)
  {
    switch (ptr->id()) {
    case E_ASSIGNMENT:
      *this << _c[C_NOR] << "cannot assign " << ptr->term2()
	    << _c[C_NOR] << " to " << ptr->term1();
      break;

    case E_HEAD:
      *this << _c[C_NOR] << "head term " << ptr->term1()
	    << _c[C_NOR] << " cannot be reduced.";
      break;

    case E_MODE:
      *this << _c[C_NOR] << "the mode is not enough to reduce " << ptr->term1();
      break;

    case E_CAST:
      *this << _c[C_NOR] << "cannot cast " << ptr->term1()
	    << _c[C_NOR] << " from " << ptr->term2()
	    << _c[C_NOR] << " to " << ptr->term3();
      break;

    case E_NO_LIB:
      *this << _c[C_NOR] << "cannot find library: " << ptr->term1();
      break;

    case E_NO_MATCH:
      *this << _c[C_NOR] << "cannot find function " << ptr->term1()
	    << _c[C_NOR] << " with type " << ptr->term2();
      break;

    case E_NO_KEY:
      *this << _c[C_NOR] << "cannot find key " << ptr->term2()
	    << _c[C_NOR] << " in dictionary " << ptr->term1();
      break;

    case E_INDEX:
      *this << _c[C_NOR] << "index " << ptr->term2()
	    << _c[C_NOR] << " is out of the range of " << ptr->term1();
      break;

    case E_DUP_NAME:
      *this << _c[C_NOR] << "the symbol with name " << ptr->term1()
	    << _c[C_NOR] << " has already defined ";
      break;

    case E_FREEVAR:
      *this << _c[C_NOR] << ptr->term1()
	    << _c[C_NOR] << " is a free variable in " << ptr->term2();
      break;

    case E_INCONSISTENT:
      *this << _c[C_NOR] << "equation " << ptr->term1()
	    << _c[C_NOR] << " = " << ptr->term2()
	    << _c[C_NOR] << " is inconsistent" << ptr->term3();
      break;

    case E_MISSING_SPECIAL:
      *this << _c[C_NOR] << "special name " << ptr->term1()
	    << _c[C_NOR] << " is missing";
      break;

    case E_OUTFIX_MISMATCH:
      *this << _c[C_NOR] << "outfix operators " << ptr->term1()
	    << _c[C_NOR] << " and" << ptr->term2()
	    << _c[C_NOR] << " do not match";
      break;

    case E_OUTFIX_L:
      *this << _c[C_NOR] << "outfix operator " << ptr->term1()
	    << _c[C_NOR] << " is not closed";
      break;

    case E_OUTFIX_R:
      *this << _c[C_NOR] << "no left outfix operators to match with" << ptr->term1();
      break;

    default:
      *this << _c[C_SEP] << "EXC: " << ptr->id();
      break;
    }
  }

  void MyViewHandler::visit_fam   (FamPtr   ptr) { *this << _c[C_FAM] << ptr->name(); }
  void MyViewHandler::visit_func  (FuncPtr  ptr) { *this << _c[C_FUNC] << ptr->name(); }
  void MyViewHandler::visit_int   (IntPtr   ptr) { *this << _c[C_INT] << ptr->val(); }

  void MyViewHandler::visit_proj  (ProjPtr  ptr)
  {
    if (CAST<Tree> (ptr->from())) {
      *this << _c[C_SEP] << '(';
      ptr->from()->visit (*this);
      *this << _c[C_SEP] << ')';
    }
    else
      ptr->from()->visit (*this);

    *this << _c[C_SEP] << "->";
    ptr->to()->visit (*this);
  }

  void MyViewHandler::visit_raw (RawPtr ptr)
  {
    *this << _c[C_SEP] << "<";
    ptr->visit_children (*this);
    *this << _c[C_SEP] << ">";
  }

  void MyViewHandler::visit_ret   (RetPtr   ptr) { }
  void MyViewHandler::visit_seq   (SeqPtr   ptr)
  {
    *this << _c[C_SEP] << '{';
    for (unsigned int i = 0; i < ptr->size(); ++i) {
      ptr->elem(i)->visit (*this);
      *this << _c[C_SEP] << "; ";
    }
    *this << _c[C_SEP] << '}';
  }

  void MyViewHandler::visit_str   (StrPtr   ptr) { *this << _c[C_STR] << '\"' << ptr->str() << '\"'; }
  void MyViewHandler::visit_sym   (SymPtr   ptr) { *this << _c[C_SYM] << ptr->str(); }
  void MyViewHandler::visit_temp  (TempPtr  ptr) { *this << _c[C_TEMP] << ptr->name(); }
  void MyViewHandler::visit_tok   (TokPtr   ptr) { *this << _c[C_TOK] << ptr->str(); }
  void MyViewHandler::visit_type  (TypePtr  ptr) { *this << _c[C_TYPE] << ptr->str(); }
  void MyViewHandler::visit_var   (VarPtr   ptr) { *this << _c[C_VAR] << "$" << ptr->id(); }

  void MyViewHandler::visit_term  (TermPtr ptr)
  {
    *this << _c[C_TERM] << "?";
  }
};
