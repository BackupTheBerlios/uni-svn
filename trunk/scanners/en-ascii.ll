%{
#include <en-ascii.hh>
%}

%option c++
%option noyywrap
%option case-sensitive
%option yylineno
%option yyclass="ENScanner"

%x S
%x EOS
%x IND

DIG    [0-9]
ID     [A-Za-z\_][A-Za-z0-9\_]*
OP     [\~\!\@\#\$\%\^\&\*\-\+\=\|\:\?\,\.\/\<\>\\]
SOP    [\(\)\[\]\{\}\`\'\;]

%%

\#.*

^[^ ]+            { _indent = 0; _check_indent(); REJECT}
^[ ]+/[^ \n]      { _indent = strlen (YYText()); _check_indent(); }
^[ ]+\n

{DIG}+            { new_ctor (YYText(), "num_ctor", lineno()); }
{OP}+             { new_tok  (YYText(), lineno()); }
{SOP}             { new_tok  (YYText(), lineno()); }
{ID}              { new_tok  (YYText(), lineno()); }

\"                BEGIN(S);
<S>([^\"]|(\\.))* { new_ctor (YYText(), "str_ctor", lineno()); BEGIN(EOS); }
<S>\"             { new_ctor ("",       "str_ctor", lineno()); BEGIN(INITIAL); }
<EOS>\"           BEGIN(INITIAL);

\n                { _prev_indent = _indent; }
.|\t

%%

void
ENScanner::_check_indent()
{
  if (_prev_indent >= 0) {
    if (_indent > _prev_indent)
      group_begin (_indent);
    else if (_indent < _prev_indent)
      group_end (_indent);
  }
}

void
ENScanner::xscan (std::istream& input, std::ostream& error)
{
  _prev_indent = -1;
  _indent = 0;

  switch_streams (&input, &error);
  yylex ();
}

extern "C"
{
  void* create (int id)
  {
    return new ENScanner;
  }
};
