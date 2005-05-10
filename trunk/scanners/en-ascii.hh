#ifndef _EN_SCANNER_HH_
#define _EN_SCANNER_HH_

#include <scanner.hh>
#include <FlexLexer.h>

using namespace NAMESPACE;

class ENScanner : public Scanner, private yyFlexLexer
{
protected:

  virtual int yylex ();
  virtual void xscan (std::istream& input, std::ostream& error);

private:

  int _prev_indent;
  int _indent;

private:

  void _check_indent();
};

#endif
