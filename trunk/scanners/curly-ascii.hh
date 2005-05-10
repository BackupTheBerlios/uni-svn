#ifndef _CURLY_ASCII_HH_
#define _CURLY_ASCII_HH_

#include <scanner.hh>
#include <FlexLexer.h>

using namespace NAMESPACE;

class CScanner : public Scanner, private yyFlexLexer
{
protected:

  virtual int yylex ();
  virtual void xscan (std::istream& input, std::ostream& error);
};

#endif
