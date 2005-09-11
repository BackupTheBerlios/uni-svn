#include <abs.hh>
#include <app.hh>
#include <family.hh>
#include <func.hh>
#include <handle.hh>
#include <proj.hh>
#include <string.hh>
#include <tok.hh>
#include <sym.hh>
#include <int.hh>
#include <seq.hh>
#include <func.hh>
#include <bool.hh>
#include <temp.hh>

#include <vector>
#include <map>
#include <iostream>
#include <sstream>

#define RED        "\033[0;31m"
#define GREEN      "\033[0;32m"
#define YELLOW     "\033[0;33m"
#define BLUE       "\033[0;34m"
#define MAGENTA    "\033[0;35m"
#define CYAN       "\033[0;36m"
#define WHITE      "\033[0;37m"

#define B_RED      "\033[1;31m"
#define B_GREEN    "\033[1;32m"
#define B_YELLOW   "\033[1;33m"
#define B_BLUE     "\033[1;34m"
#define B_MAGENTA  "\033[1;35m"
#define B_CYAN     "\033[1;36m"
#define B_WHITE    "\033[1;37m"

#define COL_NORMAL "\033[0m"

using namespace NAMESPACE;
using namespace std;

enum { C_NOR = 0, C_SEP, C_BOOL, C_DICT,
       C_FAM, C_FUNC, C_CONST,
       C_INT, C_RET, C_SCOPE, C_STR, C_SYM,
       C_TEMP, C_TOK, C_TYPE, C_VAR, C_E, C_TERM, C_SIZE };

vector <string> cols (C_SIZE);

class ViewHandler : public Visitor
{
private:

  std::ostream& _os;
  const vector <string> _c;

public:

  ViewHandler (std::ostream& os) : _os (os) { }
  ViewHandler (std::ostream& os, const vector<string>& cols) : _os (os), _c (cols) { }

public:

  virtual void visit_abs    (AbsPtr   ptr);
  virtual void visit_app    (AppPtr   ptr);
  virtual void visit_bool   (BoolPtr  ptr);
  virtual void visit_const  (ConstPtr ptr);
  virtual void visit_exc    (ExcPtr   ptr);
  virtual void visit_fam    (FamPtr   ptr);
  virtual void visit_func   (FuncPtr  ptr);
  virtual void visit_int    (IntPtr   ptr);
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

public:

  ViewHandler& operator << (TermPtr term) { term->visit (*this); return *this; }
  ViewHandler& operator << (const string& x) { _os << x; return *this; }
  ViewHandler& operator << (char x) { _os << x; return *this; }
  ViewHandler& operator << (int x) { _os << x; return *this; }
  ViewHandler& operator << (unsigned int x) { _os << x; return *this; }
};

void ViewHandler::visit_abs (AbsPtr ptr)
{
  *this << _c[C_SEP] << "\\"
	<< ptr->bv()
	<< _c[C_SEP] << "."
	<< ptr->body();
}

  void ViewHandler::visit_app (AppPtr ptr)
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

  void ViewHandler::visit_bool (BoolPtr ptr) { *this << _c[C_BOOL] << (ptr->val() ? "true" : "false"); }

  void ViewHandler::visit_const (ConstPtr ptr)
  {
    *this << _c[C_CONST] << ptr->name();
  }

  void ViewHandler::visit_exc (ExcPtr ptr)
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

  void ViewHandler::visit_fam   (FamPtr   ptr) { *this << _c[C_FAM] << "*"; }
  void ViewHandler::visit_func  (FuncPtr  ptr) { *this << _c[C_FUNC] << ptr->name(); }
  void ViewHandler::visit_int   (IntPtr   ptr) { *this << _c[C_INT] << ptr->val(); }

  void ViewHandler::visit_proj  (ProjPtr  ptr)
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

  void ViewHandler::visit_raw (RawPtr ptr)
  {
    *this << _c[C_SEP] << "<";
    ptr->visit_children (*this);
    *this << _c[C_SEP] << ">";
  }

  void ViewHandler::visit_ret   (RetPtr   ptr) { }
  void ViewHandler::visit_seq   (SeqPtr   ptr)
  {
    *this << _c[C_SEP] << '{';
    for (unsigned int i = 0; i < ptr->size(); ++i) {
      ptr->elem(i)->visit (*this);
      *this << _c[C_SEP] << "; ";
    }
    *this << _c[C_SEP] << '}';
  }

  void ViewHandler::visit_str   (StrPtr   ptr) { *this << _c[C_STR] << '\"' << ptr->str() << '\"'; }
  void ViewHandler::visit_sym   (SymPtr   ptr) { *this << _c[C_SYM] << ptr->str(); }
  void ViewHandler::visit_temp  (TempPtr  ptr) { *this << _c[C_TEMP] << ptr->name(); }
  void ViewHandler::visit_tok   (TokPtr   ptr) { *this << _c[C_TOK] << ptr->str(); }
  void ViewHandler::visit_type  (TypePtr  ptr) { *this << _c[C_TYPE] << ptr->str(); }
  void ViewHandler::visit_var   (VarPtr   ptr) { *this << _c[C_VAR] << "$" << ptr->id(); }

  void ViewHandler::visit_term  (TermPtr ptr)
  {
    *this << _c[C_TERM] << "?";
  }

static TermPtr
str (TermPtr term)
{
  if (CAST<MStr> (term) && (Str::T == term->type()))
    return term;
  else {
    std::ostringstream os;
    ViewHandler viewer (os);
    viewer << term;
    return MStr::create (os.str().c_str());
  }
}

static TermPtr
str_ansi (TermPtr term)
{
  std::ostringstream os;
  ViewHandler viewer (os, cols);
  viewer << term;
  return MStr::create (os.str().c_str());
}

static ext_t _exts[] = {
  {"str",      SimpleFunc::create (1, PURE, (void*)str, Proj::create (Term::T, Str::T))},
  {"str_ansi", SimpleFunc::create (1, PURE, (void*)str_ansi, Proj::create (Term::T, Str::T))},
  {0, TermPtr()}
};

extern "C"
{
  ext_t*
  create_map ()
  {
    cols [C_NOR]   = COL_NORMAL;
    cols [C_SEP]   = RED;
    cols [C_BOOL]  = GREEN;
    cols [C_CONST] = B_MAGENTA;
    cols [C_FAM]   = B_RED;
    cols [C_FUNC]  = B_GREEN;
    cols [C_INT]   = MAGENTA;
    cols [C_RET]   = COL_NORMAL;
    cols [C_SCOPE] = COL_NORMAL;
    cols [C_STR]   = COL_NORMAL;
    cols [C_SYM]   = COL_NORMAL;
    cols [C_TEMP]  = B_GREEN;
    cols [C_TOK]   = YELLOW;
    cols [C_TYPE]  = B_BLUE;
    cols [C_VAR]   = CYAN;
    cols [C_E]     = B_WHITE;
    cols [C_TERM]  = B_MAGENTA;

    return _exts;
  }
};
