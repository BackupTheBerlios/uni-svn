#include <uni.hh>

using namespace NAMESPACE;

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
tok_str (TermPtr t_name)
{
  return Tok::create (TCAST<Str>(t_name)->str());
}

static TermPtr
tok_sym (TermPtr t_name)
{
  return Tok::create (TCAST<Sym>(t_name)->str());
}

static TermPtr
is_sub (TermPtr sub, TermPtr super)
{
  return sub->compat (super) ? Bool::TRUE : Bool::FALSE;
}

//// Namespace functions ///////////////////////////////////////////////////////
static TermPtr
new_ns (Machine* machine)
{
  return Space::create();
}

static TermPtr
get_ns (Machine* machine, TermPtr t_ns, TermPtr t_name)
{
  SpacePtr nspace = TCAST<Space> (t_ns);
  StrPtr   name   = TCAST<Str> (t_name);

  if (TermPtr result = machine->context()->get_symbol (name->str(), nspace))
    return result;
  else
    return NIL;
}

static TermPtr
set_ns (Machine* machine, TermPtr t_ns)
{
  if (NIL == t_ns)
    machine->context()->current_nspace (SpacePtr());
  else {
    SpacePtr nspace = TCAST<Space> (t_ns);
    machine->context()->current_nspace (nspace);
  }

  return VOID;
}

//// Scope functions ///////////////////////////////////////////////////////
static TermPtr
scope_push (Machine *m)
{
  m->context()->push();
  return VOID;
}

static TermPtr
scope_pushx (Machine *m, TermPtr t_scope)
{
  ScopePtr scope = TCAST <Scope> (t_scope);
  m->context()->push (scope);
  return VOID;
}

static TermPtr
scope_pop (Machine *m)
{
  m->context()->pop();
  return VOID;
}

static TermPtr
scope_popx (Machine *m)
{
  ScopePtr result = m->context()->top();
  m->context()->pop();
  return result;
}

static TermPtr
scope_set (Machine *m, TermPtr t_flags)
{
  int flags = TCAST<Int>(t_flags)->val();
  m->context()->current_flags (flags);
  return VOID;
}

static TermPtr
get_special (Machine *m, TermPtr n)
{
  const char* name = TCAST<Str>(n)->str();
  return MStr::create (m->context()->get_special(name).c_str());
}

static TermPtr
set_special (Machine *m, TermPtr n, TermPtr v)
{
  const char* name  = TCAST<Str>(n)->str();
  const char* value = TCAST<Str>(v)->str();
  m->context()->set_special (name, value);
  return VOID;
}

//// evaluation functions //////////////////////////////////////////////////
static TermPtr
bind (Machine *m, TermPtr term)
{
  return m->reduce (term, BIND);
}

static TermPtr
redsh (Machine *m, TermPtr term)
{
  TermPtr r = m->reduce_in_shield (term);
  return r;
}

static TermPtr
redshx (Machine *m, TermPtr term, TermPtr exit)
{
  TermPtr r = m->reduce_in_shield (term, ALL, exit);
  return r;
}

//// return value functions ////////////////////////////////////////////////
static TermPtr
retval (TermPtr term)
{
  return Ret::create (term);
}

static TermPtr
retpop (Machine *m, TermPtr term)
{
  m->context()->pop();
  return Ret::create (term);
}

static TermPtr
raise (TermPtr t_id, TermPtr t1, TermPtr t2, TermPtr t3)
{
  int id = TCAST<Int>(t_id)->val();
  throw E (id, t1, t2, t3);
}

static TermPtr
subs (TermPtr from, TermPtr to, TermPtr term)
{
  return term->sub (from, to);
}

static TermPtr
as (TermPtr type, TermPtr term)
{
  return AsType::create (type, term);
}

