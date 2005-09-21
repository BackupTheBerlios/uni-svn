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

//   TermPtr
//   crec (Machine* machine, TermPtr t_size)
//   {
//     int size = TCAST<Int>(t_size)->val();
//     return CRec::create (size);
//   }

//   TermPtr
//   cmem (Machine* machine, TermPtr type, TermPtr t_size)
//   {
//     int size = TCAST<Int>(t_size)->val();
//     return CMem::create (type, size);
//   }

//   TermPtr
//   set_attr (Machine* c, TermPtr n, TermPtr v)
//   {
//     const char* name  = TCAST<Str>(n)->str();
//     int value = TCAST<Int>(v)->val();
//     c->set_attr (name, value);
//     return VOID;
//   }

//   TermPtr
//   set_int (Machine *m, TermPtr i, TermPtr val)
//   {
//     TCAST<Int>(i)->val (TCAST<Int>(val)->val());
//     return VOID;
//   }

//   TermPtr
//   set_str (Machine *m, TermPtr i, TermPtr val)
//   {
//     TCAST<Str>(i)->str (TCAST<Str>(val)->str());
//     return VOID;
//   }

//   TermPtr
//   ifeq (Machine *m, TermPtr lhs, TermPtr rhs, TermPtr eq, TermPtr ne)
//   {
//     return ((lhs == rhs) || (lhs->equ (rhs))) ? eq : ne;
//   }

//   TermPtr
//   cons (Machine* machine, TermPtr left, TermPtr right)
//   {
//     return Cons::create (left, right);
//   }

//   TermPtr
//   consh (Machine* machine, TermPtr left, TermPtr right)
//   {
//     return Consh::create (left, right);
//   }

//   TermPtr
//   head (Machine* machine, TermPtr cons)
//   {
//     ConsPtr c = TCAST<Cons> (cons);
//     return c->left();
//   }

//   TermPtr
//   tail (Machine* machine, TermPtr cons)
//   {
//     ConsPtr c = TCAST<Cons> (cons);
//     return c->right();
//   }

//   TermPtr
//   headh (Machine* machine, TermPtr consh)
//   {
//     ConshPtr c = TCAST<Consh> (consh);
//     return c->left();
//   }

//   TermPtr
//   tailh (Machine* machine, TermPtr consh)
//   {
//     ConshPtr c = TCAST<Consh> (consh);
//     return c->right();
//   }

//   TermPtr
//   cons_t2 (Machine* machine, TermPtr term)
//   {
//     TermPtr result = NIL;
//     term = machine->reduce (term, CONS);

//     for (P(STree<2>) tree; tree = CAST< STree<2> > (term); term = tree->elem(0))
//       result = Cons::create (tree->elem(1), result);
//     result = Cons::create (term, result);
//     return result;
//   }

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

//   TermPtr red (Machine* machine, TermPtr term, TermPtr t_flags)
//   {
//     int flags = TCAST<Int>(t_flags)->val();
//     ListPtr l = List::create();
//     l->insert_back (machine->reduce (term, flags));
//     return l;
//   }
};
