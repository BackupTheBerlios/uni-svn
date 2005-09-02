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

  virtual void step_break (Machine* machine, TermPtr term)
  {
    if (machine->current_flags() == ALL)
      cout << 'S';
    else if (machine->current_flags() == ALL_NICE)
      cout << 'N';
    else if (machine->current_flags() == ALL_CTXT)
      cout << 'C';
    else if (machine->current_flags() == ALL_PURE)
      cout << 'P';
    else if (machine->current_flags() == ALL_META)
      cout << 'M';
    else if (machine->current_flags() == BIND)
      cout << 'B';
    else if (machine->current_flags() == 0)
      cout << ' ';
    else
      cout << '?';

    for (unsigned int i = 1; i <= machine->frame_depth(); ++i)
      cout << BLUE << "| ";

    _view << term;

    for (unsigned int i = 0; i < machine->arg_count(); ++i) {
      cout << BLUE << " | ";
      _view << machine->arg(i);
    }

    cout << COL_NORMAL << '\n';
  }
};

#endif
