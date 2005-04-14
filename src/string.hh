#ifndef _UNI_STRING_HH_
#define _UNI_STRING_HH_

#include <cval.hh>
#include <term.hh>

#include <string.h>

namespace NAMESPACE
{
  class Str : public Term,
	      public CVal
  {
    DEF_TERM        (Str);
    DEF_VISITABLE   (Str,   str);

  public:

    virtual int to_c (int style, int id) const;
    virtual void from_c (int style, int val);

    const char* str () const { assert (_str); return _str; }

  protected:

    virtual bool xequ (TermPtr rhs) const;

  protected:

    Str (const char* str) : _str(const_cast<char*> (str)) { assert (str); }

    char* _str;
  };

  class MStr : public Str
  {
    DEF_TERMLET (MStr, Str);

  protected:

    MStr (const MStr& mstr) : Str (strdup (mstr._str)) { }
    MStr (const char* str) : Str (strdup (str)) { }
    MStr (const string& str) : Str (strdup (str.c_str())) { }

  public:

    ~MStr () { if (_str) free (_str); }
  };
};

#endif
