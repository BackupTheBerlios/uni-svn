#ifndef _UNI_REFLECTION_HH_
#define _UNI_REFLECTION_HH_

#include <term.hh>

namespace NAMESPACE
{
  TermPtr retval (Context* context, TermPtr term);
  TermPtr retpop (Context* context, TermPtr term);

  TermPtr as (Context* context, TermPtr type, TermPtr term);
  TermPtr sa (Context* context, TermPtr term);

  TermPtr cons (Context* context, TermPtr left, TermPtr right);
  TermPtr consh (Context* context, TermPtr left, TermPtr right);

  TermPtr head (Context* context, TermPtr cons);
  TermPtr tail (Context* context, TermPtr cons);

  TermPtr headh (Context* context, TermPtr consh);
  TermPtr tailh (Context* context, TermPtr consh);

  TermPtr proj (Context* context, TermPtr left, TermPtr right);
  TermPtr lambda (Context* context, TermPtr var, TermPtr body);
  TermPtr lambdax (Context* context, TermPtr var, TermPtr body);
  TermPtr lambdas (Context* context, TermPtr var, TermPtr body);

  TermPtr defsty (Context* context, TermPtr name, TermPtr style, TermPtr prec);
  TermPtr defout (Context* context, TermPtr left, TermPtr right);
  TermPtr defmap (Context* context, TermPtr name, TermPtr term);
  TermPtr define (Context* context, TermPtr name, TermPtr type, TermPtr body);
  TermPtr defaty (Context* context, TermPtr name);
  TermPtr deftmp (Context* context, TermPtr name, TermPtr var, TermPtr type, TermPtr body);
  TermPtr deflib (Context* context, TermPtr name, TermPtr path);
  TermPtr defext (Context* context, TermPtr name, TermPtr type,
		  TermPtr arity, TermPtr cname, TermPtr lib, TermPtr ret);

  TermPtr undef (Context* context, TermPtr name);
  TermPtr undefx (Context* context, TermPtr t_nspace, TermPtr t_name);

  TermPtr redef (Context* context, TermPtr t_name, TermPtr term);

  TermPtr new_ns (Context* context);
  TermPtr get_ns (Context* context, TermPtr t_ns, TermPtr t_name);
  TermPtr set_ns (Context* context, TermPtr t_ns);

  TermPtr crec (Context* context, TermPtr size);
  TermPtr cmem (Context* context, TermPtr type, TermPtr size);

  TermPtr subs (Context* context, TermPtr from, TermPtr to, TermPtr term);

  TermPtr solve (Context* context, TermPtr var, TermPtr lhs, TermPtr rhs);

  TermPtr bind (Context* context, TermPtr term);
  TermPtr redsh  (Context* context, TermPtr term);
  TermPtr redshx (Context* context, TermPtr term, TermPtr exit);
  TermPtr red (Context* context, TermPtr term, TermPtr t_flags);

  TermPtr is_sub (Context* context, TermPtr sub, TermPtr super);
  TermPtr type_of (Context* context, TermPtr term);
  TermPtr proj (Context* context, TermPtr from, TermPtr to);

  TermPtr type (Context* context, TermPtr name);
  TermPtr typetmp (Context* context, TermPtr body, TermPtr param);

  TermPtr get_special (Context* context, TermPtr name);
  TermPtr set_special (Context* context, TermPtr name, TermPtr value);
  TermPtr set_attr (Context* context, TermPtr name, TermPtr value);

  TermPtr get_slot (Context* context, TermPtr id);
  TermPtr set_slot (Context* context, TermPtr id, TermPtr val);

  TermPtr scope_pushx (Context* context, TermPtr t_scope);
  TermPtr scope_push (Context* context);
  TermPtr scope_popx (Context* context);
  TermPtr scope_pop (Context* context);
  TermPtr scope_set (Context* c, TermPtr t_flags);

  TermPtr import (Context* c, TermPtr name);

  TermPtr set_int (Context* c, TermPtr v, TermPtr val);
  TermPtr set_str (Context* c, TermPtr v, TermPtr val);

  TermPtr ifeq (Context* context, TermPtr lhs, TermPtr rhs, TermPtr t, TermPtr f);

  TermPtr cons_t2 (Context* context, TermPtr term);

  TermPtr dict_new (Context* context, TermPtr type);
  TermPtr dict_has (Context* context, TermPtr t_dict, TermPtr t_name);
  TermPtr dict_get (Context* context, TermPtr t_dict, TermPtr t_name);
  TermPtr dict_set (Context* context, TermPtr t_dict, TermPtr t_name, TermPtr term);
  TermPtr dict_vis (Context* context, TermPtr t_dict, TermPtr t_visitor);

  TermPtr list_new (Context* context, TermPtr type);
  TermPtr list_ins (Context* context, TermPtr t_list, TermPtr term);
  TermPtr list_add (Context* context, TermPtr t_list, TermPtr term);
  TermPtr list_vis (Context* context, TermPtr t_list, TermPtr t_visitor);

  TermPtr set (Context* context, TermPtr t_ref, TermPtr term);

  TermPtr tok_str (Context* context, TermPtr t_name);
  TermPtr tok_sym (Context* context, TermPtr t_name);

  TermPtr raise (Context* context, TermPtr t_id, TermPtr term1, TermPtr term2, TermPtr term3);
};

#endif
