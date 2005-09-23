#include <builtin.hh>
#include <uni.hh>

#include <dlimport.hh>
#include <import.hh>
#include <ref.hh>

#include <clo.h>
#include <color.hh>
#include <debugger.hh>

#include <iostream>
#include <sstream>
#include <fstream>
#include <readline/readline.h>
#include <readline/history.h>

using namespace NAMESPACE;
using namespace std;

static TermPtr _num_ctor (TermPtr arg);
static TermPtr _str_ctor (TermPtr arg);

TermPtr num_ctor_f = _A (1, CONS, _num_ctor, Proj::create(Str::T,Int::T));
TermPtr str_ctor_f = _A (1, CONS, _str_ctor, Proj::create(Str::T,Str::T));

TermPtr
_num_ctor (TermPtr arg)
{
  StrPtr s = TCAST<Str> (arg);
  return Int::create (s->str());
}

TermPtr
_str_ctor (TermPtr arg)
{
  return TCAST<Str> (arg);
}

void
exec_files (Machine* machine, const vector<string>& files)
{
  try {
    for (vector<string>::const_iterator f = files.begin(); f != files.end(); ++f ) {
      ifstream infile (f->c_str());
      if (infile.good())
	uni_run (machine, uni_load_scanner_for(*f), infile, ALL);
      else
	cout << "cannot open file " << *f << endl;
    }
  }
  catch (TermPtr e) {
    cout << RED << "exception: " << COL_NORMAL;
    cout << str_ansi(machine, e)->str() << endl;
  }
  catch (const char* s) {
    cout << "[ERROR] " << s << endl;
  }
  catch (const string& s) {
    cout << "[ERROR] \"" << s << "\"" << endl;
  }
  catch (int e) {
    cout << "int exception: " << e << endl;
  }
  catch (...) {
    cout << "Unknown exception" << endl;
  }
}

void
shell (Machine* machine,
       Scanner* scanner,
       const string& prompt)
{
  while (char* cs = readline (prompt.c_str())) {
    if (*cs) {
      stringstream  input;
      input << cs;
      add_history (cs);
      free (cs);

      try {
	TermPtr result = uni_run (machine, scanner, input, ALL);
	TermPtr type   = type_of (machine, result);

	assert (result);
	assert (type);

	if (result != VOID)
	  cout << str_ansi(machine, result)->str() << " :: "
	       << str_ansi(machine, type)->str() << endl;
      }
      catch (TermPtr e) {
	cout << RED << "exception: " << COL_NORMAL;
	cout << str_ansi(machine, e)->str() << endl;
      }
      catch (const char* s) {
	cout << "[ERROR] " << s << endl;
      }
      catch (int e) {
	cout << "int exception: " << e << endl;
      }
      catch (...) {
	cout << "Unknown exception" << endl;
      }
    }
  }

  cout << endl;
}

int
main (int argc, char** argv)
{
  try {
    ShellDebugger   debugger;

    Builtin         builtin;
    Machine         machine;

    machine.debugger (&debugger);

    builtin.init (&machine);

    machine.context()->set_special ("num_ctor", "num_ctor");
    machine.context()->set_special ("str_ctor", "str_ctor");

    machine.context()->add_symbol (num_ctor_f, "num_ctor");
    machine.context()->add_symbol (str_ctor_f, "str_ctor");

    if (! lib_register (&machine, dlimport_create_map()))
      throw "cannot load library: \"dlimport\"";
    if (! lib_import (&machine, "stringify"))
      throw "cannot load library: \"dlimport\"";

    clo::parser clo_parser;
    clo_parser.parse (argc, argv);

    machine.step_break (clo_parser.get_options().dumpstack);

    if (! clo_parser.get_non_options().empty())
      exec_files (&machine, clo_parser.get_non_options());
    if (clo_parser.get_options().interactive || clo_parser.get_non_options().empty())
      shell (&machine, uni_load_scanner ("curly-ascii"), string(RED) + "] " + COL_NORMAL);
    return 0;
  }
  catch (clo::autoexcept &e) {
    switch (e.get_autothrow_id()) {
    case clo::autothrow_help:
      cout << "Usage: " << argv[0] << " [options]\n";
      cout << e.what();
      return 0;
    default:
      return 1;
    }
  }
  catch (const char* e) {
    cout << e << endl;
    return 1;
  }
  catch (...) {
    cout << "Unknown exception" << endl;
    return 1;
  }
}
