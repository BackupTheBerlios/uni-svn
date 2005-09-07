#ifndef _SHELL_DEBUGGER_HH_
#define _SHELL_DEBUGGER_HH_

#include <typedef.hh>

using namespace NAMESPACE;

StrPtr str_ansi (Machine *machine, TermPtr term);

class ShellDebugger : public DebugHandler
{
private:

  bool _first_step;

public:

  virtual void step_break (Machine* machine, TermPtr term);
};

#endif
