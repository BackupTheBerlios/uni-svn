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
    Func (TermPtr type) : Term (type), _name ("*") { }
  };

  class Envf : public Func
  {
    DEF_DYNAMIC  (Envf);
    VAL_PROPERTY (unsigned int, arity);
    VAL_PROPERTY (unsigned int, style);

  public:

    enum { MACH_PTR = 1 };

    virtual TermPtr reduce (Machine* machine, int flags, TermPtr expected);

    Envf (unsigned int  arity,
	  unsigned int  style,
	  void*         entry,
	  TermPtr       type,
	  unsigned int  flags,
	  ...);

  protected:

    Envf () { assert (false); }

  private:

    typedef TermPtr (*_envf_t)(Machine*,TermPtr,TermPtr,TermPtr,TermPtr,TermPtr,TermPtr,TermPtr,TermPtr);
    typedef TermPtr (*_simf_t)(TermPtr,TermPtr,TermPtr,TermPtr,TermPtr,TermPtr,TermPtr,TermPtr);

    void*        _entry;
    unsigned int _flags;
    vector<int>  _arg_styles;
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
    Intf (TermPtr body, TermPtr type);

  private:

    bool _proj;
  };

  class Lib : public Term
  {
    DEF_TERM        (Lib);
    REF_PROPERTY    (string, name);
    VAL_PROPERTY_RO (void*,  handle);

  protected:

    Lib (const string& name);
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
