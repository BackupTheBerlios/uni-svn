#include <app.hh>
#include <frame.hh>

namespace NAMESPACE
{
  void
  Frame::push (AppPtr app)
  {
    push_back (AppType (app, app->rand()->type()));
  }

  void
  Frame::pop (unsigned int n)
  {
    while (n-- && size())
      pop_back();
  }
};
