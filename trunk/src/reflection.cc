#include <abs.hh>
#include <app.hh>
#include <bool.hh>
#include <cons.hh>
#include <machine.hh>
#include <exception.hh>
#include <func.hh>
#include <int.hh>
#include <proj.hh>
#include <reflection.hh>
#include <context.hh>
#include <seq.hh>
#include <string.hh>
#include <style.hh>
#include <sym.hh>
#include <temp.hh>
#include <tok.hh>
#include <type.hh>
#include <opt.hh>

namespace NAMESPACE
{
  TermPtr
  retval (Machine* c, TermPtr term)
  {
    return Ret::create (term);
  }

  TermPtr
  retpop (Machine* c, TermPtr term)
  {
    c->context()->pop();
    return Ret::create (term);
  }

  TermPtr
  as (Machine* c, TermPtr type, TermPtr term)
  {
    return AsType::create (type, term);
  }

  TermPtr
  sa (Machine* c, TermPtr term)
  {
    shared_ptr<AsType> as = TCAST<AsType> (term);
    return as->body();
  }

  TermPtr
  lambda (Machine* c, TermPtr var, TermPtr body)
  {
    return Abs::create (var, body);
  }

  TermPtr
  lambdax (Machine* c, TermPtr t_var, TermPtr body)
  {
    const char* var = TCAST<Str>(t_var)->str();
    return Abs::create (Tok::create (var), body);
  }

  TermPtr
  lambdas (Machine* c, TermPtr t_var, TermPtr body)
  {
    const char* var = TCAST<Str>(t_var)->str();
    AbsPtr abs = Abs::create (Sym::create (var), body);
    return Abs::create (Tok::create (var), abs->body());
  }

  TermPtr
  solve (Machine* c, TermPtr var, TermPtr lhs, TermPtr rhs)
  {
    return lhs->solve (var, rhs);
  }

  //// defsty //////////////////////////////////////////////////////////////////
  TermPtr
  defsty (Machine* c, TermPtr n, TermPtr f, TermPtr p)
  {
    const char* name = TCAST<Str>(n)->str();
    int fix  = TCAST<Int>(f)->val();
    int prec = TCAST<Int>(p)->val();
    int sty  = MAKE_STYLE (fix, prec);
    c->context()->add_style (name, sty);
    return VOID;
  }

  //// defout //////////////////////////////////////////////////////////////////
  TermPtr
  defout (Machine* c, TermPtr t_lhs, TermPtr t_rhs)
  {
    const char *lhs = TCAST<Str>(t_lhs)->str();
    const char *rhs = TCAST<Str>(t_rhs)->str();
    c->context()->add_outfix (lhs, rhs);
    return VOID;
  }

  //// defmap //////////////////////////////////////////////////////////////////
  TermPtr
  defmap (Machine* c, TermPtr n, TermPtr term)
  {
    string  name = TCAST<Str>(n)->str();
    TermPtr body = term;
    c->context()->add_symbol (body, name);
    return VOID;
  }

  //// define //////////////////////////////////////////////////////////////////
  TermPtr
  define (Machine* c, TermPtr n, TermPtr t, TermPtr b)
  {
    string  name = TCAST<Str>(n)->str();
    TermPtr func = Intf::create (name, b, t);
    c->context()->add_symbol (func, name);
    return VOID;
  }

  TermPtr
  defaty (Machine* machine, TermPtr t_name)
  {
    const char* name = TCAST<Str>(t_name)->str();
    machine->context()->add_symbol (Type::create (name), name);
    return VOID;
  }

  //// deflib //////////////////////////////////////////////////////////////////
  TermPtr
  deflib (Machine* machine, TermPtr t_name, TermPtr t_path)
  {
    string name = (TCAST<Str> (t_name))->str();
    string path = (TCAST<Str> (t_path))->str();
    machine->context()->add_symbol (Lib::create (path), name);
    return VOID;
  }

  //// defext //////////////////////////////////////////////////////////////////
  TermPtr
  defext (Machine* machine,
	  TermPtr  t_name,
	  TermPtr  t_type,
	  TermPtr  t_arity,
	  TermPtr  t_cname,
	  TermPtr  t_lib,
	  TermPtr  t_ret)
  {
    string  name  = TCAST<Str> (t_name)->str();
    string  cname = TCAST<Str> (t_cname)->str();
    int     arity = TCAST<Int> (t_arity)->val();
    int     ret   = TCAST<Int> (t_ret)->val();
    P(Lib)  lib   = TCAST<Lib> (t_lib);
    TermPtr func  = Extf::create (cname, (unsigned int) arity, lib, t_type, ret);

    machine->context()->add_symbol (func, name);
    return VOID;
  }

