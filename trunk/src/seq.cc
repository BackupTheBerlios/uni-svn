#include <machine.hh>
#include <raw.hh>
#include <seq.hh>
#include <style.hh>
#include <type.hh>

namespace NAMESPACE
{
  IMP_TYPE (Ret, "RET");

  bool
  Seq::xequ (TermPtr rhs) const
  {
    if (SeqPtr r = CAST<Seq>(rhs)) {
      if (size() == r->size()) {
	for (unsigned int i = size(); i < size(); ++i) {
	  if (! (*this)[i]->equ ((*r)[i]))
	    return false;
	}
      }
      return true;
    }

    return false;
  }

  bool
  Seq::xfreeof (TermPtr term) const
  {
    for (const_iterator i = begin(); i != end(); ++i)
      if ((*i)->freeof (term))
	return false;

    return true;
  }

  TermPtr
  Seq::xsub (TermPtr var, TermPtr sub) const
  {
    for (const_iterator i = begin(); i != end(); ++i) {
      TermPtr e = (*i)->sub (var, sub);
      if (e != (*i)) {
	SeqPtr seq = Seq::create();
	for (const_iterator j = begin(); j != i; ++j)
	  seq->append (*j);
	seq->append (e);
	for (++i; i < end(); ++i)
	  seq->append ((*i)->sub (var, sub));
	return seq;
      }
    }

    return SELF;
  }

  TermPtr
  Seq::reduce (Machine* machine, int flags, TermPtr expected)
  {
    if (ALL_CTXT != (flags & ALL_CTXT))
      throw E_MODE;

    for (iterator i = begin(); i != end(); ++i) {
      if (RawPtr raw = CAST<Raw> (*i)) {
	for (RawPtr next; next = raw->deoutfix (machine); raw = next) {
	  raw->construct (machine, CONS);
	  if (RetPtr ret = CAST<Ret> (machine->reduce (raw, flags)))
	    return ret->value();
	}

	raw->construct (machine, CONS);
	if (RetPtr ret = CAST<Ret> (machine->reduce (raw, flags)))
	  return ret->value();
      }
      else {
	TermPtr r = machine->reduce (*i, flags);
	assert (r);
	if (RetPtr ret = CAST<Ret> (r))
	  return ret->value();
      }
    }

    return VOID;
  }
};
