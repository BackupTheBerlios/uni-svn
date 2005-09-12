#ifndef _UNI_REFLECTION_HH_
#define _UNI_REFLECTION_HH_

#include <term.hh>

namespace NAMESPACE
{
  TermPtr retval (Machine* machine, TermPtr term);
  TermPtr retpop (Machine* machine, TermPtr term);

  TermPtr as (Machine* machine, TermPtr type, TermPtr term);
  TermPtr sa (Machine* machine, TermPtr term);

  TermPtr lambda (Machine* machine, TermPtr var, TermPtr body);
  TermPtr lambdax (Machine* machine, TermPtr var, TermPtr body);
  TermPtr lambdas (Machine* machine, TermPtr var, TermPtr body);

  TermPtr deflib (Machine* machine, TermPtr name, TermPtr path);
  TermPtr defext (Machine* machine, TermPtr name, TermPtr type,
		  TermPtr arity, TermPtr cname, TermPtr lib, TermPtr ret);

  TermPtr new_ns (Machine* machine);
  TermPtr get_ns (Machine* machine, TermPtr t_ns, TermPtr t_name);
  TermPtr set_ns (Machine* machine, TermPtr t_ns);

  TermPtr crec (Machine* machine, TermPtr size);
  TermPtr cmem (Machine* machine, TermPtr type, TermPtr size);

  TermPtr subs (Machine* machine, TermPtr from, TermPtr to, TermPtr term);

  TermPtr solve (Machine* machine, TermPtr var, TermPtr lhs, TermPtr rhs);

  TermPtr bind (Machine* machine, TermPtr term);
  TermPtr redsh  (Machine* machine, TermPtr term);
  TermPtr redshx (Machine* machine, TermPtr term, TermPtr exit);
//   TermPtr red (Machine* machine, TermPtr term, TermPtr t_flags);

  TermPtr is_sub (Machine* machine, TermPtr sub, TermPtr super);

  TermPtr get_special (Machine* machine, TermPtr name);
  TermPtr set_special (Machine* machine, TermPtr name, TermPtr value);
  TermPtr set_attr (Machine* machine, TermPtr name, TermPtr value);

  TermPtr get_slot (Machine* machine, TermPtr id);
  TermPtr set_slot (Machine* machine, TermPtr id, TermPtr val);

  TermPtr scope_pushx (Machine* machine, TermPtr t_scope);
  TermPtr scope_push (Machine* machine);
  TermPtr scope_popx (Machine* machine);
  TermPtr scope_pop (Machine* machine);
  TermPtr scope_set (Machine* c, TermPtr t_flags);

  TermPtr ifeq (Machine* machine, TermPtr lhs, TermPtr rhs, TermPtr t, TermPtr f);

  TermPtr tok_str (Machine* machine, TermPtr t_name);
  TermPtr tok_sym (Machine* machine, TermPtr t_name);

  TermPtr raise (Machine* machine, TermPtr t_id, TermPtr term1, TermPtr term2, TermPtr term3);
};

#endif