  //// deftmp //////////////////////////////////////////////////////////////////
  TermPtr
  deftmp (Machine* c, TermPtr n, TermPtr var, TermPtr type, TermPtr body)
  {
    const char* name = TCAST<Str>(n)->str();
    shared_ptr<Temp> ft = Temp::create (name, var, type, body);
    c->context()->add_symbol (ft, name);
    return VOID;
  }

  //// undefine ////////////////////////////////////////////////////////////////
  TermPtr
  undef (Machine* c, TermPtr name)
  {
    c->context()->del_symbol (TCAST<Str>(name)->str());
    return VOID;
  }

  TermPtr
  undefx (Machine* c, TermPtr t_nspace, TermPtr t_name)
  {
    SpacePtr nspace = TCAST<Space> (t_nspace);
    StrPtr   name   = TCAST<Str>   (t_name);
    c->context()->del_symbol (name->str(), nspace);
    return VOID;
  }

  //// redef ///////////////////////////////////////////////////////////////////
  TermPtr redef (Machine* machine, TermPtr t_name, TermPtr term)
  {
    const char* name = TCAST<Str>(t_name)->str();
    if (! machine->context()->set_symbol (name, term))
      machine->context()->add_symbol (term, name);
    return VOID;
  }

  TermPtr
  new_ns (Machine* machine)
  {
    return Space::create();
  }

  TermPtr
  get_ns (Machine* machine, TermPtr t_ns, TermPtr t_name)
  {
    SpacePtr nspace = TCAST<Space> (t_ns);
    StrPtr   name   = TCAST<Str> (t_name);

    if (TermPtr result = machine->context()->get_symbol (name->str(), nspace))
      return result;
    else
      return NIL;
  }

  TermPtr
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

  TermPtr
  crec (Machine* machine, TermPtr t_size)
  {
    int size = TCAST<Int>(t_size)->val();
    return CRec::create (size);
  }

  TermPtr
  cmem (Machine* machine, TermPtr type, TermPtr t_size)
  {
    int size = TCAST<Int>(t_size)->val();
    return CMem::create (type, size);
  }

  TermPtr
  subs (Machine* c, TermPtr from, TermPtr to, TermPtr term)
  {
    return term->sub (from, to);
  }

  TermPtr
  bind (Machine* c, TermPtr term)
  {
    return c->reduce (term, BIND);
  }

  TermPtr
  redsh (Machine* c, TermPtr term)
  {
    TermPtr r = c->reduce_in_shield (term);
    return r;
  }

  TermPtr
  redshx (Machine* c, TermPtr term, TermPtr exit)
  {
    TermPtr r = c->reduce_in_shield (term, ALL, exit);
    return r;
  }

  TermPtr
  is_sub (Machine* c, TermPtr sub, TermPtr super)
  {
    return sub->compat (super) ? Bool::TRUE : Bool::FALSE;
  }

