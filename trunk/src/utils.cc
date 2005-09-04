#include <bool.hh>
#include <machine.hh>
#include <exception.hh>
#include <int.hh>
#include <string.hh>

#include <sstream>
#include <stdio.h>

namespace NAMESPACE
{
  TermPtr
  add_i (Machine* c, TermPtr left, TermPtr right)
  {
    return Int::create (TCAST<Int>(left)->val() + TCAST<Int>(right)->val());
  }

  TermPtr
  sub_i (Machine* c, TermPtr left, TermPtr right)
  {
    return Int::create (TCAST<Int>(left)->val() - TCAST<Int>(right)->val());
  }

  TermPtr
  mul_i (Machine* c, TermPtr left, TermPtr right)
  {
    return Int::create (TCAST<Int>(left)->val() * TCAST<Int>(right)->val());
  }

  TermPtr
  div_i (Machine* c, TermPtr left, TermPtr right)
  {
    return Int::create (TCAST<Int>(left)->val() / TCAST<Int>(right)->val());
  }

  TermPtr
  mod_i (Machine* c, TermPtr left, TermPtr right)
  {
    return Int::create (TCAST<Int>(left)->val() % TCAST<Int>(right)->val());
  }

  TermPtr
  equ_i (Machine* machine, TermPtr left, TermPtr right)
  {
    return TCAST<Int>(left)->val() == TCAST<Int>(right)->val() ? Bool::TRUE : Bool::FALSE;
  }

  TermPtr
  less_i (Machine* machine, TermPtr left, TermPtr right)
  {
    return TCAST<Int>(left)->val() < TCAST<Int>(right)->val() ? Bool::TRUE : Bool::FALSE;
  }

  TermPtr
  greater_i (Machine* machine, TermPtr left, TermPtr right)
  {
    return TCAST<Int>(left)->val() > TCAST<Int>(right)->val() ? Bool::TRUE : Bool::FALSE;
  }

  TermPtr
  concat (Machine* c, TermPtr left, TermPtr right)
  {
    string s (TCAST<Str>(left)->str());
    s += TCAST<Str>(right)->str();
    return MStr::create (s.c_str());
  }

  TermPtr
  raw_print (Machine* c, TermPtr str)
  {
    std::cout << TCAST<Str>(str)->str() << std::flush;
    return VOID;
  }

  TermPtr
  cstr (Machine* c, TermPtr src)
  {
    std::ostringstream os;
    bool escape = false;

    for (const char* s = TCAST<Str>(src)->str(); *s; ++s) {
      if (escape) {
	switch ( *s ) {
	case '\\':
	  os << '\\';
	  break;
	case 'n':
	  os << '\n';
	  break;
	case 'r':
	  os << '\r';
	  break;
	case 't':
	  os << '\t';
	  break;
	default:
	  os << *s;
	}
	escape = false;
      }
      else if ( '\\' == *s )
	escape = true;
      else
	os << *s;
    }

    return MStr::create (os.str().c_str());
  }

  TermPtr
  cast_str (Machine* machine, TermPtr term)
  {
    if (StrPtr str = CAST<Str> (term))
      return MStr::create (str->str());
    else if (CMemPtr mem = CAST<CMem> (term))
      return Str::create (static_cast<const char*> (mem->buf()));
    else
      assert (false);
  }
};
