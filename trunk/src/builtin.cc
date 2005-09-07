#include <bool.hh>
#include <builtin.hh>
#include <cons.hh>
#include <machine.hh>
#include <exception.hh>
#include <func.hh>
#include <int.hh>
#include <opt.hh>
#include <proj.hh>
#include <raw.hh>
#include <reflection.hh>
#include <context.hh>
#include <seq.hh>
#include <string.hh>
#include <style.hh>
#include <sym.hh>
#include <temp.hh>
#include <tree.hh>
#include <type.hh>
#include <utils.hh>

#include <string>

#define _F(name, func, num, style, strict, type) \
name, Envf::create (name, num, style, strict, (void*)func, type)

#define _T(name, func, num, style, strict, type, var) \
name, Temp::create (name, var, type, Envf::create (name, num, style, strict, (void*)func, type))

#define _TT(BODY, PARAM) (TypeTemplate::create (BODY, PARAM))

#define P1(T1, T2) (Proj::create (T1, T2))
#define P2(T, ...) (Proj::create (T, P1(__VA_ARGS__)))
#define P3(T, ...) (Proj::create (T, P2(__VA_ARGS__)))
#define P4(T, ...) (Proj::create (T, P3(__VA_ARGS__)))
#define P5(T, ...) (Proj::create (T, P4(__VA_ARGS__)))
#define P6(T, ...) (Proj::create (T, P5(__VA_ARGS__)))

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

namespace NAMESPACE
{
  struct _builtin_t
  {
    std::string name;
    TermPtr     term;
  };

