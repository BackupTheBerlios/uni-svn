#ifndef _UNI_CVAL_HH_
#define _UNI_CVAL_HH_

#include <term.hh>

namespace NAMESPACE
{
  class CVal
  {
  public:

    enum { C, CC };

    static const TermPtr T;

  public:

    virtual int csize () const { return 1; }
    virtual int to_c (int style, int id) const = 0;
    virtual void from_c (int style, int val) = 0;
  };

  class CPtr : public Term,
	       public CVal
  {
    DEF_DYNAMIC   (CPtr);
    DEF_VISITABLE (CPtr, cptr);
    VAL_PROPERTY  (void*, val);

  public:

    virtual int to_c (int style, int id) const { return (int)_val; }
    virtual void from_c (int style, int val) { assert (false); }

  protected:

    CPtr () { assert (false); }
    CPtr (void* ptr, TermPtr type) : Term (type), _val (ptr) { }
  };

  class CRec : public Term,
	       public CVal
  {
    DEF_TERMLETX (CRec, Term);

  public:

    virtual int csize () const { return _size; }
    virtual int to_c (int style, int id) const;
    virtual void from_c (int style, int val) { assert (false); }

  protected:

    CRec () { assert (false); }
    CRec (int size) : _size (size), _rec (new int [size]) { }

  private:

    int  _size;
    int* _rec;
  };

  class CMem : public Term,
	       public CVal
  {
    DEF_DYNAMIC (CMem);

    VAL_PROPERTY_RO (int, size);
    VAL_PROPERTY_RO (void*, buf);

  public:

    virtual int to_c (int style, int id) const { return (int) _buf; }
    virtual void from_c (int style, int buf) { _buf = (char*) buf; }

  protected:

    CMem () { assert (false); }
    CMem (const CMem& cmem);
    CMem (TermPtr type, int size) : _size (size), _buf (new char [size]) { _type = type; }
    CMem (TermPtr type, int size, void* buf) : _size (size), _buf (buf) { _type = type; }

  public:

    ~CMem ();
  };
};

#endif
