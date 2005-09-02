#ifndef _UNI_UTILS_HH_
#define _UNI_UTILS_HH_

#include <basicdef.hh>
#include <typedef.hh>

namespace NAMESPACE
{
  class Machine;

  TermPtr add_i (Machine* machine, TermPtr left, TermPtr right);
  TermPtr sub_i (Machine* machine, TermPtr left, TermPtr right);
  TermPtr mul_i (Machine* machine, TermPtr left, TermPtr right);
  TermPtr div_i (Machine* machine, TermPtr left, TermPtr right);
  TermPtr mod_i (Machine* machine, TermPtr left, TermPtr right);
  TermPtr equ_i (Machine* machine, TermPtr left, TermPtr right);
  TermPtr less_i (Machine* machine, TermPtr left, TermPtr right);
  TermPtr greater_i (Machine* machine, TermPtr left, TermPtr right);

  TermPtr concat (Machine* machine, TermPtr left, TermPtr right);
  TermPtr raw_print (Machine* machine, TermPtr str);
  TermPtr str (Machine* machine, TermPtr term);
  TermPtr str_ansi (Machine* machine, TermPtr term);
  TermPtr cstr (Machine* machine, TermPtr src);

  TermPtr cast_str (Machine* machine, TermPtr term);
};

#endif
