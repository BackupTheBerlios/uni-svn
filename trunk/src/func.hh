/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _UNI_FUNC_HH_
#define _UNI_FUNC_HH_

#include <term.hh>

namespace NAMESPACE
{
  class Func : public Term
  {
    REF_PROPERTY  (string, name);
    DEF_VISITABLE (Func, func);
    VAL_PROPERTY  (TermPtr, successor);

  public:

    Func () { assert (false); }
    Func (TermPtr type, const string& name) : Term (type), _name (name) { }
  };

  class SimpleFunc : public Func
  {
    DEF_DYNAMIC  (SimpleFunc);
    VAL_PROPERTY (unsigned int, arity);
    VAL_PROPERTY (unsigned int, style);

  public:

    virtual TermPtr reduce (Machine* machine, int flags, TermPtr expected);

  protected:

    SimpleFunc () { }

    SimpleFunc (unsigned int  arity,
	        unsigned int  style,
		void*         entry,
		TermPtr       type);

  private:

    typedef TermPtr (*_entry_type)(TermPtr,TermPtr,TermPtr,TermPtr,TermPtr,TermPtr,TermPtr,TermPtr);

    _entry_type  _entry;
  };

  class Envf : public Func
  {
    DEF_DYNAMIC  (Envf);
    VAL_PROPERTY (unsigned int, arity);
    VAL_PROPERTY (unsigned int, style);
    VAL_PROPERTY (unsigned int, strict);

  public:

    enum {B = 1, M = 2, P = 3, C = 4, N = 5, S = 6, Z = 7};
    enum {BITS = 3, MASK = 7};

  public:

    virtual TermPtr reduce (Machine* machine, int flags, TermPtr expected);

  protected:

    Envf () { }

    Envf (const string& name,
	  unsigned int  arity,
	  unsigned int  style,
	  unsigned int  strictness,
	  void*         entry,
	  TermPtr       type);

  private:

    typedef TermPtr (*_entry_type)(Machine*,TermPtr,TermPtr,TermPtr,TermPtr,TermPtr,TermPtr,TermPtr,TermPtr);

    _entry_type  _entry;
  };

  class Lib : public Term
  {
    DEF_TERM        (Lib);
    REF_PROPERTY    (string, name);
    VAL_PROPERTY_RO (void*,  handle);

  protected:

    Lib (const string& name);
  };

  /**
   * Intf, or internal functions are the class for functions
   * that is defined using the language.
   */
  class Intf : public Func
  {
    DEF_DYNAMIC  (Intf);
    VAL_PROPERTY (TermPtr, body);

  public:

    virtual TermPtr reduce (Machine* c, int flags, TermPtr expected);

  protected:

    Intf () { assert (false); }
    Intf (const std::string& name, TermPtr body, TermPtr type);

  private:

    bool _proj;
  };

  class Extf : public Func
  {
    DEF_DYNAMIC  (Extf);
    VAL_PROPERTY (int,     arity);
    VAL_PROPERTY (int,     retstyle);
    VAL_PROPERTY (TermPtr, rettype);

  public:

    virtual TermPtr reduce (Machine* machine, int flags, TermPtr expected);

  public:

    enum { INT, PTR, NONE };

  protected:

    Extf () { }

    Extf (const string& name,
	  int           arity,
	  P(Lib)        lib,
	  TermPtr       type,
	  int           rettype);

  private:

    void* _fun;
  };
};

#endif
