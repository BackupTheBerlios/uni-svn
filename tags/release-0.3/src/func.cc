#include <context.hh>
#include <cval.hh>
#include <exception.hh>
#include <func.hh>
#include <int.hh>
#include <proj.hh>
#include <style.hh>
#include <type.hh>

#include <dlfcn.h>

#define MAX_C_ARGS 8

namespace NAMESPACE
{
  //////////////////////////////////////////////////////////////////////////////
  Intf::Intf (const std::string& name, TermPtr body, TermPtr type)
    : Func (type, name), _body(body)
  {
    assert (body);
    _proj = CAST<Proj>(type) ? true : false;
  }

  TermPtr
  Intf::reduce (Context* c, int flags, TermPtr expected)
  {
    if (! (flags & PURE))
      throw E_MODE;
    else if ((0 == c->arg_count()) && _proj)
      throw E_ARG;
    else
      return _body->clone (type());
  }

  //////////////////////////////////////////////////////////////////////////////
  Envf::Envf (const string& name,
	      unsigned int  arity,
	      unsigned int  style,
	      unsigned int  strictness,
	      void*         entry,
	      TermPtr       type)
    : Func (type, name),
      _arity (arity),
      _style (style | BIND),      // BIND is always required
      _strict (strictness),
      _entry ((_entry_type)entry)
  {
    assert (_arity <= MAX_C_ARGS);
  }

  TermPtr
  Envf::reduce (Context* c, int flags, TermPtr expected)
  {
    if ((flags & _style) != _style) // if some style is not desired...
      throw E_MODE;                 // then it cannot be reduced
    if (c->arg_count() < _arity)    // if no enough arguments...
      return TermPtr();             // then it cannot be reduced neither

    TermPtr a[MAX_C_ARGS], curr = type(), next;
    int fs [] = {CONS, BIND, ALL_META, ALL_PURE, ALL_CTXT,
		 ALL_NICE, ALL};

    for (unsigned int i = 0; i < _arity; ++i, curr = next) {
      unsigned int s = (_strict >> (i * BITS)) & MASK;
      assert (curr);

      if (ProjPtr p = CAST<Proj> (curr)) {
	curr = p->from();
	next = p->to();
      }

      if (Z == s)
	a[i] = c->arg(i);
      else
	a[i] = c->arg_reduce (i, fs[s] & flags, curr);

      assert (a[i]);
    }

    // extend to multi-arg case.

    TermPtr result = _entry (c,
			     a[0], a[1], a[2], a[3],
			     a[4], a[5], a[6], a[7]);

    if (_arity)
      c->pop (_arity);

    assert (result);
    return result;
  }

  //////////////////////////////////////////////////////////////////////////////
  IMP_TYPE (Lib, "LIB");

  Lib::Lib (const string &name)
    : _name (name)
  {
    _handle = dlopen (name.c_str(), RTLD_LAZY);
    if (0 == _handle)
      throw string (dlerror());
  }

  //////////////////////////////////////////////////////////////////////////////
  typedef int (*entry_t) (int, int, int, int, int, int, int, int);

  Extf::Extf (const string& name,
	      int           arity,
	      P(Lib)        lib,
	      TermPtr       type,
	      int           ret)
    : Func (type, name),
      _arity (arity),
      _retstyle (ret)
  {
    assert (arity <= MAX_C_ARGS);

    dlerror();                                  // clear previous error message
    _fun = dlsym (lib->handle(), name.c_str()); // get symbol from the library
    if (const char *e = dlerror())              // if there is any error...
      throw string (e);                         // throw exception

    for (ProjPtr p; p = CAST<Proj>(type); type = p->to())
      ;

    _rettype = type;
  }

  TermPtr
  Extf::reduce (Context* context, int flags, TermPtr expected)
  {
    if (ALL != flags)
      throw E_MODE;
    if (context->arg_count() < (unsigned int)_arity)
      return TermPtr();

    if (_arity) {
      int a[MAX_C_ARGS], p = 0;
      TermPtr curr = type(), next;

      for (int i = 0; i < _arity; ++i, curr = next) {
	assert (curr);

	if (ProjPtr p = CAST<Proj> (curr)) {
	  curr = p->from();
	  next = p->to();
	}

	TermPtr arg = context->arg_reduce (i, ALL, curr);
	P(CVal) v = TCAST<CVal> (arg);
	for (int j = 0; j < v->csize(); ++j) {
	  assert (p <= MAX_C_ARGS);
	  a[p++] = v->to_c (CVal::C, j);
	}
      }

      // extend to multi-arg case.

      int result = ((entry_t) _fun) (a[0], a[1], a[2], a[3], 
				     a[4], a[5], a[6], a[7]);
      context->pop (_arity);

      if (INT == _retstyle)
	return Int::create (result);
      else if (PTR == _retstyle)
	return CPtr::create ((void*) result, _rettype);
      else
	return VOID;
    }
    else {
      // if the arity is zero, then it means the target
      // is not a function, but simply a symbol (like
      // stdout). the pointer obtained by dlsym will be
      // the pointer to the actual symbol.

      assert (PTR == _retstyle);
      return CPtr::create (*((void**)_fun), _rettype);
    }
  }
};
