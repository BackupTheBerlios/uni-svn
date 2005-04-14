#include <context.hh>
#include <exception.hh>
#include <importer.hh>
#include <scope.hh>
#include <string.hh>

#include <dlfcn.h>
#include <fstream>

bool
MyImportHandler::import (Context* context, const string& name)
{
  if (context->scopes()->get_mod (name))
    return true;

  map<string,string>::iterator i;
  for (i = _ext.begin(); i != _ext.end(); ++i) {
    string filename (_library_path + "/" + name + "." + i->first);
    ifstream file (filename.c_str());
    if (file.good()) {
      context->scopes()->set_mod (name, NIL);
      context->eval (load_scanner (i->second), file, ALL);
      return true;
    }
  }

  throw E (E_NO_LIB, MStr::create (name));
}

Scanner*
MyImportHandler::load_scanner (const string& name)
{
  typedef void* (*ctor_t)(int);

  map<string,Scanner*>::iterator iter = _scanners.find (name);
  if (_scanners.end() != iter)
    return iter->second;

  string libfile  ("/libvs-" + name + ".so");
  string fullname (_scanner_path + libfile);

  dlerror();
  if (void* handle = dlopen (fullname.c_str(), RTLD_LAZY)) {
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

MyImportHandler::MyImportHandler (const string& scanner_path, const string& lib_path)
  : _scanner_path (scanner_path),
    _library_path (lib_path)
{
  _default_ext = "curly-ascii";
  _ext ["vs"] = "curly-ascii";
  _ext ["en"] = "en-ascii";
}

MyImportHandler::~MyImportHandler ()
{
  map<string,Scanner*>::iterator i;
  for (i = _scanners.begin(); i != _scanners.end(); ++i)
    delete i->second;
}