  TermPtr
  type_of (Machine* c, TermPtr term)
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
      term = c->reduce (term, BIND);
      if (Raw::T == term->type())
	return Raw::T;
      else
	return type_of (c, term);
    }
    else if (DEP_T == result) {
      term = c->reduce (term, ALL_CTXT);
      if (DEP_T == term->type())
	return Raw::T;
      else
	return type_of(c, term);
    }
    else
      return result;
  }

  TermPtr
  proj (Machine* c, TermPtr left, TermPtr right)
  {
    return Proj::create (left,right);
  }

  TermPtr
  type (Machine* c, TermPtr name)
  {
    return Type::create ((TCAST<Str>(name))->str());
  }

  TermPtr
  typetmp (Machine* machine, TermPtr body, TermPtr param)
  {
    return TypeTemplate::create (body, param);
  }

  TermPtr
  get_special (Machine* c, TermPtr n)
  {
    const char* name = TCAST<Str>(n)->str();
    return MStr::create (c->context()->get_special(name).c_str());
  }

  TermPtr
  set_special (Machine* c, TermPtr n, TermPtr v)
  {
    const char* name  = TCAST<Str>(n)->str();
    const char* value = TCAST<Str>(v)->str();
    c->context()->set_special (name, value);
    return VOID;
  }

  TermPtr
  set_attr (Machine* c, TermPtr n, TermPtr v)
  {
    const char* name  = TCAST<Str>(n)->str();
    int value = TCAST<Int>(v)->val();
    c->set_attr (name, value);
    return VOID;
  }

  TermPtr get_slot (Machine* c, TermPtr t_name)
  {
    const char* name  = TCAST<Str>(t_name)->str();
    if (TermPtr result = c->context()->get_slot (name))
      return result;
    else
      throw E (E_NO_MATCH, t_name, Term::T);
  }

  TermPtr set_slot (Machine* c, TermPtr n, TermPtr val)
  {
    const char* name  = TCAST<Str>(n)->str();
    c->context()->set_slot (name, val);
    return VOID;
  }

  TermPtr scope_pushx (Machine* c, TermPtr t_scope)
  {
    ScopePtr scope = TCAST <Scope> (t_scope);
    c->context()->push (scope);
    return VOID;
  }

  TermPtr scope_push (Machine* c)
  {
    c->context()->push();
    return VOID;
  }

  TermPtr scope_pop (Machine* c)
  {
    c->context()->pop();
    return VOID;
  }

  TermPtr scope_popx (Machine* c)
  {
    ScopePtr result = c->context()->top();
    c->context()->pop();
    return result;
  }

  TermPtr scope_set (Machine *m, TermPtr t_flags)
  {
    int flags = TCAST<Int>(t_flags)->val();
    m->context()->current_flags (flags);
    return VOID;
  }

  TermPtr
  set_int (Machine *m, TermPtr i, TermPtr val)
  {
    TCAST<Int>(i)->val (TCAST<Int>(val)->val());
    return VOID;
  }

  TermPtr
  set_str (Machine *m, TermPtr i, TermPtr val)
  {
//     TCAST<Str>(i)->str (TCAST<Str>(val)->str());
    return VOID;
  }

  TermPtr
  ifeq (Machine *m, TermPtr lhs, TermPtr rhs, TermPtr eq, TermPtr ne)
  {
    return ((lhs == rhs) || (lhs->equ (rhs))) ? eq : ne;
  }

  TermPtr
  cons (Machine* machine, TermPtr left, TermPtr right)
  {
    return Cons::create (left, right);
  }

  TermPtr
  consh (Machine* machine, TermPtr left, TermPtr right)
  {
    return Consh::create (left, right);
  }

  TermPtr
  head (Machine* machine, TermPtr cons)
  {
    ConsPtr c = TCAST<Cons> (cons);
    return c->left();
  }

  TermPtr
  tail (Machine* machine, TermPtr cons)
  {
    ConsPtr c = TCAST<Cons> (cons);
    return c->right();
  }

  TermPtr
  headh (Machine* machine, TermPtr consh)
  {
    ConshPtr c = TCAST<Consh> (consh);
    return c->left();
  }

  TermPtr
  tailh (Machine* machine, TermPtr consh)
  {
    ConshPtr c = TCAST<Consh> (consh);
    return c->right();
  }

  TermPtr
  cons_t2 (Machine* machine, TermPtr term)
  {
    TermPtr result = NIL;
    term = machine->reduce (term, CONS);

    for (P(STree<2>) tree; tree = CAST< STree<2> > (term); term = tree->elem(0))
      result = Cons::create (tree->elem(1), result);
    result = Cons::create (term, result);
    return result;
  }

//   TermPtr
//   set (Machine* machine, TermPtr t_ref, TermPtr term)
//   {
//     DictPtr ref = TCAST<Dict> (t_ref);
//     TermPtr obj = ref->get ("obj");
//     TermPtr key = ref->get ("key");

//     if (obj) {
//       if (IntPtr x = CAST<Int> (obj))
// 	return set_int (machine, x, term);
//       else if (StrPtr x = CAST<Str> (obj))
// 	return set_str (machine, x, term);
//       else if (DictPtr x = CAST<Dict> (obj)) {
// 	if (key)
// 	  return dict_set (machine, x, key, term);
// 	else
// 	  throw E (E_NO_KEY, x, key);
//       }
//       else
// 	throw 4;
//     }
//     else
//       throw E (E_NO_KEY, ref, MStr::create ("obj"));
//   }

  TermPtr tok_str (Machine* machine, TermPtr t_name)
  {
    return Tok::create (TCAST<Str>(t_name)->str());
  }

  TermPtr tok_sym (Machine* machine, TermPtr t_name)
  {
    return Tok::create (TCAST<Sym>(t_name)->str());
  }

  TermPtr raise (Machine* machine, TermPtr t_id, TermPtr t1, TermPtr t2, TermPtr t3)
  {
    int id = TCAST<Int>(t_id)->val();
    throw E (id, t1, t2, t3);
  }

//   TermPtr red (Machine* machine, TermPtr term, TermPtr t_flags)
//   {
//     int flags = TCAST<Int>(t_flags)->val();
//     ListPtr l = List::create();
//     l->insert_back (machine->reduce (term, flags));
//     return l;
//   }
};
