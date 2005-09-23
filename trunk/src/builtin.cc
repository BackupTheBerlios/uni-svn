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
    _builtin_t _builtins[] = {
      //// built-in types ////
      {"AMB",     AMB_T},
      {"ANY",     Term::T},
      {"BOOL",    Bool::T},
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
//       {"EXT_INT",  Int::create (Extf::INT)},
//       {"EXT_PTR",  Int::create (Extf::PTR)},
//       {"EXT_VOID", Int::create (Extf::NONE)},
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
//       {_F ("red",   red,               2, SIDE, 0,  P2 (Raw::T, Int::T, UPR_T))},
      //// machine manipulation functions ////
//       {_F ("set_attr",          set_attr,          2, CTXT, SX, P2 (Str::T, Int::T, VOID_T))},
      //////////////////////////////////////
      ////////
//       {_F ("cptr",    cptr,     1, CTXT, S0,       P1 (Int::T, Term::T))},
//       {_F ("crec",     crec,     1, CTXT, S0,       P1 (Int::T, Term::T))},
//       {_F ("cmem",     cmem,     2, CTXT, S0+S1,    P2 (Term::T, Int::T, Term::T))},
      //// pure meta-reflection functions ////
      //// reflection functions ////
//       {_F ("set_int",  set_int,  2, CTXT, SX,       P2 (Int::T, Int::T, VOID_T))},
//       {_F ("set_str",  set_str,  2, CTXT, SX,       P2 (Str::T, Str::T, VOID_T))},
      ////
//       {_F ("set",      set,      2, CTXT, S0+S1,    P2 (Term::T, Term::T, VOID_T))},
      //// utility functions ////
//       {_F ("ifeq",     ifeq,     4, PURE, S0+S1,    P4 (Term::T, Term::T, Raw::T, Raw::T, DEP_T))},






//       {_F ("deflib", deflib, 2, CTXT, S0+S1,    P2 (Str::T, Str::T, Lib::T))},
//       {_F ("defext", defext, 6, CTXT, SX,       P6 (Str::T, Term::T, Int::T, Str::T, Lib::T, Int::T, VOID_T))},

//       {_F ("add",      add_i,    2, PURE, SX,       P2 (Int::T, Int::T, Int::T))},
//       {_F ("sub",      sub_i,    2, PURE, SX,       P2 (Int::T, Int::T, Int::T))},
//       {_F ("mul",      mul_i,    2, PURE, SX,       P2 (Int::T, Int::T, Int::T))},
//       {_F ("div",      div_i,    2, PURE, SX,       P2 (Int::T, Int::T, Int::T))},
//       {_F ("mod",      mod_i,    2, PURE, SX,       P2 (Int::T, Int::T, Int::T))},
//       {_F ("equ",      equ_i,    2, PURE, SX,       P2 (Int::T, Int::T, Bool::T))},
//       {_F ("less",     less_i,   2, PURE, SX,       P2 (Int::T, Int::T, Bool::T))},
//       {_F ("greater",  greater_i,2, PURE, SX,       P2 (Int::T, Int::T, Bool::T))},
//       {_F ("concat",   concat,   2, PURE, SX,       P2 (Str::T, Str::T, Str::T))},
//       {_F ("caststr",  cast_str, 1, PURE, SX,       P1 (Term::T, Str::T))},
//       {_F ("cstr",     cstr,     1, PURE, SX,       P1 (Str::T, Str::T))}
    };

    for (unsigned int i=0; i<sizeof(_builtins)/sizeof(_builtin_t); ++i)
      machine->context()->add_symbol (_builtins[i].term, _builtins[i].name);
  }
};
