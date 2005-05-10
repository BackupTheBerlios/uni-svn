#include <context.hh>
#include <int.hh>
#include <opt.hh>
#include <seq.hh>
#include <style.hh>

namespace NAMESPACE
{
  TermPtr
  IdOpt::reduce (Context* context, int flags, TermPtr expected)
  {
    // the dummy 'id' is used only as an update point,
    // so when reduce, it does nothing except reduce the rand.
    if (context->arg_count() > 0) {
      TermPtr result = context->arg_reduce(0, flags);
      context->pop();
      return result;
    }
    else
      return TermPtr();
  }

//   TermPtr
//   Let::reduce (Context* context, int flags, TermPtr expected)
//   {
//     return body()->sub (_var, exp());
//   }
};
