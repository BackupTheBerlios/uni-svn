#include <app.hh>
#include <builtin.hh>
#include <context.hh>
#include <scanner.hh>
#include <tok.hh>

#include <dlfcn.h>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace NAMESPACE;
using namespace std;

void filt (Context* context, Scanner* scanner, istream& input, ostream& output, ostream& error)
{
  int x;

  while (-1 != (x = input.get())) {
    if ('{' != x)
      output << static_cast<char>(x);
    else {
      stringstream ss;
      while (input.good()) {
	char y = input.get();
	if ('}' == y)
	  break;
	ss.put (y);
      }
      if (ss.good()) {
	RawPtr raw = scanner->scan (context, ss, error);
	TermPtr result = context->eval (raw, ALL);

	TermPtr str = Tok::create ("str");
	TermPtr app_result = App::create (str,result);

	result = context->reduce_in_shield (app_result);

	StrPtr str_result = CAST<Str> (result);

	output << str_result->str();
      }
    }
  }
}

Scanner* load_scanner (const string& name)
{
  typedef void* (*ctor_t)(int);

  // \todo   operator + on string might be slow, use sstream instead
  string libfile  ("libuni-" + name + ".so");

  dlerror();
  if (void* handle = dlopen (libfile.c_str(), RTLD_LAZY)) {
    if (const char *e = dlerror())
      throw e;
    ctor_t ctor = (ctor_t) dlsym (handle, "create");
    if (const char *e = dlerror())
      throw e;
    if (Scanner* result = (Scanner*) ctor(0))
      return result;
    else
      throw "cannot create scanner";
  }
  else
    throw dlerror();
}

int main (int argc, int argv)
{
  Context context;
  Builtin builtin;

  try {
    Scanner* s = load_scanner("curly-ascii");
    builtin.init (&context);
    filt (&context, s, cin, cout, cerr);
  }
  catch (const char* err) {
    cerr << err << endl;
  }

  return 0;
}
