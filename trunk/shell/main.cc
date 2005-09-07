#include <app.hh>
#include <builtin.hh>
#include <context.hh>
#include <machine.hh>
#include <exception.hh>
#include <func.hh>
#include <handler.hh>
#include <int.hh>
#include <proj.hh>
#include <raw.hh>
#include <reflection.hh>
#include <style.hh>
#include <sym.hh>
#include <tok.hh>

#include <dlimport.hh>

#include <clo.h>
#include <color.hh>
#include <debugger.hh>
#include <importer.hh>

#include <sstream>
#include <fstream>
#include <readline/readline.h>
#include <readline/history.h>

using namespace NAMESPACE;
using namespace std;

static TermPtr _num_ctor (TermPtr arg);
static TermPtr _str_ctor (TermPtr arg);

TermPtr num_ctor_f = SimpleFunc::create (1, CONS, (void*)_num_ctor, Proj::create(Str::T,Int::T));
TermPtr str_ctor_f = SimpleFunc::create (1, CONS, (void*)_str_ctor, Proj::create(Str::T,Str::T));

TermPtr
_num_ctor (TermPtr arg)
{
  if (StrPtr s = CAST<Str> (arg))
    return Int::create (s->str());
  else {
    // \todo something is wrong, throw an exception.
    return TermPtr();
  }
}

TermPtr
_str_ctor (TermPtr arg)
{
  if (CAST<Str> (arg))
    return arg;
  else {
    // \todo something is wrong, throw an exception.
    return TermPtr();
  }
}

void
exec_files (Machine* machine,
	    const vector<string>& files,
	    MyImportHandler& importer)
{
  try {
    for (vector<string>::const_iterator f = files.begin(); f != files.end(); ++f ) {
      ifstream infile (f->c_str());
      if (infile.good())
	importer.run (machine, importer.load_scanner_for(*f), infile, ALL);
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
       const string& prompt,
       MyImportHandler& importer)
{
  while (char* cs = readline (prompt.c_str())) {
    if (*cs) {
      stringstream  input;
      input << cs;
      add_history (cs);
      free (cs);

      try {
	TermPtr result = importer.run (machine, scanner, input, ALL);
	TermPtr type   = type_of (machine, result);

	assert (result);
	assert (type);

	if (result != VOID) {
 	  bool dump = machine->step_break();
 	  machine->step_break (false);

	  TermPtr str = Tok::create ("str_ansi");
	  TermPtr app_result = App::create (str,result);
	  TermPtr app_type   = App::create (str,type);

	  result = machine->reduce_in_shield (app_result);
	  type   = machine->reduce_in_shield (app_type);

	  assert (result);
	  assert (type);

	  StrPtr str_result = CAST<Str> (result);
	  StrPtr str_type   = CAST<Str> (type);

	  cout << str_result->str() << " :: " << str_type->str() << endl;
 	  machine->step_break (dump);
	}
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
    const char* lib_path = getenv ("UNI_LIBRARY_PATH");
    if (0 == lib_path)
      throw "error: environment variable UNI_LIBRARY_PATH is not set.";

    MyImportHandler importer (lib_path);
    ShellDebugger   debugger;

    Builtin         builtin;
    Machine         machine;

    machine.debugger (&debugger);

    builtin.init (&machine);

    machine.context()->set_special ("num_ctor", "num_ctor");
    machine.context()->set_special ("str_ctor", "str_ctor");

    machine.context()->add_symbol (num_ctor_f, "num_ctor");
    machine.context()->add_symbol (str_ctor_f, "str_ctor");

    lib_register (&machine, dlimport_create_map());
    lib_import (&machine, "libuni-stringify");

    clo::parser clo_parser;
    clo_parser.parse (argc, argv);

    machine.step_break (clo_parser.get_options().dumpstack);

    if (! clo_parser.get_non_options().empty())
      exec_files (&machine, clo_parser.get_non_options(), importer);
    if (clo_parser.get_options().interactive || clo_parser.get_non_options().empty())
      shell (&machine, importer.load_scanner ("curly-ascii"), string(RED) + "] " + COL_NORMAL, importer);
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
