#ifndef _DLIMPORT_HH_
#define _DLIMPORT_HH_

#include <handle.hh>

#include <string>

using namespace NAMESPACE;

bool lib_register (Machine *machine, const ext_map_t* m);
bool lib_import (Machine *machine, const std::string& name);

ext_map_t* dlimport_create_map ();
void dlimport_delete_map (ext_map_t* map);

#endif
