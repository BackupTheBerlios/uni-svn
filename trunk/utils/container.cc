#include <bool.hh>
#include <exception.hh>
#include <func.hh>
#include <handle.hh>
#include <proj.hh>
#include <string.hh>
#include <style.hh>

#include <list>
#include <map>
#include <string>

using namespace NAMESPACE;
using std::list;
using std::map;
using std::string;

typedef map <string, TermPtr> map_t;
typedef list <TermPtr> list_t;

//// map ////////////////////////////////////////////////////////////////
static void
map_delete (void* obj)
{
  map_t *m = static_cast<map_t*> (obj);
  delete m;
}

static TermPtr
map_new (TermPtr type)
{
  return Handle::create (type, new map_t, map_delete);
}

static TermPtr
map_get (TermPtr obj_term, TermPtr key_term)
{
  P(Handle) obj = TCAST<Handle> (obj_term);
  P(Str)    key = TCAST<Str>    (key_term);

  map_t *m = static_cast<map_t*> (obj->handle());
  map_t::iterator iter = m->find (key->str());

  if (m->end() != iter)
    return iter->second;
  else
    return TermPtr(); // \todo throw an exception.
}

static TermPtr
map_has (TermPtr obj_term, TermPtr key_term)
{
  P(Handle) obj = TCAST<Handle> (obj_term);
  P(Str)    key = TCAST<Str>    (key_term);

  map_t *m = static_cast<map_t*> (obj->handle());
  map_t::iterator iter = m->find (key->str());

  if (m->end() != iter)
    return Bool::TRUE;
  else
    return Bool::FALSE;
}

static TermPtr
map_set (TermPtr obj_term, TermPtr key_term, TermPtr val_term)
{
  P(Handle) obj = TCAST<Handle> (obj_term);
  P(Str)    key = TCAST<Str> (key_term);

  map_t *m = static_cast<map_t*> (obj->handle());
  (*m)[key->str()] = val_term;
  return VOID;
}

//// list ////////////////////////////////////////////////////////////
static void
list_delete (void* obj)
{
  list_t *l = static_cast<list_t*> (obj);
  delete l;
}

static TermPtr
list_new (TermPtr type)
{
  return Handle::create (type, new list_t, list_delete);
}

static TermPtr
list_append (TermPtr obj_term, TermPtr val_term)
{
  list_t *l = static_cast<list_t*> ((TCAST<Handle>(obj_term))->handle());
  l->insert(l->end(), val_term);
  return VOID;
}

static TermPtr
list_front (TermPtr obj_term)
{
  list_t *l = static_cast<list_t*> ((TCAST<Handle>(obj_term))->handle());
  return l->front();
}

static TermPtr
list_back (TermPtr obj_term)
{
  list_t *l = static_cast<list_t*> ((TCAST<Handle>(obj_term))->handle());
  return l->back();
}

//// list iterator ///////////////////////////////////////////////////////
static void
list_iter_delete (void* obj)
{
  list_t::iterator *iter = static_cast<list_t::iterator*> (obj);
  delete iter;
}

static TermPtr
list_begin (TermPtr obj_term)
{
  list_t *l = static_cast<list_t*> ((TCAST<Handle>(obj_term))->handle());
  return Handle::create (Term::T, new list_t::iterator (l->begin()), list_iter_delete);
}

static TermPtr
list_end (TermPtr obj_term)
{
  list_t *l = static_cast<list_t*> ((TCAST<Handle>(obj_term))->handle());
  return Handle::create (Term::T, new list_t::iterator (l->end()), list_iter_delete);
}

static TermPtr
list_iter_deref (TermPtr obj_term)
{
  list_t::iterator *iter = static_cast<list_t::iterator*> ((TCAST<Handle>(obj_term))->handle());
  return **iter;
}

static TermPtr
list_iter_inc (TermPtr obj_term)
{
  list_t::iterator *iter = static_cast<list_t::iterator*> ((TCAST<Handle>(obj_term))->handle());
  ++(*iter);
  return VOID;
}

static TermPtr
list_iter_dec (TermPtr obj_term)
{
  list_t::iterator *iter = static_cast<list_t::iterator*> ((TCAST<Handle>(obj_term))->handle());
  --(*iter);
  return VOID;
}

//// library entry points //////////////////////////////////////////
extern "C"
{
  ext_map_t* create_map ()
  {
    ext_map_t *map = new ext_map_t;

    (*map) ["map_new"] = ext_t (1, PURE, (void*)map_new, P1 (Term::T, Term::T));
    (*map) ["map_get"] = ext_t (2, PURE, (void*)map_get, P2 (Term::T, Str::T, Term::T));
    (*map) ["map_has"] = ext_t (2, PURE, (void*)map_has, P2 (Term::T, Str::T, Bool::T));
    (*map) ["map_set"] = ext_t (3, PURE, (void*)map_set, P3 (Term::T, Str::T, Term::T, VOID_T));

    (*map) ["list_new"]    = ext_t (1, PURE, (void*)list_new, P1 (Term::T, Term::T));
    (*map) ["list_append"] = ext_t (2, PURE, (void*)list_append, P2 (Term::T, Term::T, VOID_T));
    (*map) ["list_front"]  = ext_t (1, PURE, (void*)list_front, P1 (Term::T, Term::T));
    (*map) ["list_back"]   = ext_t (1, PURE, (void*)list_back, P1 (Term::T, Term::T));

    (*map) ["list_begin"]  = ext_t (1, PURE, (void*)list_begin, P1 (Term::T, Term::T));
    (*map) ["list_end"]    = ext_t (1, PURE, (void*)list_end, P1 (Term::T, Term::T));

    (*map) ["list_iter_deref"] = ext_t (1, PURE, (void*)list_iter_deref, P1 (Term::T, Term::T));
    (*map) ["list_iter_inc"]   = ext_t (1, PURE, (void*)list_iter_inc,   P1 (Term::T, VOID_T));
    (*map) ["list_iter_dec"]   = ext_t (1, PURE, (void*)list_iter_dec,   P1 (Term::T, VOID_T));

    return map;
  }

  void destroy_map (ext_map_t *map)
  {
    delete map;
  }
};
