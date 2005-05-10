#include <bool.hh>
#include <context.hh>
#include <exception.hh>
#include <int.hh>
#include <string.hh>
#include <my_view_handler.hh>

#include <sstream>
#include <stdio.h>

namespace NAMESPACE
{
  TermPtr
  add_i (Context* c, TermPtr left, TermPtr right)
  {
    return Int::create (TCAST<Int>(left)->val() + TCAST<Int>(right)->val());
  }

  TermPtr
  sub_i (Context* c, TermPtr left, TermPtr right)
  {
    return Int::create (TCAST<Int>(left)->val() - TCAST<Int>(right)->val());
  }

  TermPtr
  mul_i (Context* c, TermPtr left, TermPtr right)
  {
    return Int::create (TCAST<Int>(left)->val() * TCAST<Int>(right)->val());
  }

  TermPtr
  div_i (Context* c, TermPtr left, TermPtr right)
  {
    return Int::create (TCAST<Int>(left)->val() / TCAST<Int>(right)->val());
  }

  TermPtr
  mod_i (Context* c, TermPtr left, TermPtr right)
  {
    return Int::create (TCAST<Int>(left)->val() % TCAST<Int>(right)->val());
  }

  TermPtr
  equ_i (Context* context, TermPtr left, TermPtr right)
  {
    return TCAST<Int>(left)->val() == TCAST<Int>(right)->val() ? Bool::TRUE : Bool::FALSE;
  }

  TermPtr
  less_i (Context* context, TermPtr left, TermPtr right)
  {
    return TCAST<Int>(left)->val() < TCAST<Int>(right)->val() ? Bool::TRUE : Bool::FALSE;
  }

  TermPtr
  greater_i (Context* context, TermPtr left, TermPtr right)
  {
    return TCAST<Int>(left)->val() > TCAST<Int>(right)->val() ? Bool::TRUE : Bool::FALSE;
  }

  TermPtr
  concat (Context* c, TermPtr left, TermPtr right)
  {
    string s (TCAST<Str>(left)->str());
    s += TCAST<Str>(right)->str();
    return MStr::create (s.c_str());
  }

  TermPtr
  raw_print (Context* c, TermPtr str)
  {
    std::cout << TCAST<Str>(str)->str() << std::flush;
    return VOID;
  }

  TermPtr
  str (Context* c, TermPtr term)
  {
    if (CAST<MStr> (term) && (Str::T == term->type()))
      return term;
    else {
      std::ostringstream os;
      MyViewHandler viewer (os);
      viewer << term;
      return MStr::create (os.str().c_str());
    }
  }

  TermPtr
  str_ansi (Context* c, TermPtr term)
  {
    if (c->ansi_attrs()) {
      std::ostringstream os;
      MyViewHandler viewer (os, *c->ansi_attrs());
      viewer << term;
      return MStr::create (os.str().c_str());
    }
    else
      return str (c, term);
  }

  TermPtr
  cstr (Context* c, TermPtr src)
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
  cast_str (Context* context, TermPtr term)
  {
    if (StrPtr str = CAST<Str> (term))
      return MStr::create (str->str());
    else if (CMemPtr mem = CAST<CMem> (term))
      return Str::create (static_cast<const char*> (mem->buf()));
    else
      assert (false);
  }
};
