#ifndef _UNI_UTILS_HH_
#define _UNI_UTILS_HH_

#include <basicdef.hh>
#include <typedef.hh>

namespace NAMESPACE
{
  class Context;

  TermPtr add_i (Context* context, TermPtr left, TermPtr right);
  TermPtr sub_i (Context* context, TermPtr left, TermPtr right);
  TermPtr mul_i (Context* context, TermPtr left, TermPtr right);
  TermPtr div_i (Context* context, TermPtr left, TermPtr right);
  TermPtr mod_i (Context* context, TermPtr left, TermPtr right);
  TermPtr equ_i (Context* context, TermPtr left, TermPtr right);
  TermPtr less_i (Context* context, TermPtr left, TermPtr right);
  TermPtr greater_i (Context* context, TermPtr left, TermPtr right);

  TermPtr concat (Context* context, TermPtr left, TermPtr right);
  TermPtr raw_print (Context* context, TermPtr str);
  TermPtr str (Context* context, TermPtr term);
  TermPtr str_ansi (Context* context, TermPtr term);
  TermPtr cstr (Context* context, TermPtr src);

  TermPtr cast_str (Context* context, TermPtr term);
};

#endif
