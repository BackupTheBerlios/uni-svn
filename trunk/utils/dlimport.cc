#include <dlimport.hh>

#include <context.hh>
#include <func.hh>
#include <machine.hh>
#include <proj.hh>
#include <string.hh>
#include <style.hh>

#include <fstream>
#include <map>

#include <dlfcn.h>

using namespace NAMESPACE;

bool
lib_register (Machine *machine, const ext_map_t* m)
{
  for (ext_map_t::const_iterator i = m->begin(); i != m->end(); ++i)
    machine->context()->add_symbol (i->second.term, i->first);
  return true;
}

bool
lib_import (Machine *machine, const std::string& name)
{
  if (! machine->context()->get_mod (name)) {
    if (0 == name.find ("libuni-")) {
      string libfile (name + ".so");
      dlerror();
      if (void* handle = dlopen (libfile.c_str(), RTLD_LAZY)) {
	if (0 == dlerror()) {
	  ext_ctor_t ctor_map = (ext_ctor_t) dlsym (handle, "create_map");
	  if (0 == dlerror()) {
	    if (ext_map_t *m = (ext_map_t*)ctor_map())
	      return lib_register (machine, m);
	  }
	}
      }
    }

    return false;
  }

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

extern "C"
{
  ext_map_t*
  create_map ()
  {
    ext_map_t *map = new ext_map_t;

    (*map) ["import"] = ext_t (1, Envf::create ("import",
						1, CTXT, Envf::N,
						(void*) import,
						P1 (Str::T, VOID_T)));

    return map;
  }

  void
  destroy_map (ext_map_t* map)
  {
    delete map;
  }
};

ext_map_t* dlimport_create_map () { return create_map(); }
void dlimport_delete_map (ext_map_t* map) { destroy_map (map); }
