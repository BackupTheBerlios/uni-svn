#include <cval.hh>
#include <type.hh>

namespace NAMESPACE
{
  const TermPtr CVal::T = Type::create ("CVAL");

  int
  CRec::to_c (int style, int id) const
  {
    if (id >= _size)
      throw 0;
    return _rec [id];
  }

  //// CMem ////////////////////////////////////////////////////////////////////
  CMem::CMem (const CMem& cmem)
    : _size (cmem._size),
      _buf  (new char [cmem._size])
  {
    memcpy (_buf, cmem._buf, cmem._size);
  }

  CMem::~CMem ()
  {
    assert (_buf);
    delete[] (char*) _buf;
  }
};
