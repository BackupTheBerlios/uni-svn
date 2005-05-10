#ifndef _UNI_HANDLER_HH_
#define _UNI_HANDLER_HH_

#include <basicdef.hh>
#include <term.hh>
#include <typedef.hh>
#include <visitor.hh>

#include <iostream>

namespace NAMESPACE
{
  class Context;

  class ImportHandler
  {
  public:

    virtual ~ImportHandler () { }
    virtual bool import (Context* context, const std::string& name) = 0;
  };

  class DebugHandler
  {
  public:

    virtual ~DebugHandler () { }
    virtual void step_break (Context* context, TermPtr term) = 0;
  };

  class ViewHandler : public Visitor
  {
  public:

    ViewHandler (std::ostream& os) : _os(os) { }
    virtual ~ViewHandler () { }

  public:

    ViewHandler& operator << (TermPtr term) { term->visit (*this); return *this; }
    ViewHandler& operator << (const string& x) { _os << x; return *this; }
    ViewHandler& operator << (char x) { _os << x; return *this; }
    ViewHandler& operator << (int x) { _os << x; return *this; }
    ViewHandler& operator << (unsigned int x) { _os << x; return *this; }

  private:

    std::ostream& _os;
  };
};

#endif
