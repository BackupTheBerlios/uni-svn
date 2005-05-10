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

    virtual TermPtr reduce (Context* context, int flags, TermPtr expected);

  protected:

    virtual bool xequ (TermPtr rhs) const;

    Tok (const char* name) : MStr (name) { }
  };
};

#endif
