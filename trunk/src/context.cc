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

#include <family.hh>
#include <context.hh>
#include <style.hh>
#include <type.hh>

namespace NAMESPACE
{
  IMP_TYPE (Space, "SPACE");
  IMP_TYPE (Scope, "SCOPE");

  void
  symbols_t::add (const string& sym, SpacePtr nspace, TermPtr term)
  {
    iterator iter = find (make_pair (sym, nspace));
    if (end() != iter)
      iter->second->add (term, term->type());
    else {
      FamPtr family = Family::create ();
      family->add (term, term->type());
      (*this) [make_pair (sym, nspace)] = family;
    }
  }

  bool
  symbols_t::del (const string& sym, SpacePtr nspace)
  {
    iterator iter = find (make_pair (sym, nspace));
    if (end() == iter)
      return false;
    else {
      erase (iter);
      return true;
    }
  }

  bool
  symbols_t::set (const std::string& sym, SpacePtr nspace, TermPtr term)
  {
    iterator iter = find (make_pair (sym, nspace));
    if (end() != iter) {
      FamPtr family = Family::create ();
      family->add (term, term->type());
      iter->second = family;
      return true;
    }

    return false;
  }

  TermPtr
  symbols_t::get (const std::string& sym, SpacePtr nspace) const
  {
    const_iterator iter = find (make_pair (sym, nspace));
    if (end() != iter)
      return iter->second;
    else
      return TermPtr();
  }

  void
  styles_t::add (const string& sym, unsigned int style)
  {
    (*this) [sym] = style;
  }

  int
  styles_t::get (const string& sym) const
  {
    const_iterator iter = find (sym);
    return end() == iter ? 0 : iter->second;
  }

  void
  pairs_t::add (const string& left, const string& right)
  {
    (*this) [left] = right;
  }

  bool
  pairs_t::has (const string& left, const string& right) const
  {
    const_iterator iter = find (left);
    return end() != iter && iter->second == right;
  }

  const string*
  pairs_t::get (const string& left) const
  {
    const_iterator iter = find (left);
    return end() == iter ? 0 : &(iter->second);
  }

  TermPtr
  slots_t::get (const string& name) const
  {
    const_iterator iter = find (name);
    return end() == iter ? TermPtr() : iter->second;
  }

  void
  slots_t::set (const string& name, TermPtr val)
  {
    (*this) [name] = val;
  }

  //// scope stack /////////////////////////////////////////////////////////////
  Context::reverse_iterator
  Context::_writable ()
  {
    for (reverse_iterator r = rbegin(); r != rend(); ++r)
      if (! ((*r)->flags & SET_TRANS))
	return r;

    assert (false);
  }

  void Context::push (int flags)
  {
    push_back (Scope::create());
    back()->nspace = SpacePtr();
    back()->flags  = flags;
  }

  void
  Context::add_symbol (TermPtr term, const string& sym, SpacePtr nspace)
  {
    reverse_iterator r = _writable();
    (*r)->symbols.add (sym, nspace ? nspace : (*r)->nspace, term);
  }

  bool
  Context::del_symbol (const string& sym, SpacePtr nspace)
  {
    for (reverse_iterator r = rbegin(); r != rend(); ++r)
      if ((*r)->symbols.del(sym, nspace))
	return true;

    return false;
  }

  bool
  Context::set_symbol (const string& sym, TermPtr term, SpacePtr nspace)
  {
    for (reverse_iterator r = rbegin(); r != rend(); ++r) {
      if ((*r)->symbols.set (sym, nspace, term))
	return true;
      else if ((*r)->flags & GET_OPAQUE)
	return false;
    }

    return false;
  }

  TermPtr
  Context::get_symbol (const string& sym, SpacePtr nspace) const
  {
    for (const_reverse_iterator r = rbegin(); r != rend(); ++r) {
      if (TermPtr result = (*r)->symbols.get (sym, nspace))
	return result;
      else if ((*r)->flags & GET_OPAQUE)
	return TermPtr();
    }

    return TermPtr ();
  }

  void
  Context::add_style (const string& sym, unsigned int style)
  {
    (*_writable())->styles.add (sym, style);
  }

  int
  Context::get_style (const string& sym) const
  {
    for (const_reverse_iterator r = rbegin(); r != rend(); ++r) {
      if (unsigned int result = (*r)->styles.get (sym))
	return result;
      else if ((*r)->flags & GET_OPAQUE)
	break;
    }

    return MAKE_STYLE (PREFIX, PREC_NOR);
  }

  void
  Context::add_outfix (const string& left, const string& right)
  {
    reverse_iterator r = _writable();
    (*r)->pairs.add (left, right);
    (*r)->styles.add (left,  MAKE_STYLE (LOUTFIX, PREC_SEP));
    (*r)->styles.add (right, MAKE_STYLE (ROUTFIX, PREC_SEP));
  }

  bool
  Context::has_outfix (const string& left, const string& right) const
  {
    for (const_reverse_iterator r = rbegin(); r != rend(); ++r) {
      if ((*r)->pairs.has (left, right))
	return true;
      else if ((*r)->flags & GET_OPAQUE)
	break;
    }

    return false;
  }

  const string&
  Context::get_special (const string& id) const
  {
    for (const_reverse_iterator r = rbegin(); r != rend(); ++r) {
      if (const string* val = (*r)->names.get (id))
	return *val;
      else if ((*r)->flags & GET_OPAQUE)
	break;
    }

    throw E (E_MISSING_SPECIAL, MStr::create (id));
  }

  void
  Context::set_special (const string& id, const string& name)
  {
    (*_writable())->names.add (id, name);
  }

  TermPtr
  Context::get_slot (const string& name) const
  {
    for (const_reverse_iterator r = rbegin(); r != rend(); ++r) {
      if (TermPtr result = (*r)->slots.get (name))
	return result;
      else if ((*r)->flags & GET_OPAQUE)
	return TermPtr();
    }

    return TermPtr ();
  }

  void
  Context::set_slot (const string& name, TermPtr val)
  {
    (*_writable())->slots.set (name, val);
  }

  TermPtr
  Context::get_mod (const string& name) const
  {
    for (const_reverse_iterator r = rbegin(); r != rend(); ++r) {
      if (TermPtr result = (*r)->mods.get (name))
	return result;
      else if ((*r)->flags & GET_OPAQUE)
	return TermPtr();
    }

    return TermPtr ();
  }

  void
  Context::set_mod (const string& name, TermPtr val)
  {
    (*_writable())->mods.set (name, val);
  }
};
