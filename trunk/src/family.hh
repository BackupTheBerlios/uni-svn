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
    REF_PROPERTY  (string, name);

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

  protected:

    Family (const string& name) : _name (name) { }

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
