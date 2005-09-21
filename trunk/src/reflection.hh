#ifndef _UNI_REFLECTION_HH_
#define _UNI_REFLECTION_HH_

#include <term.hh>

namespace NAMESPACE
{
  TermPtr deflib (Machine* machine, TermPtr name, TermPtr path);
  TermPtr defext (Machine* machine, TermPtr name, TermPtr type,
		  TermPtr arity, TermPtr cname, TermPtr lib, TermPtr ret);

//   TermPtr crec (Machine* machine, TermPtr size);
//   TermPtr cmem (Machine* machine, TermPtr type, TermPtr size);

  TermPtr bind (Machine* machine, TermPtr term);
  TermPtr redsh  (Machine* machine, TermPtr term);
  TermPtr redshx (Machine* machine, TermPtr term, TermPtr exit);
//   TermPtr red (Machine* machine, TermPtr term, TermPtr t_flags);

  TermPtr get_special (Machine* machine, TermPtr name);
  TermPtr set_special (Machine* machine, TermPtr name, TermPtr value);
//   TermPtr set_attr (Machine* machine, TermPtr name, TermPtr value);

//   TermPtr ifeq (Machine* machine, TermPtr lhs, TermPtr rhs, TermPtr t, TermPtr f);
};

#endif
