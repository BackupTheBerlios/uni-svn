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

#ifndef _UNI_MACHINE_HH_
#define _UNI_MACHINE_HH_

#include <basicdef.hh>
#include <style.hh>
#include <term.hh>

namespace NAMESPACE
{
  class Frame;
  class Context;
  class Shield;

  class DebugHandler
  {
  public:

    virtual ~DebugHandler () { }
    virtual void step_break (Machine* machine, TermPtr term) = 0;
  };

  /**
   * The class of machine object.
   * A machine is a framework of the environment in which programs can run.
   */
  class Machine : private vector<Shield*>
  {
    VAL_PROPERTY_RO (Context*, context);
    VAL_PROPERTY    (bool, step_break);
    VAL_PROPERTY    (DebugHandler*,   debugger);

  public:

    Machine ();
    ~Machine ();

    TermPtr special_sym (const string& id) const;

    void set_attr (const string& id, int val);

    /**
     * Push an application root on to the evaluation stack.
     * Conceptually, this is equivalent to append an argument to a function.
     *
     * \param  app  the application root to be pushed on to the stack.
     */
    void push (AppPtr app);

    void pop (unsigned int n);
    TermPtr pop ();

    /**
     * This function returns the ith argument on the stack
     * without any process. It will not reduce nor update anything.
     *
     * \param   i   the index of the argument to be returned
     * \return  the argument requested
     */
    TermPtr arg (unsigned int i);
    TermPtr arg_reduce (unsigned int i, int flags, TermPtr expected = TermPtr(), bool collect = true);
    TermPtr arg_type (unsigned int i);
    AppPtr  arg_root (unsigned int i);
    bool arg_compat (unsigned int i, int flags, TermPtr type);
    unsigned int arg_count () const;

    int current_flags () const;

    Frame* current_frame () const;

    /**
     * The depth of frames in the innermost shield.
     */
    unsigned int frame_depth () const;

    /**
     * The depth of shields.
     */
    unsigned int shield_depth () const { return vector<Shield*>::size(); }

    /**
     * Reduce a term in the machine.
     * This function DOES NOT try to catch any exceptions,
     * thus all the exception raised will be thrown outward,
     * and will not be catched by handlers already defined.
     * For reduction with exceptions being properly handled,
     * use reduce_in_shield instead of this one.
     *
     * \param   term     the term to be reduced,
     *
     * \param   flags    flags for reduction, can be a bit
     *                   combination any flags defined in the
     *                   reduction flags section.
     *
     * \param   expect   the expected type. it is by default empty.
     *                   some terms can be reduced into different
     *                   form depending on the expected type.
     *                   for instance, functions can be overload
     *                   on different return type.
     *
     * \param   collect  Whether or not to collect application
     *                   tree that cannot be reduced due to the
     *                   reduction flags problem.
     *                   If set to true, applications left will
     *                   be collected and returned as they were.
     *                   Otherwise, null pointer will be returned
     *                   if reduction cannot continue properly.
     *
     * \return  if the term is reducible, the result is returned,
     *          otherwise the original term is returned.
     */
    TermPtr reduce (TermPtr term,
		    int flags,
		    TermPtr expected = TermPtr(),
		    bool collect = true);

    /**
     * Reduce a term inside a exception shield.
     * This function internally call [reduce] perform the reduction,
     * however, this function will catch all the exception thrown
     * during the reduction, and try to pass them to handlers
     * defined in the context, and only throw out exceptions
     * when no handler can handle the exception.
     *
     * \param   term    The term to be reduced.
     *
     * \param   flags   The flags for reduction, can be a bit
     *                  combination any flags defined in the
     *                  reduction flags section.
     *
     * \param   exit    The exit guard expression.
     *                  When control flow exit the shield (either
     *                  normally or due to exception) this expression
     *                  will be reduced.
     *
     * \return  if the term is reducible, the result is returned,
     *          otherwise the original term is returned.
     */
    TermPtr reduce_in_shield (TermPtr term, int flags = ALL, TermPtr exit = TermPtr());

    /**
     * Evaluate a raw expression generated by scanner.
     * This function internally call reduce_in_shield to reduce
     * the raw expression. Therefore, all the exceptions will be
     * handled by handlers defined inside the input code, and only
     * be thrown outside when there are not handlers for them.
     *
     * \param   raw     the raw expression generated by the scanner
     *
     * \param   flags   flags for reduction, could be a bit
     *                  combination of BIND, CTXT, NICE, and DIRTY.
     *                  the default value is 0 which means only
     *                  pure terms will be reduced.
     *
     * \return  if the input stream does not contain any valid code,
     *          VOID term will be returned.
     *          otherwise the original term is returned.
     */
    TermPtr eval (RawPtr raw, int flags = ALL);

  private:

    void _new_frame (int flags);
    void _del_frame ();
    void _new_shield (TermPtr exit = TermPtr());
    void _del_shield (int flags);

    AppPtr _pop_root ();
    AppPtr _top_root ();
    AppPtr _bottom_root ();

    TermPtr _expand_app (TermPtr term);
  };
};

#endif
