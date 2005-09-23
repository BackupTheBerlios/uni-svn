#include <uni.hh>
#include <dlimport.hh>

#include <iostream>
#include <sstream>
#include <dlfcn.h>

using namespace NAMESPACE;
using std::stringstream;

bool
lib_register (Machine *machine, const ext_t *m)
{
  for ( ; m->name; ++m)
    machine->context()->add_symbol (m->term, m->name);
  return true;
}

bool
lib_import (Machine *machine, const std::string& name)
{
  if (! machine->context()->get_mod (name)) {
    stringstream libfile;
    libfile << "libuni-" << name << ".so";
    dlerror();
    if (void* handle = dlopen (libfile.str().c_str(), RTLD_LAZY)) {
      if (0 == dlerror()) {
	ext_ctor_t ctor_map = (ext_ctor_t) dlsym (handle, "create_map");
	if (0 == dlerror()) {
	  if (ext_t *m = ctor_map()) {
	    if (lib_register (machine, m)) {
	      machine->context()->set_mod (name, NIL);
	      return true;
	    }
	  }
	}
      }
    }

    return false;
  }
  else
    std::clog << "[log:normal] library already imported: [" << name << "]" << std::endl;

  return true;
}

static TermPtr
import (Machine* machine, TermPtr t_name)
{
  if (StrPtr name = CAST<Str> (t_name))
    if (lib_import (machine, name->str()))
      return VOID;

  return TermPtr(); // call next handler on the chain of responsibility
}

static ext_t _exts[] = {
  {"import", _F (1, CTXT, import, P1 (Str::T, VOID_T), ALL_NICE)},
  {0, TermPtr()}
};

extern "C" { ext_t* create_map () { return _exts; } };

ext_t* dlimport_create_map () { return create_map(); }
