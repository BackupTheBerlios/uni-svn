#include <abs.hh>
#include <app.hh>
#include <bool.hh>
#include <cons.hh>
#include <context.hh>
#include <dict.hh>
#include <exception.hh>
#include <func.hh>
#include <int.hh>
#include <list.hh>
#include <proj.hh>
#include <reflection.hh>
#include <scope.hh>
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
  class RefListVisitor : public ListVisitor
  {
    Context* _context;
    TermPtr  _visitor;

  public:

    RefListVisitor (Context *context, TermPtr visitor)
      : _context (context), _visitor (visitor) { }

    virtual void visit_list_item (TermPtr item)
    {
      AppPtr app = App::create (_visitor, item);
      _context->reduce (app, ALL);
    };
  };

  class RefDictVisitor : public DictVisitor
  {
    Context* _context;
    TermPtr  _visitor;

  public:

    RefDictVisitor (Context *context, TermPtr visitor)
      : _context (context), _visitor (visitor) { }

    virtual void visit_dict_item (const string& key, TermPtr item)
    {
      AppPtr app = App::create (_visitor, MStr::create (key));
      app = App::create (app, item);
      _context->reduce (app, ALL);
    };
  };

  TermPtr
  retval (Context* c, TermPtr term)
  {
    return Ret::create (term);
  }

  TermPtr
  retpop (Context* c, TermPtr term)
  {
    c->scopes()->pop();
    return Ret::create (term);
  }

  TermPtr
  as (Context* c, TermPtr type, TermPtr term)
  {
    return AsType::create (type, term);
  }

  TermPtr
  sa (Context* c, TermPtr term)
  {
    shared_ptr<AsType> as = TCAST<AsType> (term);
    return as->body();
  }

  TermPtr
  lambda (Context* c, TermPtr var, TermPtr body)
  {
    return Abs::create (var, body);
  }

  TermPtr
  lambdax (Context* c, TermPtr t_var, TermPtr body)
  {
    const char* var = TCAST<Str>(t_var)->str();
    return Abs::create (Tok::create (var), body);
  }

  TermPtr
  lambdas (Context* c, TermPtr t_var, TermPtr body)
  {
    const char* var = TCAST<Str>(t_var)->str();
    AbsPtr abs = Abs::create (Sym::create (var), body);
    return Abs::create (Tok::create (var), abs->body());
  }

  TermPtr
  solve (Context* c, TermPtr var, TermPtr lhs, TermPtr rhs)
  {
    return lhs->solve (var, rhs);
  }

  //// defsty //////////////////////////////////////////////////////////////////
  TermPtr
  defsty (Context* c, TermPtr n, TermPtr f, TermPtr p)
  {
    const char* name = TCAST<Str>(n)->str();
    int fix  = TCAST<Int>(f)->val();
    int prec = TCAST<Int>(p)->val();
    int sty  = MAKE_STYLE (fix, prec);
    c->scopes()->add_style (name, sty);
    return VOID;
  }

  //// defout //////////////////////////////////////////////////////////////////
  TermPtr
  defout (Context* c, TermPtr t_lhs, TermPtr t_rhs)
  {
    const char *lhs = TCAST<Str>(t_lhs)->str();
    const char *rhs = TCAST<Str>(t_rhs)->str();
    c->scopes()->add_outfix (lhs, rhs);
    return VOID;
  }

  //// defmap //////////////////////////////////////////////////////////////////
  TermPtr
  defmap (Context* c, TermPtr n, TermPtr term)
  {
    string  name = TCAST<Str>(n)->str();
    TermPtr body = term;
    c->scopes()->add_symbol (body, name);
    return VOID;
  }

  //// define //////////////////////////////////////////////////////////////////
  TermPtr
  define (Context* c, TermPtr n, TermPtr t, TermPtr b)
  {
    string  name = TCAST<Str>(n)->str();
    TermPtr func = Intf::create (name, b, t);
    c->scopes()->add_symbol (func, name);
    return VOID;
  }

  TermPtr
  defaty (Context* context, TermPtr t_name)
  {
    const char* name = TCAST<Str>(t_name)->str();
    context->scopes()->add_symbol (Type::create (name), name);
    return VOID;
  }

  //// deflib //////////////////////////////////////////////////////////////////
  TermPtr
  deflib (Context* context, TermPtr t_name, TermPtr t_path)
  {
    string name = (TCAST<Str> (t_name))->str();
    string path = (TCAST<Str> (t_path))->str();
    context->scopes()->add_symbol (Lib::create (path), name);
    return VOID;
  }

  //// defext //////////////////////////////////////////////////////////////////
  TermPtr
  defext (Context* context,
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

    context->scopes()->add_symbol (func, name);
    return VOID;
  }

  //// deftmp //////////////////////////////////////////////////////////////////
  TermPtr
  deftmp (Context* c, TermPtr n, TermPtr var, TermPtr type, TermPtr body)
  {
    const char* name = TCAST<Str>(n)->str();
    shared_ptr<Temp> ft = Temp::create (name, var, type, body);
    c->scopes()->add_symbol (ft, name);
    return VOID;
  }

  //// undefine ////////////////////////////////////////////////////////////////
  TermPtr
  undef (Context* c, TermPtr name)
  {
    c->scopes()->del_symbol (TCAST<Str>(name)->str());
    return VOID;
  }

  TermPtr
  undefx (Context* c, TermPtr t_nspace, TermPtr t_name)
  {
    SpacePtr nspace = TCAST<Space> (t_nspace);
    StrPtr   name   = TCAST<Str>   (t_name);
    c->scopes()->del_symbol (name->str(), nspace);
    return VOID;
  }

  //// redef ///////////////////////////////////////////////////////////////////
  TermPtr redef (Context* context, TermPtr t_name, TermPtr term)
  {
    const char* name = TCAST<Str>(t_name)->str();
    if (! context->scopes()->set_symbol (name, term))
      context->scopes()->add_symbol (term, name);
    return VOID;
  }

  TermPtr
  new_ns (Context* context)
  {
    return Space::create();
  }

  TermPtr
  get_ns (Context* context, TermPtr t_ns, TermPtr t_name)
  {
    SpacePtr nspace = TCAST<Space> (t_ns);
    StrPtr   name   = TCAST<Str> (t_name);

    if (TermPtr result = context->scopes()->get_symbol (name->str(), nspace))
      return result;
    else
      return NIL;
  }

  TermPtr
  set_ns (Context* context, TermPtr t_ns)
  {
    if (NIL == t_ns)
      context->scopes()->current_nspace (SpacePtr());
    else {
      SpacePtr nspace = TCAST<Space> (t_ns);
      context->scopes()->current_nspace (nspace);
    }

    return VOID;
  }

  TermPtr
  crec (Context* context, TermPtr t_size)
  {
    int size = TCAST<Int>(t_size)->val();
    return CRec::create (size);
  }

  TermPtr
  cmem (Context* context, TermPtr type, TermPtr t_size)
  {
    int size = TCAST<Int>(t_size)->val();
    return CMem::create (type, size);
  }

  TermPtr
  subs (Context* c, TermPtr from, TermPtr to, TermPtr term)
  {
    return term->sub (from, to);
  }

  TermPtr
  bind (Context* c, TermPtr term)
  {
    return c->reduce (term, BIND);
  }

  TermPtr
  redsh (Context* c, TermPtr term)
  {
    TermPtr r = c->reduce_in_shield (term);
    return r;
  }

  TermPtr
  redshx (Context* c, TermPtr term, TermPtr exit)
  {
    TermPtr r = c->reduce_in_shield (term, ALL, exit);
    return r;
  }

  TermPtr
  is_sub (Context* c, TermPtr sub, TermPtr super)
  {
    return sub->compat (super) ? Bool::TRUE : Bool::FALSE;
  }

  TermPtr
  type_of (Context* c, TermPtr term)
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
  proj (Context* c, TermPtr left, TermPtr right)
  {
    return Proj::create (left,right);
  }

  TermPtr
  type (Context* c, TermPtr name)
  {
    return Type::create ((TCAST<Str>(name))->str());
  }

  TermPtr
  typetmp (Context* context, TermPtr body, TermPtr param)
  {
    return TypeTemplate::create (body, param);
  }

  TermPtr
  get_special (Context* c, TermPtr n)
  {
    const char* name = TCAST<Str>(n)->str();
    return MStr::create (c->scopes()->get_special(name).c_str());
  }

  TermPtr
  set_special (Context* c, TermPtr n, TermPtr v)
  {
    const char* name  = TCAST<Str>(n)->str();
    const char* value = TCAST<Str>(v)->str();
    c->scopes()->set_special (name, value);
    return VOID;
  }

  TermPtr
  set_attr (Context* c, TermPtr n, TermPtr v)
  {
    const char* name  = TCAST<Str>(n)->str();
    int value = TCAST<Int>(v)->val();
    c->set_attr (name, value);
    return VOID;
  }

  TermPtr get_slot (Context* c, TermPtr t_name)
  {
    const char* name  = TCAST<Str>(t_name)->str();
    if (TermPtr result = c->scopes()->get_slot (name))
      return result;
    else
      throw E (E_NO_MATCH, t_name, Term::T);
  }

  TermPtr set_slot (Context* c, TermPtr n, TermPtr val)
  {
    const char* name  = TCAST<Str>(n)->str();
    c->scopes()->set_slot (name, val);
    return VOID;
  }

  TermPtr scope_pushx (Context* c, TermPtr t_scope)
  {
    ScopePtr scope = TCAST <Scope> (t_scope);
    c->scopes()->push (scope);
    return VOID;
  }

  TermPtr scope_push (Context* c)
  {
    c->scopes()->push();
    return VOID;
  }

  TermPtr scope_pop (Context* c)
  {
    c->scopes()->pop();
    return VOID;
  }

  TermPtr scope_popx (Context* c)
  {
    ScopePtr result = c->scopes()->top();
    c->scopes()->pop();
    return result;
  }

  TermPtr scope_set (Context* c, TermPtr t_flags)
  {
    int flags = TCAST<Int>(t_flags)->val();
    c->scopes()->current_flags (flags);
    return VOID;
  }

  TermPtr
  import (Context* c, TermPtr name)
  {
    c->import (TCAST<Str>(name)->str());
    return VOID;
  }

  TermPtr
  set_int (Context* c, TermPtr i, TermPtr val)
  {
    TCAST<Int>(i)->val (TCAST<Int>(val)->val());
    return VOID;
  }

  TermPtr
  set_str (Context* c, TermPtr i, TermPtr val)
  {
//     TCAST<Str>(i)->str (TCAST<Str>(val)->str());
    return VOID;
  }

  TermPtr
  ifeq (Context* c, TermPtr lhs, TermPtr rhs, TermPtr eq, TermPtr ne)
  {
    return ((lhs == rhs) || (lhs->equ (rhs))) ? eq : ne;
  }

  TermPtr
  cons (Context* context, TermPtr left, TermPtr right)
  {
    return Cons::create (left, right);
  }

  TermPtr
  consh (Context* context, TermPtr left, TermPtr right)
  {
    return Consh::create (left, right);
  }

  TermPtr
  head (Context* context, TermPtr cons)
  {
    ConsPtr c = TCAST<Cons> (cons);
    return c->left();
  }

  TermPtr
  tail (Context* context, TermPtr cons)
  {
    ConsPtr c = TCAST<Cons> (cons);
    return c->right();
  }

  TermPtr
  headh (Context* context, TermPtr consh)
  {
    ConshPtr c = TCAST<Consh> (consh);
    return c->left();
  }

  TermPtr
  tailh (Context* context, TermPtr consh)
  {
    ConshPtr c = TCAST<Consh> (consh);
    return c->right();
  }

  TermPtr
  cons_t2 (Context* context, TermPtr term)
  {
    TermPtr result = NIL;
    term = context->reduce (term, CONS);

    for (P(STree<2>) tree; tree = CAST< STree<2> > (term); term = tree->elem(0))
      result = Cons::create (tree->elem(1), result);
    result = Cons::create (term, result);
    return result;
  }

