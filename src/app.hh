#ifndef _UNI_APP_HH_
#define _UNI_APP_HH_

#include <raw.hh>
#include <tree.hh>

namespace NAMESPACE
{
  class App : public STree<2>
  {
    DEF_TERMLET   (App, Raw);
    DEF_VISITABLE (App, app);
    TERM_TREEMAP  (rator, 0);
    TERM_TREEMAP  (rand,  1);

  public:

    void update_type();

  protected:

    App (TermPtr x, TermPtr y);
  };
};

#endif
