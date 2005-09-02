#ifndef _UNI_TOK_HH_
#define _UNI_TOK_HH_

#include <raw.hh>
#include <string.hh>

namespace NAMESPACE
{
  class Tok : public MStr
  {
    DEF_TERMLET   (Tok, Raw);
    DEF_VISITABLE (Tok, tok);

  public:

    virtual TermPtr reduce (Machine* machine, int flags, TermPtr expected);

  protected:

    virtual bool xequ (TermPtr rhs) const;

    Tok (const char* name, bool special = false) : MStr (name), _spec (special) { }

  private:

    bool _spec;
  };
};

#endif