  void
  Builtin::init (Machine* machine)
  {
    TermPtr x = Sym::create ("x");
    TermPtr y = Sym::create ("y");

    TermVec xy;
    xy.push_back (x);
    xy.push_back (y);

    _builtin_t _builtins[] = {
      //// built-in types ////
      {"AMB",     AMB_T},
      {"ANY",     Term::T},
      {"BOOL",    Bool::T},
      {"CONS",    Cons::T},
      {"CONSH",   Consh::T},
      {"EXC",     Exc::T},
      {"INT",     Int::T},
      {"PROJ",    Proj::T},
      {"RAW",     Raw::T},
      {"RET",     Ret::T},
      {"SCOPE",   Scope::T},
      {"SPACE",   Space::T},
      {"STR",     Str::T},
      {"SYM",     Sym::T},
      {"TYPE",    Type::T},
      {"DEP",     DEP_T},
      {"UPR",     UPR_T},
      {"VOID",    VOID_T},
      //// core constants ////
      {"nil",     NIL},
      {"void",    VOID},
      {"true",    Bool::TRUE},
      {"false",   Bool::FALSE},
      //// built-in constants ////
      {"PREFIX",   Int::create (PREFIX)},
      {"POSTFIX",  Int::create (POSTFIX)},
      {"LINFIX",   Int::create (LINFIX)},
      {"RINFIX",   Int::create (RINFIX)},
      {"EOS",      Int::create (EOS)},
      {"GROUP",    Int::create (GROUP)},
      {"DUMMY",    Int::create (DUMMY)},
      //// precedence ////
      {"PREC_MIN", Int::create (PREC_MIN)},
      {"PREC_SEP", Int::create (PREC_SEP)},
      {"PREC_NOR", Int::create (PREC_NOR)},
      {"PREC_MAX", Int::create (PREC_MAX)},
      //// extension type ////
      {"EXT_INT",  Int::create (Extf::INT)},
      {"EXT_PTR",  Int::create (Extf::PTR)},
      {"EXT_VOID", Int::create (Extf::NONE)},
      //// exception id ////
      {"E_ASSIGN", Int::create (E_ASSIGNMENT)},
      {"E_ARG",    Int::create (E_ARG)},
      {"E_CAST",   Int::create (E_CAST)},
      {"E_DUP",    Int::create (E_DUP_NAME)},
      {"E_USER",   Int::create (E_USER)},
      //// scope flags ////
      {"GET_OPAQUE", Int::create (Context::GET_OPAQUE)},
      {"SET_TRANS",  Int::create (Context::SET_TRANS)},
      //// evaluation ////
      {_F ("bind",  bind,              1, BIND, 0,  P1 (Raw::T, DEP_T))},
//       {_F ("red",   red,               2, SIDE, 0,  P2 (Raw::T, Int::T, UPR_T))},
      {_F ("redsh",  redsh,             1, SIDE, 0,  P1 (Raw::T, UPR_T))},
      {_F ("redshx", redshx,            2, SIDE, 0,  P2 (Raw::T, Raw::T, UPR_T))},
      {_F ("tok",    tok_str,           1, BIND, S0, P1 (Str::T, Raw::T))},
      {_F ("tok",    tok_sym,           1, BIND, S0, P1 (Sym::T, Raw::T))},
      //// machine manipulation functions ////
      {_F ("name_get",  get_special,       1, CTXT, SX, P1 (Str::T, Str::T))},
      {_F ("name_set",          set_special,       2, CTXT, SX, P2 (Str::T, Str::T, VOID_T))},
      {_F ("set_attr",          set_attr,          2, CTXT, SX, P2 (Str::T, Int::T, VOID_T))},
      //// machine manipulation functions ////
      {_F ("slot_get", get_slot, 1, CTXT, SX, P1 (Str::T, Term::T))},
      {_F ("slot_set", set_slot, 2, CTXT, SX, P2 (Str::T, Raw::T, VOID_T))},
      //// scope manipulation functions ////
      {_F ("scope_pop",   scope_pop,   0, CTXT, 0,  VOID_T)},
      {_F ("scope_popx",  scope_popx,  0, CTXT, 0,  Scope::T)},
      {_F ("scope_push",  scope_push,  0, CTXT, 0,  VOID_T)},
      {_F ("scope_pushx", scope_pushx, 1, CTXT, S0, P1 (Scope::T, VOID_T))},
      {_F ("scope_set",   scope_set,   1, CTXT, S0, P1 (Int::T, VOID_T))},
      //////////////////////////////////////
      {_F ("return", retval, 1, CTXT, S0,    P1 (Term::T, VOID_T))},
      {_F ("retpop", retpop, 1, CTXT, S0,    P1 (Term::T, VOID_T))},
      {_F ("as",     as,     2, PURE, S0+M1, P2 (Type::T, Term::T, DEP_T))},
      {_F ("sa",     sa,     1, PURE, S0,    P1 (Term::T, DEP_T))},
      //// name manipulation functions ////
      {_F ("defsty", defsty, 3, CTXT, S0+S1+S2, P3 (Str::T, Int::T, Int::T, VOID_T))},
      {_F ("defout", defout, 2, CTXT, S0+S1,    P2 (Str::T, Str::T, VOID_T))},
      {_F ("defmap", defmap, 2, CTXT, S0+S1,    P2 (Str::T, Term::T, VOID_T))},
      {_F ("defvar", defmap, 2, CTXT, S0+M1,    P2 (Str::T, Term::T, VOID_T))},
      {_F ("define", define, 3, CTXT, S0+S1+M2, P3 (Str::T, Term::T, Raw::T, VOID_T))},
      {_F ("defaty", defaty, 1, CTXT, S0,       P1 (Str::T, VOID_T))},
      {_F ("deftmp", deftmp, 4, CTXT, S0+S2,    P4 (Str::T, Raw::T, Term::T, Raw::T, VOID_T))},
      {_F ("deflib", deflib, 2, CTXT, S0+S1,    P2 (Str::T, Str::T, Lib::T))},
      {_F ("defext", defext, 6, CTXT, SX,       P6 (Str::T, Term::T, Int::T, Str::T, Lib::T, Int::T, VOID_T))},
      {_F ("undef",  undef,  1, CTXT, S0,       P1 (Str::T, VOID_T))},
      {_F ("undef",  undefx, 2, CTXT, S0+S1,    P2 (Space::T, Str::T, VOID_T))},
      {_F ("redef",  redef,  2, CTXT, S0+M1,    P2 (Str::T, Raw::T, VOID_T))},
      //// namespace manipulation functions ////
      {_F ("new_ns",   new_ns,   0, PURE, 0,        Space::T)},
      {_F ("get_ns",   get_ns,   2, CTXT, S0+S1,    P2 (Space::T, Str::T, Term::T))},
      {_F ("set_ns",   set_ns,   1, CTXT, S0,       P1 (Term::T, VOID_T))},
      ////////
//       {_F ("cptr",    cptr,     1, CTXT, S0,       P1 (Int::T, Term::T))},
      {_F ("crec",     crec,     1, CTXT, S0,       P1 (Int::T, Term::T))},
      {_F ("cmem",     cmem,     2, CTXT, S0+S1,    P2 (Term::T, Int::T, Term::T))},
      //// pure meta-reflection functions ////
      {_F ("\\",       lambda,   2, META, Z0+Z1,    P2 (Raw::T, Raw::T, DEP_T))},
      {_F ("\\\\",     lambdax,  2, META, S0+Z1,    P2 (Sym::T, Raw::T, DEP_T))},
      {_F ("\\\\\\",   lambdas,  2, META, S0+B1,    P2 (Str::T, Raw::T, DEP_T))},
      {_F ("proj",     proj,     2, META, M0+M1,    P2 (Term::T, Term::T, Type::T))},
      //// reflection functions ////
      {_F ("is_sub",   is_sub,   2, PURE, SX,       P2 (Type::T, Type::T, Bool::T))},
      {_F ("type",     type,     1, PURE, SX,       P1 (Str::T, Type::T))},
      {_F ("typetmp",  typetmp,  2, PURE, SX,       P2 (Term::T, Term::T, Type::T))},
      {_F ("set_int",  set_int,  2, CTXT, SX,       P2 (Int::T, Int::T, VOID_T))},
      {_F ("set_str",  set_str,  2, CTXT, SX,       P2 (Str::T, Str::T, VOID_T))},
      {_F ("typeof",   type_of,  1, PURE, 0,        P1 (Term::T, Type::T))},
      ////
      {_F ("throw",    raise,    4, CTXT, SX,       P4 (Int::T, Term::T, Term::T, Term::T, VOID_T))},
//       {_F ("set",      set,      2, CTXT, S0+S1,    P2 (Term::T, Term::T, VOID_T))},
      //// homogenuous cons list functions ////
      {_T ("cons",     cons,     2, PURE, B0+B1,    P2 (x, _TT(Cons::T, x), _TT (Cons::T, x)), x)},
      {_T ("cons",     cons,     2, PURE, B0+B1,    P2 (x, x, _TT(Cons::T, x)), x)},
      {_T ("cons",     cons,     2, PURE, B0+B1,    P2 (x, NIL, _TT(Cons::T, x)), x)},
      {_T ("head",     head,     1, PURE, S0,       P1 (_TT(Cons::T, x), x), x)},
      {_T ("tail",     tail,     1, PURE, S0,       P1 (_TT(Cons::T, x), _TT(Cons::T, x)), x)},
      //// hetereguous cons list functions ////
      {_T ("consh",    consh,    2, PURE, B0+B1,    P2 (x, y, Consh::create (x, y)), xy)},
      {_F ("headh",    headh,    1, PURE, S0,       P1 (Consh::T, DEP_T))},
      {_F ("tailh",    tailh,    1, PURE, S0,       P1 (Consh::T, DEP_T))},
      //// utility functions ////
      {_F ("cons_t2",  cons_t2,  1, PURE, 0,        P1 (Raw::T, _TT(Cons::T, Raw::T)))},
      //// utility functions ////
      {_F ("solve",    solve,    3, PURE, S0+M1+M2, P3 (Term::T, Raw::T, Raw::T, DEP_T))},
      {_F ("subs",     subs,     3, PURE, S0+S1+M2, P3 (Term::T, Term::T, Term::T, Term::T))},
      {_F ("ifeq",     ifeq,     4, PURE, S0+S1,    P4 (Term::T, Term::T, Raw::T, Raw::T, DEP_T))},
      {_F ("add",      add_i,    2, PURE, SX,       P2 (Int::T, Int::T, Int::T))},
      {_F ("sub",      sub_i,    2, PURE, SX,       P2 (Int::T, Int::T, Int::T))},
      {_F ("mul",      mul_i,    2, PURE, SX,       P2 (Int::T, Int::T, Int::T))},
      {_F ("div",      div_i,    2, PURE, SX,       P2 (Int::T, Int::T, Int::T))},
      {_F ("mod",      mod_i,    2, PURE, SX,       P2 (Int::T, Int::T, Int::T))},
      {_F ("equ",      equ_i,    2, PURE, SX,       P2 (Int::T, Int::T, Bool::T))},
      {_F ("less",     less_i,   2, PURE, SX,       P2 (Int::T, Int::T, Bool::T))},
      {_F ("greater",  greater_i,2, PURE, SX,       P2 (Int::T, Int::T, Bool::T))},
      {_F ("concat",   concat,   2, PURE, SX,       P2 (Str::T, Str::T, Str::T))},
      {_F ("raw_out",  raw_print,1, SIDE, S0,       P1 (Str::T, VOID_T))},
      {_F ("caststr",  cast_str, 1, PURE, SX,       P1 (Term::T, Str::T))},
      {_F ("cstr",     cstr,     1, PURE, SX,       P1 (Str::T, Str::T))}
    };

    for (unsigned int i=0; i<sizeof(_builtins)/sizeof(_builtin_t); ++i)
      machine->context()->add_symbol (_builtins[i].term, _builtins[i].name);
  }
};
