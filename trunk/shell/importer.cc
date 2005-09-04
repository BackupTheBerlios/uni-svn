#include <machine.hh>
#include <exception.hh>
#include <importer.hh>
#include <context.hh>
#include <string.hh>
#include <raw.hh>

#include <dlfcn.h>
#include <fstream>
#include <map>
#include <iostream>

typedef std::map<string,TermPtr> ext_map_t;
typedef void* (ctor_map_t)();

bool
MyImportHandler::import (Machine* machine, const string& name)
{
  if (! machine->context()->get_mod (name)) {
    // \todo iterate through library path list as well
    for (map<string,string>::iterator i = _ext.begin(); i != _ext.end(); ++i) {
      string filename (_library_path + "/" + name + "." + i->first); // \todo directly use + operator on string might be very slow.
      ifstream file (filename.c_str());
      if (file.good()) {
	Scanner *scanner = load_scanner(i->second);
	machine->context()->set_mod (name, NIL);
	run (machine, scanner, file, ALL);
	return true;
      }
    }

    if (0 == name.find ("libuni-")) {
      string libfile (name + ".so");
      dlerror();
      if (void* handle = dlopen (libfile.c_str(), RTLD_LAZY)) {
	if (const char *e = dlerror())
	  throw e;
	ctor_map_t* ctor_map = (ctor_map_t*) dlsym (handle, "create_map");
	if (const char *e = dlerror())
	  throw e;
	if (ext_map_t *fs = (ext_map_t*)ctor_map()) {
	  for (ext_map_t::iterator iter = fs->begin(); iter != fs->end(); ++iter)
	    machine->context()->add_symbol (iter->second, iter->first);
	  return true;
	}
	else
	  throw "cannot load library";
      }
    }

    throw E (E_NO_LIB, MStr::create (name));
  }
  else
    return true;
}

TermPtr
MyImportHandler::run (Machine* machine, Scanner* scanner, std::istream& input, int flags)
{
  RawPtr raw = scanner->scan (machine, input, std::cerr);
  return machine->eval (raw, ALL);
}

Scanner*
MyImportHandler::load_scanner (const string& name)
{
  typedef void* (*ctor_t)(int);

  map<string,Scanner*>::iterator iter = _scanners.find (name);
  if (_scanners.end() != iter)
    return iter->second;

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
      return _scanners [name] = result;
    else
      throw "cannot create scanner";
  }
  else
    throw dlerror();
}

Scanner*
MyImportHandler::load_scanner_for (const string& filename)
{
  unsigned int i = filename.rfind ('.');
  if (string::npos == i || (filename.size() - i == 1))
    return load_scanner (_default_ext);
  else {
    string ext (filename, i+1, filename.size() - (i+1));
    return load_scanner (_ext [ext]);
  }
}

MyImportHandler::MyImportHandler (const string& lib_path)
  : _library_path (lib_path)
{
  _default_ext = "curly-ascii";
  _ext ["uni"] = "curly-ascii";
  _ext ["en"] = "en-ascii";
}

MyImportHandler::~MyImportHandler ()
{
  map<string,Scanner*>::iterator i;
  for (i = _scanners.begin(); i != _scanners.end(); ++i)
    delete i->second;
}
