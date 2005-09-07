#ifndef _UNI_REFLECTION_HH_
#define _UNI_REFLECTION_HH_

#include <term.hh>

namespace NAMESPACE
{
  TermPtr retval (Machine* machine, TermPtr term);
  TermPtr retpop (Machine* machine, TermPtr term);

  TermPtr as (Machine* machine, TermPtr type, TermPtr term);
  TermPtr sa (Machine* machine, TermPtr term);

  TermPtr cons (Machine* machine, TermPtr left, TermPtr right);
  TermPtr consh (Machine* machine, TermPtr left, TermPtr right);

  TermPtr head (Machine* machine, TermPtr cons);
  TermPtr tail (Machine* machine, TermPtr cons);

  TermPtr headh (Machine* machine, TermPtr consh);
  TermPtr tailh (Machine* machine, TermPtr consh);

  TermPtr proj (Machine* machine, TermPtr left, TermPtr right);
  TermPtr lambda (Machine* machine, TermPtr var, TermPtr body);
  TermPtr lambdax (Machine* machine, TermPtr var, TermPtr body);
  TermPtr lambdas (Machine* machine, TermPtr var, TermPtr body);

  TermPtr defsty (Machine* machine, TermPtr name, TermPtr style, TermPtr prec);
  TermPtr defout (Machine* machine, TermPtr left, TermPtr right);
  TermPtr defmap (Machine* machine, TermPtr name, TermPtr term);
  TermPtr define (Machine* machine, TermPtr name, TermPtr type, TermPtr body);
  TermPtr defaty (Machine* machine, TermPtr name);
  TermPtr deftmp (Machine* machine, TermPtr name, TermPtr var, TermPtr type, TermPtr body);
  TermPtr deflib (Machine* machine, TermPtr name, TermPtr path);
  TermPtr defext (Machine* machine, TermPtr name, TermPtr type,
		  TermPtr arity, TermPtr cname, TermPtr lib, TermPtr ret);

  TermPtr undef (Machine* machine, TermPtr name);
  TermPtr undefx (Machine* machine, TermPtr t_nspace, TermPtr t_name);

  TermPtr redef (Machine* machine, TermPtr t_name, TermPtr term);

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
  TermPtr type_of (Machine* machine, TermPtr term);
  TermPtr proj (Machine* machine, TermPtr from, TermPtr to);

  TermPtr type (Machine* machine, TermPtr name);
  TermPtr typetmp (Machine* machine, TermPtr body, TermPtr param);

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

  TermPtr set_int (Machine* c, TermPtr v, TermPtr val);
  TermPtr set_str (Machine* c, TermPtr v, TermPtr val);

  TermPtr ifeq (Machine* machine, TermPtr lhs, TermPtr rhs, TermPtr t, TermPtr f);

  TermPtr cons_t2 (Machine* machine, TermPtr term);

//   TermPtr set (Machine* machine, TermPtr t_ref, TermPtr term);

  TermPtr tok_str (Machine* machine, TermPtr t_name);
  TermPtr tok_sym (Machine* machine, TermPtr t_name);

  TermPtr raise (Machine* machine, TermPtr t_id, TermPtr term1, TermPtr term2, TermPtr term3);
};

#endif
