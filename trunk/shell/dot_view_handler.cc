#include <abs.hh>
#include <app.hh>
#include <proj.hh>
#include <string.hh>
#include <tok.hh>
#include <sym.hh>
#include <int.hh>
#include <cons.hh>
#include <app.hh>
#include <family.hh>
#include <seq.hh>
#include <func.hh>
#include <bool.hh>
#include <temp.hh>
#include <func.hh>

#include <dot_view_handler.hh>

namespace NAMESPACE
{
  void DotViewHandler::visit_abs (AbsPtr ptr)
  {
    *this << (int)ptr.get() << " [label = \"\\\"]\n"
	  << ptr->bv() << ptr->body()
	  << (int)ptr.get() << " -> " << (int)ptr->body().get() << "\n"
	  << (int)ptr.get() << " -> " << (int)ptr->bv().get() << "\n";
  }

  void DotViewHandler::visit_app (AppPtr ptr)
  {
    *this << (int)ptr.get() << " [label = \"@\"]\n"
	  << ptr->rator() << ptr->rand()
	  << (int)ptr.get() << " -> " << (int)ptr->rator().get() << "\n"
	  << (int)ptr.get() << " -> " << (int)ptr->rand().get() << "\n";
  }

  void DotViewHandler::visit_int   (IntPtr   ptr)
  {
    *this << (int)ptr.get() << " [label = \"" << ptr->val() << "\"]\n";
  }

  void DotViewHandler::visit_raw   (RawPtr   ptr)
  {
    *this << (int)ptr.get() << " [label = \"...\"]\n";
  }

  void DotViewHandler::visit_sym (SymPtr ptr)
  {
    *this << (int)ptr.get() << " [label = \"" << ptr->str() << "\"]\n";
  }

  void DotViewHandler::visit_tok   (TokPtr   ptr)
  {
    *this << (int)ptr.get() << " [label = \"" << ptr->str() << "\"]\n";
  }

  void DotViewHandler::visit_term  (TermPtr ptr)
  {
    *this << (int)ptr.get() << "[label = \"?\"]\n";
  }
};
