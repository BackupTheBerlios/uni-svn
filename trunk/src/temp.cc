#include <app.hh>
#include <cons.hh>
#include <machine.hh>
#include <exception.hh>
#include <proj.hh>
#include <temp.hh>

namespace NAMESPACE
{
  Temp::Temp (const string& name, TermPtr var, TermPtr type, TermPtr body)
    : _name (name),
      _type_temp (type),
      _body_temp (body)
  {
    if (ConsPtr cons = CAST<Cons> (var)) {
      for ( ; cons && NIL != cons; cons = CAST<Cons> (cons->right()))
	_unknowns.push_back (cons->left());
    }
    else
      _unknowns.push_back (var);
  }

  Temp::Temp (const string& name, TermVec& vars, TermPtr type, TermPtr body)
    : _name (name),
      _unknowns (vars),
      _type_temp (type),
      _body_temp (body)
  {

  }

  TermPtr
  Temp::reduce (Machine* c, int flags, TermPtr expected)
  {
    if (0 == c->arg_count())
      return TermPtr();

    TermMap  solutions;

    ProjPtr proj = CAST<Proj> (_type_temp);
    for (unsigned int i = 0; proj; ++i, proj = CAST<Proj>(proj->to())) {
      TermPtr arg  = c->arg_reduce (i, BIND);
      TermPtr got  = arg->type();
      TermPtr from = proj->from();

      try {
	from->msolve (_unknowns, got, solutions);
      }
      catch (TermPtr et) {
	if (ExcPtr e = CAST<Exc>(et)) {
	  if (E_INCONSISTENT == e->id())
	    return TermPtr();
	  else if (E_FREEVAR == e->id())
	    continue;
	}
	else
	  throw;
      }
    }

    if (_unknowns.size() != solutions.size())
      return TermPtr();
    else {
      TermPtr t = _type_temp;
      TermPtr b = _body_temp;
      for (TermMap::iterator i = solutions.begin(); i != solutions.end(); ++i) {
	t = t->sub (i->first, i->second);
 	b = b->sub (i->first, i->second);
      }

      if (flags & (~BIND))
	return b;
      else
	return Instance::create (t, b);
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  TermPtr
  Instance::reduce (Machine* c, int flags, TermPtr expected)
  {
    if ((flags & BIND) && (flags != int(BIND)))
      return body();
    else
      throw E_MODE;
  }
};
