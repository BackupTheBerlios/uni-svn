#include <machine.hh>
#include <int.hh>
#include <opt.hh>
#include <seq.hh>
#include <style.hh>

namespace NAMESPACE
{
  TermPtr
  IdOpt::reduce (Machine* machine, int flags, TermPtr expected)
  {
    // the dummy 'id' is used only as an update point,
    // so when reduce, it does nothing except reduce the rand.
    if (machine->arg_count() > 0) {
      TermPtr result = machine->arg_reduce(0, flags);
      machine->pop();
      return result;
    }
    else
      return TermPtr();
  }

//   TermPtr
//   Let::reduce (Machine* machine, int flags, TermPtr expected)
//   {
//     return body()->sub (_var, exp());
//   }
};
