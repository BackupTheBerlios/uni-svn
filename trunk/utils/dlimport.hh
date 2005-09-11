#ifndef _DLIMPORT_HH_
#define _DLIMPORT_HH_

#include <handle.hh>

#include <string>

using namespace NAMESPACE;

bool lib_register (Machine *machine, const ext_t* m);
bool lib_import (Machine *machine, const std::string& name);

ext_t* dlimport_create_map ();

#endif