//   TermPtr
//   let (Context* context, TermPtr var, TermPtr exp, TermPtr body)
//   {
//     return Let::create (var, exp, body);
//   }

  TermPtr
  dict_new (Context* context, TermPtr type)
  {
    return Dict::create (type);
  }

  TermPtr
  dict_has (Context* context, TermPtr t_dict, TermPtr t_name)
  {
    DictPtr dict = TCAST<Dict> (t_dict);
    StrPtr  name = TCAST<Str>  (t_name);

    return (dict->has (name->str())) ? Bool::TRUE : Bool::FALSE;
  }

  TermPtr
  dict_get (Context* context, TermPtr t_dict, TermPtr t_name)
  {
    DictPtr dict = TCAST<Dict> (t_dict);
    StrPtr  name = TCAST<Str>  (t_name);

    if (TermPtr result = dict->get (name->str()))
      return result;
    else
      throw E (E_NO_KEY, t_dict, t_name);
  }

  TermPtr
  dict_set (Context* context, TermPtr t_dict, TermPtr t_name, TermPtr term)
  {
    DictPtr dict = TCAST<Dict> (t_dict);
    StrPtr  name = TCAST<Str>  (t_name);

    dict->set (name->str(), term);
    return VOID;
  }

  TermPtr dict_vis (Context* context, TermPtr t_dict, TermPtr t_visitor)
  {
    DictPtr dict = TCAST<Dict> (t_dict);
    RefDictVisitor visitor (context, t_visitor);
    dict->visit_dict (visitor);
    return VOID;
  }

  TermPtr list_new (Context* context, TermPtr type)
  {
    return List::create (type);
  }

  TermPtr list_ins (Context* context, TermPtr t_list, TermPtr term)
  {
    ListPtr list = TCAST<List> (t_list);
    list->insert_front (term);
    return VOID;
  }

  TermPtr list_add (Context* context, TermPtr t_list, TermPtr term)
  {
    ListPtr list = TCAST<List> (t_list);
    list->insert_back (term);
    return VOID;
  }

  TermPtr list_vis (Context* context, TermPtr t_list, TermPtr t_visitor)
  {
    ListPtr list = TCAST<List> (t_list);
    RefListVisitor visitor (context, t_visitor);
    list->visit_list (visitor);
    return VOID;
  }

  TermPtr
  set (Context* context, TermPtr t_ref, TermPtr term)
  {
    DictPtr ref = TCAST<Dict> (t_ref);
    TermPtr obj = ref->get ("obj");
    TermPtr key = ref->get ("key");

    if (obj) {
      if (IntPtr x = CAST<Int> (obj))
	return set_int (context, x, term);
      else if (StrPtr x = CAST<Str> (obj))
	return set_str (context, x, term);
      else if (DictPtr x = CAST<Dict> (obj)) {
	if (key)
	  return dict_set (context, x, key, term);
	else
	  throw E (E_NO_KEY, x, key);
      }
      else
	throw 4;
    }
    else
      throw E (E_NO_KEY, ref, MStr::create ("obj"));
  }

  TermPtr tok_str (Context* context, TermPtr t_name)
  {
    return Tok::create (TCAST<Str>(t_name)->str());
  }

  TermPtr tok_sym (Context* context, TermPtr t_name)
  {
    return Tok::create (TCAST<Sym>(t_name)->str());
  }

  TermPtr raise (Context* context, TermPtr t_id, TermPtr t1, TermPtr t2, TermPtr t3)
  {
    int id = TCAST<Int>(t_id)->val();
    throw E (id, t1, t2, t3);
  }

  TermPtr red (Context* context, TermPtr term, TermPtr t_flags)
  {
    int flags = TCAST<Int>(t_flags)->val();
    ListPtr l = List::create();
    l->insert_back (context->reduce (term, flags));
    return l;
  }
};
