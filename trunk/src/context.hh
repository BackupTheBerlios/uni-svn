#ifndef _UNI_SCOPE_HH_
#define _UNI_SCOPE_HH_

#include <term.hh>

namespace NAMESPACE
{
  struct symbols_t : map <pair <string, SpacePtr>, FamPtr>
  {
    void    add (const string& sym, SpacePtr nspace, TermPtr term);
    bool    del (const string& sym, SpacePtr nspace);
    bool    set (const string& sym, SpacePtr nspace, TermPtr term);
    TermPtr get (const string& sym, SpacePtr nspace) const;
  };

  struct styles_t : map <string, int>
  {
    void add (const string& sym, unsigned int style);
    int  get (const string& sym) const;
  };

  struct pairs_t : map <string, string>
  {
    void add (const string& left, const string& right);
    bool has (const string& left, const string& right) const;
    const string* get (const string& left) const;
  };

  struct slots_t : map <string,TermPtr>
  {
    TermPtr get (const string& id) const;
    void    set (const string& id, TermPtr val);
  };

  class Space : public Term
  {
    DEF_TERM      (Space);
    DEF_VISITABLE (Space, space);
    REF_PROPERTY  (string, name);
  };

  class Scope : public Term
  {
    DEF_TERM (Scope);
    DEF_VISITABLE (Scope, scope);

    friend class Context;

  private:

    SpacePtr  nspace;
    int       flags;
    symbols_t symbols;
    styles_t  styles;
    pairs_t   pairs;
    pairs_t   names;
    slots_t   slots;
    slots_t   mods;
  };

  /**
   * Class for scope stack objects.
   * A scope stack is a stack of nested context.
   *
   */
  class Context : private vector <ScopePtr>
  {
  public:

    enum {GET_OPAQUE = 1, SET_TRANS  = 2};

  public:

    unsigned int size () const { return vector<ScopePtr>::size(); }

    void pop () { assert (size()); pop_back(); }
    void push (int flags = 0);
    void push (ScopePtr scope) { push_back (scope); }
    ScopePtr top () const { assert (size()); return back(); }

    void current_nspace (SpacePtr nspace) { back()->nspace = nspace; }
    SpacePtr current_nspace () const { return back()->nspace; }

    void current_flags (int flags) { back()->flags = flags; }
    int  current_flags () const { return back()->flags; }

    /**
     * Add a symbol to the scope stack.
     * More specificly it will add the symbol to the topmost
     * non-write-through scope found on the scope stack.
     * If a family with the same name already exist in the
     * scope, then the term will be add the family, otherwise
     * a new family will be created.
     * If terms with same type already exist in the family,
     * then the function will throw a name duplication exception
     * (E_DUP_NAME).
     *
     * \param   term     the term to be bind to the name.
     * \param   sym      the name of the symbol.
     * \param   nspace   the namespace into which the symbol will be added.
     */
    void add_symbol (TermPtr term, const string& sym, SpacePtr nspace = SpacePtr());

    /**
     * delete the mapping between a symbol and corresponding term.
     * it will only delete the "topmost" symbol that has the name
     * in the stack of context.
     *
     * \param   name     the symbolic name of the symbol to be deleted.
     * \param   nspace   the namespace into which the symbol will be added.
     * \retval  true     the symbol is deleted
     * \retval  false    the symbol cannot be found
     */
    bool del_symbol (const string& sym, SpacePtr nspace = SpacePtr());

    bool set_symbol (const string& sym, TermPtr term, SpacePtr nspace = SpacePtr());

    /**
     * Get a symbol.
     *
     * \param   sym      the symbolic name of the symbol to be get.
     * \param   nspace   the namespace which the symbol should located.
     * \return  if the symbol exist, the term binded to the symbol will be
     *          returned, otherwise a empty pointer is returned.
     */
    TermPtr get_symbol (const string& sym, SpacePtr nspace = SpacePtr()) const;

    /**
     * Add a mapping between a symbol and its style to the machine.
     * if the mapping between the symbol and the style already exit,
     * then new style will override the old one.
     *
     * \param   sym   the name of the symbol.
     * \param   style   the style for the symbol.
     */
    void add_style (const string& sym, unsigned int style);

    int get_style (const string& sym) const;

    void add_outfix (const string& left, const string& right);
    bool has_outfix (const string& left, const string& right) const;

    const string& get_special (const string& id) const;
    void set_special (const string& id, const string& name);

    TermPtr get_slot (const string& name) const;
    void    set_slot (const string& name, TermPtr val);

    TermPtr get_mod (const string& name) const;
    void    set_mod (const string& name, TermPtr val);

  private:

    reverse_iterator _writable ();
  };
};

#endif
