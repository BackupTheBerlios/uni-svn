#include <app.hh>
#include <exception.hh>
#include <machine.hh>
#include <style.hh>
#include <tok.hh>

#include <color.hh>
#include <debugger.hh>

#include <iostream>

using namespace std;

StrPtr
str_ansi (Machine *machine, TermPtr term)
{
  bool step = machine->step_break();

  machine->step_break (false);

  TermPtr str = Tok::create ("str_ansi");
  TermPtr app = App::create (str,term);
  TermPtr result = machine->reduce_in_shield (app);

  machine->step_break (step);

  return TCAST<Str> (result);
}

void
ShellDebugger::step_break (Machine* machine, TermPtr term)
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


  cout << str_ansi(machine, term)->str();

  for (unsigned int i = 0; i < machine->arg_count(); ++i) {
    cout << BLUE << " | ";
    cout << str_ansi(machine, machine->arg(i))->str();
  }

  cout << COL_NORMAL << '\n';
}
