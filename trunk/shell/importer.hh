#ifndef _SHELL_IMPORTER_HH_
#define _SHELL_IMPORTER_HH_

#include <scanner.hh>
#include <typedef.hh>

#include <string>
#include <fstream>

using namespace NAMESPACE;
using namespace std;

class MyImportHandler
{
public:

  virtual bool import (Machine* machine, const string& name);

  Scanner* load_scanner (const string& extname);
  Scanner* load_scanner_for (const string& filename);

  TermPtr run (Machine* machine, Scanner* scanner, std::istream& input, int flags);

  MyImportHandler (const string& lib_path);
  virtual ~MyImportHandler();

private:

  map <string, Scanner*> _scanners;
  map <string, string>   _ext;
  string _default_ext;
  string _library_path;
};

#endif
