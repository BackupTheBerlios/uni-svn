#include <abs.hh>
#include <app.hh>
#include <context.hh>
#include <raw.hh>
#include <proj.hh>
#include <reflection.hh>

#include <list>
#include <vector>

namespace NAMESPACE
{
  App::App (TermPtr x, TermPtr y)
  {
    rator(x);
    rand(y);
  }

  void
  App::update_type ()
  {
    // here i assume type checking is already done,
    // so do not even bother what proj->from() is.
    if (ProjPtr proj = CAST<Proj>(rator()->type()))
      _type = proj->to();
    else
      _type = DEP_T;
  }
};
