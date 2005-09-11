#include <context.hh>
#include <exception.hh>
#include <machine.hh>
#include <raw.hh>
#include <scanner.hh>
#include <string.hh>
#include <handle.hh>
#include <proj.hh>

#include <dlfcn.h>
#include <fstream>
#include <map>
#include <list>
#include <string>
#include <iostream>

using namespace NAMESPACE;
using namespace std;

static TermPtr import (Machine *machine, TermPtr);

typedef std::list<std::string> pathlist_t;

static pathlist_t path;
static std::map<string,string> _ext;
static std::map<string,Scanner*> _scanners;
static std::string _default_ext;

extern "C"
{
  TermPtr
  uni_run (Machine* machine, Scanner* scanner, std::istream& input, int flags)
  {
    RawPtr raw = scanner->scan (machine, input, std::cerr);
    return machine->eval (raw, ALL);
  }

  Scanner*
  uni_load_scanner (const string& name)
  {
    typedef void* (*ctor_t)(int);

    map<string,Scanner*>::iterator iter = _scanners.find (name);
    if (_scanners.end() != iter)
      return iter->second;

    string libfile  ("libuni-" + name + ".so");

    dlerror();
    if (void* handle = dlopen (libfile.c_str(), RTLD_LAZY)) {
      if (const char *e = dlerror())
	throw e;
      ctor_t ctor = (ctor_t) dlsym (handle, "create");
      if (const char *e = dlerror())
	throw e;
      if (Scanner* result = (Scanner*) ctor(0))
	return _scanners [name] = result;
      else
	throw "cannot create scanner";
    }
    else
      throw dlerror();
  }

  Scanner*
  uni_load_scanner_for (const string& filename)
  {
    unsigned int i = filename.rfind ('.');
    if (string::npos == i || (filename.size() - i == 1))
      return uni_load_scanner (_default_ext);
    else {
      string ext (filename, i+1, filename.size() - (i+1));
      return uni_load_scanner (_ext [ext]);
    }
  }

  bool
  uni_import (Machine *machine, const string& name)
  {
    if (! machine->context()->get_mod (name)) {
      for (pathlist_t::iterator pi = path.begin(); pi != path.end(); ++pi) {
	for (map<string,string>::iterator i = _ext.begin(); i != _ext.end(); ++i) {
	  string filename (*pi + "/" + name + "." + i->first);
	  ifstream file (filename.c_str());
	  if (file.good()) {
	    Scanner *scanner = uni_load_scanner(i->second);
	    machine->context()->set_mod (name, NIL);
	    uni_run (machine, scanner, file, ALL);
	    return true;
	  }
	}
      }

      return false;
    }
    else
      return true;
  }

  ext_map_t*
  create_map ()
  {
    _default_ext = "curly-ascii";
    _ext ["uni"] = "curly-ascii";
    _ext ["en"] = "en-ascii";

    path.push_back (getenv("UNI_LIBRARY_PATH"));

    ext_map_t *map = new ext_map_t;

    (*map) ["import"] = ext_t (Envf::create ("import",
					     1, CTXT, Envf::N,
					     (void*) import,
					     P1 (Str::T, VOID_T)));

    return map;
  }

  void
  _map (ext_map_t* m)
  {
    map<string,Scanner*>::iterator i;
    for (i = _scanners.begin(); i != _scanners.end(); ++i)
      delete i->second;
    delete m;
  }
};

static TermPtr
import (Machine *machine, TermPtr t_name)
{
  StrPtr name = TCAST<Str> (t_name);

  if (uni_import (machine, name->str()))
    return VOID;
  else
    return TermPtr();
}
