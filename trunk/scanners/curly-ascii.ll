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

{DIG}+            { new_ctor (YYText(), "num_ctor", lineno()); }
{OP}+             { new_tok  (YYText(), lineno()); }
{SOP}             { new_tok  (YYText(), lineno()); }
{ID}              { new_tok  (YYText(), lineno()); }

\"                BEGIN(S);
<S>([^\"]|(\\.))* { new_ctor (YYText(), "str_ctor", lineno()); BEGIN(EOS); }
<S>\"             { new_ctor ("",       "str_ctor", lineno()); BEGIN(INITIAL); }
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
