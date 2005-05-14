#ifndef _SHELL_DEBUGGER_HH_
#define _SHELL_DEBUGGER_HH_

#include <handler.hh>

#include <string>
#include <iostream>

using namespace NAMESPACE;
using namespace std;

class MyDebugHandler : public DebugHandler
{
private:

  bool _first_step;
  ViewHandler& _view;

public:

  MyDebugHandler (ViewHandler& view) : _view (view) { }

  virtual void step_break (Context* context, TermPtr term)
  {
    if (context->current_flags() == ALL)
      cout << 'S';
    else if (context->current_flags() == ALL_NICE)
      cout << 'N';
    else if (context->current_flags() == ALL_CTXT)
      cout << 'C';
    else if (context->current_flags() == ALL_PURE)
      cout << 'P';
    else if (context->current_flags() == ALL_META)
      cout << 'M';
    else if (context->current_flags() == BIND)
      cout << 'B';
    else if (context->current_flags() == 0)
      cout << ' ';
    else
      cout << '?';

    for (unsigned int i = 1; i <= context->frame_depth(); ++i)
      cout << BLUE << "| ";

    _view << term;

    for (unsigned int i = 0; i < context->arg_count(); ++i) {
      cout << BLUE << " | ";
      _view << context->arg(i);
    }

    cout << COL_NORMAL << '\n';
  }
};

#endif
