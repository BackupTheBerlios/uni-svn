%{
#include <curly-ascii.hh>
%}

%option c++
%option noyywrap
%option case-sensitive
%option yylineno
%option yyclass="CScanner"

%x S
%x EOS

DIG    [0-9]
ID     [A-Za-z\_][A-Za-z0-9\_]*
OP     [\~\!\@\#\$\%\^\&\*\-\+\=\|\:\?\,\.\/\<\>\\]
SOP    [\(\)\[\]\{\}\`\'\;]

%%

\#.*

{DIG}+            { int_ctor (YYText(), lineno()); }
{OP}+             { sym_ctor (YYText(), lineno()); }
{SOP}             { sym_ctor (YYText(), lineno()); }
{ID}              { sym_ctor (YYText(), lineno()); }

\"                BEGIN(S);
<S>([^\"]|(\\.))* { str_ctor (YYText(), lineno()); BEGIN(EOS); }
<S>\"             { str_ctor ("",       lineno()); BEGIN(INITIAL); }
<EOS>\"           BEGIN(INITIAL);

.|\t|\n

%%

void
CScanner::xscan (std::istream& input, std::ostream& error)
{
  switch_streams (&input, &error);
  yylex ();
}

extern "C"
{
  void* create (int id)
  {
    return new CScanner;
  }
};
