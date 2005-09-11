#ifndef _UNI_IMPORT_HH_
#define _UNI_IMPORT_HH_

#include <machine.hh>
#include <scanner.hh>

#include <fstream>

using namespace NAMESPACE;

extern "C"
{
  TermPtr uni_run (Machine* machine, Scanner* scanner, std::istream& input, int flags);
  Scanner* uni_load_scanner (const std::string& name);
  Scanner* uni_load_scanner_for (const std::string& filename);
  bool uni_import (Machine *machine, const std::string& name);
};

#endif
