#include <uni.hh>

#include <dlfcn.h>
#include <fstream>
#include <map>
#include <list>
#include <string>
#include <iostream>

using namespace NAMESPACE;

typedef std::list <std::string> pathlist_t;
typedef std::map <std::string, std::string> fileext_t;

static pathlist_t path;
static fileext_t _ext;
static std::map<string,Scanner*> _scanners;
static std::string _default_ext;

static TermPtr import (Machine *machine, TermPtr);

static ext_t _exts[] = {
  {"import", Envf::create (1, CTXT, (void*) import, P1 (Str::T, VOID_T), 0, ALL_NICE)},
  {0, TermPtr()}
};

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
      map<string,string>::iterator ei = _ext.find (ext);
      if (_ext.end() == ei)
	throw ext;
      else
	return uni_load_scanner (_ext [ext]);
    }
  }

  bool
  uni_import (Machine *machine, const string& name)
  {
    if (! machine->context()->get_mod (name)) {
      for (pathlist_t::iterator pi = path.begin(); pi != path.end(); ++pi) {
	for (fileext_t::iterator i = _ext.begin(); i != _ext.end(); ++i) {
	  std::string filename (*pi + "/" + name + "." + i->first);
	  std::ifstream file (filename.c_str());
	  if (file.good()) {
	    Scanner *scanner = uni_load_scanner(i->second);
	    uni_run (machine, scanner, file, ALL);
	    machine->context()->set_mod (name, NIL);
	    return true;
	  }
	}
      }

      return false;
    }
    else {
      std::clog << "[log:normal] library already imported: [" << name << "]" << std::endl;
      return true;
    }
  }

  ext_t*
  create_map ()
  {
    _default_ext = "curly-ascii";
    _ext ["uni"] = "curly-ascii";
    _ext ["en"] = "en-ascii";

    path.push_back (getenv("UNI_LIBRARY_PATH"));

    return _exts;
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
