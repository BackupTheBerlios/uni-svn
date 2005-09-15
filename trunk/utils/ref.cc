#include <uni.hh>

using namespace NAMESPACE;

#define EFLAG(F,I) (Envf::F << (I * Envf::BITS))

#define Z0 EFLAG(Z,0)
#define Z1 EFLAG(Z,1)
#define Z2 EFLAG(Z,2)
#define Z3 EFLAG(Z,3)

#define S0 EFLAG(S,0)
#define S1 EFLAG(S,1)
#define S2 EFLAG(S,2)
#define S3 EFLAG(S,3)
#define S4 EFLAG(S,4)

#define M0 EFLAG(M,0)
#define M1 EFLAG(M,1)
#define M2 EFLAG(M,2)
#define M3 EFLAG(M,3)

#define B0 EFLAG(B,0)
#define B1 EFLAG(B,1)
#define B2 EFLAG(B,2)
#define B3 EFLAG(B,3)

#define SX (S0|S1|S2|S3|S4)

static TermPtr
defstyle (Machine *m, TermPtr n, TermPtr f, TermPtr p)
{
  m->context()->add_style (TCAST<Str>(n)->str(),
			   MAKE_STYLE(TCAST<Int>(f)->val(),TCAST<Int>(p)->val()));
  return VOID;
}

static TermPtr
defgroup (Machine *m, TermPtr t_lhs, TermPtr t_rhs)
{
  m->context()->add_outfix (TCAST<Str>(t_lhs)->str(),
			    TCAST<Str>(t_rhs)->str());
  return VOID;
}

static TermPtr
defmap (Machine *m, TermPtr n, TermPtr term)
{
  m->context()->add_symbol (term, TCAST<Str>(n)->str());
  return VOID;
}

static TermPtr
define (Machine *m, TermPtr name, TermPtr type, TermPtr body)
{
  m->context()->add_symbol (Intf::create (body, type),
			    TCAST<Str>(name)->str());
  return VOID;
}

static TermPtr
undef (Machine *m, TermPtr name)
{
  m->context()->del_symbol (TCAST<Str>(name)->str());
  return VOID;
}

static TermPtr
undefx (Machine *m, TermPtr t_nspace, TermPtr t_name)
{
  SpacePtr nspace = TCAST<Space> (t_nspace);
  StrPtr   name   = TCAST<Str>   (t_name);
  m->context()->del_symbol (name->str(), nspace);
  return VOID;
}

static TermPtr
redef (Machine* machine, TermPtr t_name, TermPtr term)
{
  const char* name = TCAST<Str>(t_name)->str();
  if (! machine->context()->set_symbol (name, term))
    machine->context()->add_symbol (term, name);
  return VOID;
}

//// lambda expressions //////////////////////////////////////////////////////
static TermPtr
lambda (Machine *m, TermPtr var, TermPtr body)
{
  return Abs::create (var, body);
}

static TermPtr
lambda_t (Machine *m, TermPtr t_var, TermPtr body)
{
  const char* var = TCAST<Str>(t_var)->str();
  return Abs::create (Tok::create (var), body);
}

static TermPtr
lambda_s (Machine *m, TermPtr t_var, TermPtr body)
{
  const char* var = TCAST<Str>(t_var)->str();
  AbsPtr abs = Abs::create (Sym::create (var), body);
  return Abs::create (Tok::create (var), abs->body());
}

static TermPtr
solve (Machine *m, TermPtr var, TermPtr lhs, TermPtr rhs)
{
  return lhs->solve (var, rhs);
}

//// simple functions ////////////////////////////////////////////////////////
static TermPtr
proj (Machine *m, TermPtr left, TermPtr right)
{
  return Proj::create (left, right);
}

static TermPtr
type (TermPtr name)
{
  return Type::create ((TCAST<Str>(name))->str());
}

static TermPtr
typex (TermPtr body, TermPtr param)
{
  return TypeTemplate::create (body, param);
}

static TermPtr
is_sub (TermPtr sub, TermPtr super)
{
  return sub->compat (super) ? Bool::TRUE : Bool::FALSE;
}


//// C functions ///////////////////////////////////////////////////////////
extern "C"
{
  TermPtr
  type_of (Machine *m, TermPtr term)
  {
    // For raw terms, they have to be bound before
    // any meaningful type can be extracted from it.
    // For terms whose types are DEP, the have to be
    // evaluated first, however, no side-effects are
    // wanted here, since typeof is a pure function.
    // Terms with unpredictable type (UPR) will not
    // be reduced, and UPR will be returned.

    TermPtr result = term->type();

    if (Raw::T == result) {
      term = m->reduce (term, BIND);
      if (Raw::T == term->type())
	return Raw::T;
      else
	return type_of (m, term);
    }
    else if (DEP_T == result) {
      term = m->reduce (term, ALL_CTXT);
      if (DEP_T == term->type())
	return Raw::T;
      else
	return type_of(m, term);
    }
    else
      return result;
  }
};

//// export functions //////////////////////////////////////////////////////
static ext_t _exts[] = {
  {"defsty", Envf::create (3, CTXT, S0+S1+S2, (void*) defstyle, P3 (Str::T, Int::T, Int::T, VOID_T))},
  {"defgrp", Envf::create (2, CTXT, S0+S1,    (void*) defgroup, P2 (Str::T, Str::T, VOID_T))},
  {"defmap", Envf::create (2, CTXT, S0+S1,    (void*) defmap,   P2 (Str::T, Term::T, VOID_T))},
  {"defvar", Envf::create (2, CTXT, S0+M1,    (void*) defmap,   P2 (Str::T, Term::T, VOID_T))},
  {"define", Envf::create (3, CTXT, S0+S1+M2, (void*) define,   P3 (Str::T, Term::T, Raw::T, VOID_T))},
  {"undef",  Envf::create (1, CTXT, S0,       (void*) undef,    P1 (Str::T, VOID_T))},
  {"undef",  Envf::create (2, CTXT, S0+S1,    (void*) undefx,   P2 (Space::T, Str::T, VOID_T))},
  {"redef",  Envf::create (2, CTXT, S0+M1,    (void*) redef,    P2 (Str::T, Raw::T, VOID_T))},

  {"typeof", Envf::create (1, PURE, 0,        (void*) type_of,  P1 (Term::T, Type::T))},

  {"proj",   Envf::create (2, META, M0+M1,    (void*) proj,     P2 (Term::T, Term::T, Type::T))},

  {"type",   SimpleFunc::create (1, PURE, (void*) type,     P1 (Str::T, Type::T))},
  {"type",   SimpleFunc::create (2, PURE, (void*) typex,    P2 (Term::T, Term::T, Type::T))},
  {"is_sub", SimpleFunc::create (2, META, (void*) is_sub,    P2 (Type::T, Type::T, Bool::T))},

  {"solve",  Envf::create (3, PURE, S0+M1+M2, (void*) solve,    P3 (Term::T, Raw::T, Raw::T, DEP_T))},

  {"lambda", Envf::create (2, META, Z0+Z1,    (void*) lambda,   P2 (Raw::T, Raw::T, DEP_T))},
  {"lambdat",Envf::create (2, META, S0+Z1,    (void*) lambda_t, P2 (Sym::T, Raw::T, DEP_T))},
  {"lambdas",Envf::create (2, META, S0+B1,    (void*) lambda_s, P2 (Str::T, Raw::T, DEP_T))},

  {0, TermPtr()}
};

extern "C" {
  ext_t* create_map () { return _exts; }
  ext_t* ref_create_map () { return _exts; }
};
