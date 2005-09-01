#include <app.hh>
#include <abs.hh>
#include <bool.hh>
#include <builtin.hh>
#include <cons.hh>
#include <context.hh>
#include <exception.hh>
#include <family.hh>
#include <func.hh>
#include <frame.hh>
#include <handler.hh>
#include <int.hh>
#include <proj.hh>
#include <raw.hh>
#include <reflection.hh>
#include <scope.hh>
#include <seq.hh>
#include <style.hh>
#include <sym.hh>
#include <temp.hh>
#include <tok.hh>

#include <clo.h>
#include <color.hh>
#include <debugger.hh>
#include <importer.hh>

#include <my_view_handler.hh>

#include <sstream>
#include <fstream>
#include <readline/readline.h>
#include <readline/history.h>

using namespace NAMESPACE;
using namespace std;

static TermPtr _num_ctor (Context*, TermPtr arg);
static TermPtr _str_ctor (Context*, TermPtr arg);

TermPtr num_ctor_f = Envf::create ("num_ctor", 1, CONS, 0, (void*)_num_ctor, Proj::create(Str::T,Int::T));
TermPtr str_ctor_f = Envf::create ("str_ctor", 1, CONS, 0, (void*)_str_ctor, Proj::create(Str::T,Str::T));

TermPtr
_num_ctor (Context* c, TermPtr arg)
{
  if (StrPtr s = CAST<Str> (arg))
    return Int::create (s->str());
  else {
    // \todo something is wrong, throw an exception.
    return TermPtr();
  }
}

TermPtr
_str_ctor (Context* c, TermPtr arg)
{
  if (CAST<Str> (arg))
    return arg;
  else {
    // \todo something is wrong, throw an exception.
    return TermPtr();
  }
}

void
exec_files (Context* context,
	    const vector<string>& files,
	    MyViewHandler& viewer,
	    MyImportHandler& importer)
{
  try {
    for (vector<string>::const_iterator f = files.begin(); f != files.end(); ++f ) {
      ifstream infile (f->c_str());
      if (infile.good())
	importer.run (context, importer.load_scanner_for(*f), infile, ALL);
      else
	cout << "cannot open file " << *f << endl;
    }
  }
  catch (TermPtr e) {
    cout << RED << "exception: " << COL_NORMAL;
    viewer << e;
    cout << endl;
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
shell (Context* context,
       Scanner* scanner,
       const string& prompt,
       MyViewHandler& viewer,
       MyImportHandler& importer)
{
  while (char* cs = readline (prompt.c_str())) {
    if (*cs) {
      stringstream  input;
      input << cs;
      add_history (cs);
      free (cs);

      try {
	TermPtr result = importer.run (context, scanner, input, ALL);
	TermPtr type   = type_of (context, result);

	assert (result);
	assert (type);

	if (result != VOID) {
 	  bool dump = context->step_break();
 	  context->step_break (false);

	  TermPtr str = Tok::create ("str_ansi");
	  TermPtr app_result = App::create (str,result);
	  TermPtr app_type   = App::create (str,type);

	  assert (str);
	  assert (app_result);
	  assert (app_type);

	  result = context->reduce_in_shield (app_result);
	  type   = context->reduce_in_shield (app_type);

	  assert (result);
	  assert (type);

	  StrPtr str_result = CAST<Str> (result);
	  StrPtr str_type   = CAST<Str> (type);

	  cout << str_result->str() << " :: " << str_type->str() << endl;
 	  context->step_break (dump);
	}
      }
      catch (TermPtr e) {
	cout << RED << "exception: " << COL_NORMAL;
	viewer << e;
	cout << endl;
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
  vector <string> cols (C_SIZE);

  cols [C_NOR]   = COL_NORMAL;
  cols [C_SEP]   = RED;
  cols [C_BOOL]  = GREEN;
  cols [C_CONST] = B_MAGENTA;
  cols [C_FAM]   = B_RED;
  cols [C_FUNC]  = B_GREEN;
  cols [C_INT]   = MAGENTA;
  cols [C_RET]   = COL_NORMAL;
  cols [C_SCOPE] = COL_NORMAL;
  cols [C_STR]   = COL_NORMAL;
  cols [C_SYM]   = COL_NORMAL;
  cols [C_TEMP]  = B_GREEN;
  cols [C_TOK]   = YELLOW;
  cols [C_TYPE]  = B_BLUE;
  cols [C_VAR]   = CYAN;
  cols [C_E]     = B_WHITE;
  cols [C_TERM]  = B_MAGENTA;

  try {
    const char* lib_path = getenv ("UNI_LIBRARY_PATH");
    if (0 == lib_path)
      throw "error: environment variable UNI_LIBRARY_PATH is not set.";

    MyImportHandler importer (lib_path);
    MyViewHandler   viewer (cout, cols);
    MyDebugHandler  debugger (viewer);

    Builtin         builtin;
    Context         context;

    context.importer (&importer);
    context.debugger (&debugger);
    context.ansi_attrs (&cols);

    builtin.init (&context);

    context.scopes()->set_special ("num_ctor", "num_ctor");
    context.scopes()->set_special ("str_ctor", "str_ctor");

    context.scopes()->add_symbol (num_ctor_f, "num_ctor");
    context.scopes()->add_symbol (str_ctor_f, "str_ctor");

    clo::parser clo_parser;
    clo_parser.parse (argc, argv);

    context.step_break (clo_parser.get_options().dumpstack);

    if (! clo_parser.get_non_options().empty())
      exec_files (&context, clo_parser.get_non_options(), viewer, importer);
    if (clo_parser.get_options().interactive || clo_parser.get_non_options().empty())
      shell (&context, importer.load_scanner ("curly-ascii"), string(RED) + "] " + COL_NORMAL, viewer, importer);
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
