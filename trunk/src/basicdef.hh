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

#ifndef _UNI_BASICDEF_HH_
#define _UNI_BASICDEF_HH_

////////////////////////////////////////////////////////////////////////////////
#define VAL_PROPERTY(TYPE,NAME) \
protected:\
TYPE _ ## NAME; \
public:\
TYPE NAME () const { return _ ## NAME; } \
TYPE NAME () { return _ ## NAME; } \
void NAME ( TYPE x ) { _ ## NAME = x; }

////////////////////////////////////////////////////////////////////////////////
#define VAL_PROPERTY_RO(TYPE,NAME) \
protected:\
TYPE _ ## NAME; \
public:\
TYPE NAME () const { return _ ## NAME; } \

////////////////////////////////////////////////////////////////////////////////
#define REF_PROPERTY(TYPE,NAME) \
protected:\
TYPE _ ## NAME; \
public:\
const TYPE& NAME () const { return _ ## NAME; } \
TYPE& NAME () { return _ ## NAME; } \
void NAME ( const TYPE& x ) { _ ## NAME = x; }


////////////////////////////////////////////////////////////////////////////////
#define VIRTUAL_VAL_PROPERTY(TYPE,NAME) \
public:\
virtual TYPE NAME () const = 0; \
virtual TYPE NAME () = 0; \
virtual void NAME ( TYPE x ) = 0;

////////////////////////////////////////////////////////////////////////////////
#define VIRTUAL_VAL_PROPERTY_RO(TYPE,NAME) \
public:\
virtual TYPE NAME () const = 0; \
virtual TYPE NAME () = 0;

////////////////////////////////////////////////////////////////////////////////
#define VIRTUAL_REF_PROPERTY(TYPE,NAME) \
public:\
const TYPE& NAME () const = 0; \
TYPE& NAME () = 0; \
void NAME ( const TYPE& x ) = 0;

////////////////////////////////////////////////////////////////////////////////
#define VAL_FLAGS(TYPE,NAME) \
public:\
TYPE NAME () { return _ ## NAME; } \
protected: \
TYPE _ ## NAME;

////////////////////////////////////////////////////////////////////////////////
#define DEF_TAG(TYPE) public: virtual const string tag () const { return #TYPE; }

////////////////////////////////////////////////////////////////////////////////
#define DEF_TYPEONLY() public: static const TermPtr T;

////////////////////////////////////////////////////////////////////////////////
#define DEF_TYPE() \
public: \
static const TermPtr T; \
static TermPtr TYPE () { return T; }

////////////////////////////////////////////////////////////////////////////////
#define IMP_TYPE(CLASS, NAME) \
const TermPtr CLASS::T = Type::create (NAME);

////////////////////////////////////////////////////////////////////////////////
#define __CREATE__(t) \
p->_ptr = p; if (! p->_type) p->_type = t::TYPE(); assert (p->_type); return p;

////////////////////////////////////////////////////////////////////////////////
#define DEF_CREATE(t) \
public: \
static shared_ptr<t> create() \
{ shared_ptr<t> p(new t);  __CREATE__(t); } \
\
template <typename T1> \
static shared_ptr<t> create(T1 p1) \
{ shared_ptr<t> p(new t(p1)); __CREATE__(t); } \
\
template <typename T1, typename T2> \
static shared_ptr<t> create(T1 p1, T2 p2) \
{ shared_ptr<t> p(new t(p1,p2)); __CREATE__(t); } \
\
template <typename T1, typename T2, typename T3> \
static shared_ptr<t> create(T1 p1, T2 p2, T3 p3) \
{ shared_ptr<t> p(new t(p1,p2,p3)); __CREATE__(t); } \
\
template <typename T1, typename T2, typename T3, typename T4> \
static shared_ptr<t> create(T1 p1, T2 p2, T3 p3, T4 p4) \
{ shared_ptr<t> p(new t(p1,p2,p3,p4)); __CREATE__(t); } \
\
template <typename T1, typename T2, typename T3, typename T4, typename T5> \
static shared_ptr<t> create(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5) \
{ shared_ptr<t> p(new t(p1,p2,p3,p4,p5)); __CREATE__(t); } \
\
template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6> \
static shared_ptr<t> create(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6) \
{ shared_ptr<t> p(new t(p1,p2,p3,p4,p5,p6)); __CREATE__(t); }

////////////////////////////////////////////////////////////////////////////////
#define __TERM__(t) \
DEF_CREATE(t); \
public: \
virtual TermPtr clone () const { return t::create (*this); } \
virtual TermPtr clone (TermPtr type) const { shared_ptr<t> p = t::create (*this); p->_type = _type; return p; } \

////////////////////////////////////////////////////////////////////////////////
#define DEF_STATIC(t) \
DEF_TAG(); \
DEF_TYPE(); \
public: \
virtual TermPtr type () const { return t::T; } \
virtual TermPtr clone () const { return t::create (*this); } \
virtual TermPtr clone (TermPtr type) const { assert (false); return TermPtr(); } \
public: \
template <typename T1> \
static shared_ptr<t> create(T1 p1) \
{ shared_ptr<t> p(new t(p1)); p->_ptr = p; return p; }

////////////////////////////////////////////////////////////////////////////////
#define DEF_DYNAMIC(t) \
DEF_TAG(t); \
public: \
static TermPtr TYPE () { return TermPtr(); } \
__TERM__(t);

////////////////////////////////////////////////////////////////////////////////
#define DEF_TERMLETX(t,u) \
DEF_TAG(t); \
public: \
static TermPtr TYPE () { return u::T; } \
__TERM__(t);

////////////////////////////////////////////////////////////////////////////////
#define DEF_TERMX(t) \
DEF_TAG(t); \
DEF_TYPE(); \
__TERM__(t);

////////////////////////////////////////////////////////////////////////////////
#define DEF_TERMLET(t,u) \
DEF_TERMLETX(t,u); \
protected: \
t () { }

////////////////////////////////////////////////////////////////////////////////
#define DEF_TERM(t) \
DEF_TERMX(t); \
protected: \
t () { }

////////////////////////////////////////////////////////////////////////////////
#define DEF_VISITABLE(TYPE,SUFFIX) \
public: virtual void visit (Visitor& visitor) \
{ visitor.visit_ ## SUFFIX (CAST<TYPE> (SELF)); }

////////////////////////////////////////////////////////////////////////////////
#define SELF (_ptr.lock())

////////////////////////////////////////////////////////////////////////////////
#define TERM_TREEMAP(N,I) \
public: \
TermPtr N () const { return elem(I); } \
void N (TermPtr term) { elem(I,term); }

////////////////////////////////////////////////////////////////////////////////
#define NAMESPACE uni
#define EXPORT

#ifndef DEBUG
#define DBG(x)
#else
#define DBG(x)    x
#endif

#endif
