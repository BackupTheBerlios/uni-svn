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

#ifndef _UNI_FAMILY_HH_
#define _UNI_FAMILY_HH_

#include <term.hh>

#include <list>
#include <utility>

namespace NAMESPACE
{
  class Family : public Term, private vector<TempPtr>
  {
    DEF_TERM      (Family);
    DEF_VISITABLE (Family, fam);

  public:

    virtual TermPtr reduce (Machine* machine, int flags, TermPtr expected);

    /**
     * Add a new function or a new function template
     * to the function family.
     *
     * \param   body   the function.
     * \param   type   the type of the function,
     *                 must be already reduced.
     */
    virtual void add (TermPtr body, TermPtr type);

  private:

    class BindState : private std::list<BindState*>
    {
    public:

      void add (TermPtr body, TermPtr type);
      unsigned int size() const { return std::list<BindState*>::size(); }

      BindState* move (Machine* machine, int flags, unsigned int argi);
      TermPtr move (TermPtr type);

    private:

      TermPtr _edge;
      TermPtr _body;
      bool    _leaf;
    };

  private:

    BindState _node;
  };
};

#endif