static TermPtr
sa (TermPtr term)
{
  shared_ptr<AsType> as = TCAST<AsType> (term);
  return as->body();
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
  {"defsty", _F (3, CTXT, defstyle, P3 (Str::T, Int::T, Int::T, VOID_T),  ALL, ALL, ALL)},
  {"defgrp", _F (2, CTXT, defgroup, P2 (Str::T, Str::T, VOID_T),          ALL, ALL)},
  {"defmap", _F (2, CTXT, defmap,   P2 (Str::T, Term::T, VOID_T),         ALL, ALL)},
  {"defvar", _F (2, CTXT, defmap,   P2 (Str::T, Term::T, VOID_T),         ALL, ALL_META)},
  {"define", _F (3, CTXT, define,   P3 (Str::T, Term::T, Raw::T, VOID_T), ALL, ALL, ALL_META)},
  {"undef",  _F (1, CTXT, undef,    P1 (Str::T, VOID_T),                  ALL)},
  {"undef",  _F (2, CTXT, undefx,   P2 (Space::T, Str::T, VOID_T),        ALL, ALL_META)},
  {"redef",  _F (2, CTXT, redef,    P2 (Str::T, Raw::T, VOID_T),          ALL, ALL_META)},

  {"typeof", _F (1, PURE, type_of,  P1 (Term::T, Type::T),          CONS)},
  {"proj",   _F (2, META, proj,     P2 (Term::T, Term::T, Type::T), ALL_META, ALL_META)},

  {"type",   _S (1, PURE, type,    P1 (Str::T, Type::T),           CONS)},
  {"type",   _S (2, PURE, typex,   P2 (Term::T, Term::T, Type::T), CONS, CONS)},
  {"is_sub", _S (2, META, is_sub,  P2 (Type::T, Type::T, Bool::T), CONS, CONS)},

  {"tok",    _S (1, BIND, tok_str, P1 (Str::T, Raw::T), CONS)},
  {"tok",    _S (1, BIND, tok_sym, P1 (Sym::T, Raw::T), CONS)},

  {"subs",   _S (3, PURE, subs,    P3 (Term::T, Term::T, Term::T, Term::T), ALL, ALL, ALL_META)},
  {"as",     _S (2, PURE, as,      P2 (Type::T, Term::T, DEP_T), ALL, ALL_META)},
  {"sa",     _S (1, PURE, sa,      P1 (Term::T, DEP_T), ALL)},
  {"throw",  _S (4, CTXT, raise,   P4 (Int::T, Term::T, Term::T, Term::T, VOID_T), ALL, ALL, ALL, ALL)},

  {"return", _S (1, CTXT, retval, P1 (Term::T, VOID_T), ALL)},
  {"retpop", _F (1, CTXT, retpop, P1 (Term::T, VOID_T), ALL)},

  {"solve",  _F (3, PURE, solve,    P3 (Term::T, Raw::T, Raw::T, DEP_T), ALL, ALL_META, ALL_META)},

  {"lambda", _F (2, META, lambda,   P2 (Raw::T, Raw::T, DEP_T), CONS)},
  {"lambdat",_F (2, META, lambda_t, P2 (Sym::T, Raw::T, DEP_T), ALL, CONS)},
  {"lambdas",_F (2, META, lambda_s, P2 (Str::T, Raw::T, DEP_T), ALL, BIND)},

  //// namespace manipulation functions ////
  {"new_ns", _F (0, PURE, new_ns, Space::T)},
  {"set_ns", _F (1, CTXT, set_ns, P1 (Term::T, VOID_T),           ALL)},
  {"get_ns", _F (2, CTXT, get_ns, P2 (Space::T, Str::T, Term::T), ALL, ALL)},

  //// special name functions ////
  {"name_get", _F (1, CTXT, get_special, P1 (Str::T, Str::T),         ALL)},
  {"name_set", _F (2, CTXT, set_special, P2 (Str::T, Str::T, VOID_T), ALL, ALL)},

  //// evaluation functions ////
  {"bind",   _F (1, BIND, bind,   P1 (Raw::T, DEP_T),         CONS)},
  {"redsh",  _F (1, SIDE, redsh,  P1 (Raw::T, UPR_T),         CONS)},
  {"redshx", _F (2, SIDE, redshx, P2 (Raw::T, Raw::T, UPR_T), CONS, CONS)},

  //// scope manipulation functions ////
  {"scope_pop",   _F (0, CTXT, scope_pop,   VOID_T)},
  {"scope_popx",  _F (0, CTXT, scope_popx,  Scope::T)},
  {"scope_push",  _F (0, CTXT, scope_push,  VOID_T)},
  {"scope_pushx", _F (1, CTXT, scope_pushx, P1 (Scope::T, VOID_T), ALL)},
  {"scope_set",   _F (1, CTXT, scope_set,   P1 (Int::T, VOID_T),   ALL)},

  {0, TermPtr()}
};

extern "C" {
  ext_t* create_map () { return _exts; }
  ext_t* ref_create_map () { return _exts; }
};
