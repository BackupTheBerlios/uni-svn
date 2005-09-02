#include <app.hh>
#include <builtin.hh>
#include <machine.hh>
#include <scanner.hh>
#include <tok.hh>

#include <dlfcn.h>
#include <iostream>
#include <sstream>
#include <fstream>

#define DEFAULT_OPEN_SYM   "{"
#define DEFAULT_CLOSE_SYM  "}"
#define DEFAULT_SCANNER_NAME "curly-ascii"

using namespace NAMESPACE;
using namespace std;

bool read_until (istream& input, ostream& output, string sym)
{
  string::iterator oi = sym.begin();
  stringstream buf;

  for (int x = input.get(); x != -1; x = input.get()) {
    if (*oi == x) {
      do {
	if (*(oi++) == x)
	  buf.put (x);
	else if ('\0' == *oi) {
	  input.unget();
	  return true;
	}
	else {
	  if (buf.good())
	    output << buf;
	  buf.clear();
	  oi = sym.begin();
	}
      } while (-1 != (x = input.get()));
    }
    else
      output.put (x);
  }

  return false;
}

void filt (Machine* machine,
	   Scanner* scanner,
	   istream& input,
	   ostream& output,
	   ostream& error,
	   const string&  open,
	   const string&  close)
{
  stringstream buf;

  for ( ; ; ) {
    if (read_until (input, output, open)) {
      if (input.good()) {
	if (read_until (input, buf, close)) {
	  if (buf.good()) {
 	    RawPtr raw = scanner->scan (machine, buf, error);
 	    TermPtr result = machine->eval (raw, ALL);
	    
 	    TermPtr str = Tok::create ("str");
 	    TermPtr app_result = App::create (str,result);

 	    result = machine->reduce_in_shield (app_result);

 	    StrPtr str_result = CAST<Str> (result);

 	    output << str_result->str();
	  }
	}
	else
	  error << "[ERROR] imcomplete open-close pair";
      }
      else
	error << "[ERROR] imcomplete open-close pair";
    }
    else
      break;
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

int main (int argc, char** argv)
{
  Machine machine;
  Builtin builtin;

  const char* open = 0;
  const char* close = 0;
  const char* scanner = 0;

  for (int c; (c = getopt (argc, argv, "b:e:s:")) != -1; ) {
    switch (c) {
    case 'b':
      open = optarg;
      break;
    case 'e':
      close = optarg;
      break;
    case 's':
      scanner = optarg;
      break;
    default:
      abort();
    }
  }

  if (0 == open)
    open = DEFAULT_OPEN_SYM;
  if (0 == close)
    close = DEFAULT_CLOSE_SYM;
  if (0 == scanner)
    scanner = DEFAULT_SCANNER_NAME;

  try {
    Scanner* s = load_scanner(scanner);
    builtin.init (&machine);
    filt (&machine, s, cin, cout, cerr, open, close);
  }
  catch (const char* err) {
    cerr << err << endl;
  }

  return 0;
}
