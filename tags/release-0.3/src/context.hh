#ifndef _UNI_CONTEXT_HH_
#define _UNI_CONTEXT_HH_

#include <basicdef.hh>
#include <style.hh>
#include <term.hh>

#define TOP_SHIELD (back())
#define TOP_FRAME  (back()->back())
#define HAS_SHIELD (! empty())
#define HAS_FRAME  ((! empty()) && (! back()->empty()))

namespace NAMESPACE
{
  class Frame;
  class ScopeStack;
  class Shield;

  class ImportHandler;
  class DebugHandler;

  /**
   * The class of context object.
   * A context is a framework of the environment in which programs can run.
   */
  class Context : private vector<Shield*>
  {
    VAL_PROPERTY_RO (ScopeStack*,     scopes);

    VAL_PROPERTY (bool, step_break);

    VAL_PROPERTY (ImportHandler*,  importer);
    VAL_PROPERTY (DebugHandler*,   debugger);
    VAL_PROPERTY (vector<string>*, ansi_attrs);

  public:

    Context ();
    ~Context ();

    TermPtr special_sym (const string& id) const;

    void set_attr (const string& id, int val);

    /**
     * Push an application root on to the evaluation stack.
     * This is equivalent to append an argument to a function.
     *
     * \param  app  the application root to be pushed on to the stack.
     */
    void push (AppPtr app);

    void pop (unsigned int n);
    TermPtr pop ();

    TermPtr arg (unsigned int i);
    AppPtr  arg_root (unsigned int i);
    TermPtr arg_reduce (unsigned int i, int flags, TermPtr expected = TermPtr(), bool collect = true);
    TermPtr arg_type (unsigned int i);
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
     * Reduce a term in the context.
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
     * Return a term inside a exception shield.
     * This function internally call reduce perform the reduction,
     * however, this function will catch all the exception thrown
     * during the reduction, and try to pass them to handlers
     * defined in the scopes, and only throw out exceptions
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

    /**
     * Import a library.
     * Programs can request to import a library through this interface.
     * The request will be forward to the import handler, by which
     * the request will be checked against security settings and the
     * list of available libraries.
     * And all these behaviors are depending on the import handler.
     *
     * \param   libname   the name of the library; this parameter will be
     *                    passed to the handler directly, with no interpretion.
     */
    void import (const string& libname);

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