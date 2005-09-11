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
#include <utility>

using namespace NAMESPACE;
using std::list;
using std::map;
using std::string;
using std::make_pair;

typedef map <string, TermPtr> map_t;
typedef list <TermPtr> list_t;

//// map ////////////////////////////////////////////////////////////////
class MapHandle : public Term, public map_t
{
  DEF_DYNAMIC (MapHandle);

protected:

  MapHandle () { assert (false); }
  MapHandle (TermPtr type) : Term (type) { }
};

static TermPtr
map_new (TermPtr type)
{
  return MapHandle::create (type);
}

static TermPtr
map_get (TermPtr obj_term, TermPtr key_term)
{
  P(MapHandle) obj = TCAST<MapHandle> (obj_term);
  P(Str)       key = TCAST<Str>       (key_term);

  map_t::iterator iter = obj->find (key->str());

  if (obj->end() != iter)
    return iter->second;
  else
    return TermPtr(); // \todo throw an exception.
}

static TermPtr
map_has (TermPtr obj_term, TermPtr key_term)
{
  P(MapHandle) obj = TCAST<MapHandle> (obj_term);
  P(Str)       key = TCAST<Str>       (key_term);

  if (obj->end() == obj->find (key->str()))
    return Bool::FALSE;
  else
    return Bool::TRUE;
}

static TermPtr
map_set (TermPtr obj_term, TermPtr key_term, TermPtr val_term)
{
  P(MapHandle) obj = TCAST<MapHandle> (obj_term);
  P(Str)       key = TCAST<Str> (key_term);

  obj->insert (make_pair (key->str(), val_term));
  return VOID;
}

//// list ////////////////////////////////////////////////////////////
class ListHandle : public Term, public list_t
{
  DEF_DYNAMIC (ListHandle);

protected:

  ListHandle () { assert (false); }
  ListHandle (TermPtr type) : Term (type) { }
};

static TermPtr
list_new (TermPtr type)
{
  return ListHandle::create (type);
}

static TermPtr
list_append (TermPtr obj_term, TermPtr val_term)
{
  P(ListHandle) obj = TCAST<ListHandle> (obj_term);
  obj->insert(obj->end(), val_term);
  return VOID;
}

static TermPtr
list_front (TermPtr obj_term)
{
  P(ListHandle) obj = TCAST<ListHandle> (obj_term);
  return obj->front();
}

static TermPtr
list_back (TermPtr obj_term)
{
  P(ListHandle) obj = TCAST<ListHandle> (obj_term);
  return obj->back();
}

//// list iterator ///////////////////////////////////////////////////////
class ListIter : public Term, public list_t::iterator
{
  DEF_DYNAMIC (ListIter);

protected:

  ListIter () { assert (false); }
  ListIter (TermPtr type, const list_t::iterator& iter) : Term (type), list_t::iterator (iter) { }
};

static TermPtr
list_begin (TermPtr obj_term)
{
  P(ListHandle) obj = TCAST<ListHandle> (obj_term);
  return ListIter::create (Term::T, obj->begin());
}

static TermPtr
list_end (TermPtr obj_term)
{
  P(ListHandle) obj = TCAST<ListHandle> (obj_term);
  return ListIter::create (Term::T, obj->end());
}

static TermPtr
list_iter_deref (TermPtr obj_term)
{
  P(ListIter) obj = TCAST<ListIter> (obj_term);
  return **obj;
}

static TermPtr
list_iter_inc (TermPtr obj_term)
{
  P(ListIter) obj = TCAST<ListIter> (obj_term);
  ++(*obj);
  return VOID;
}

static TermPtr
list_iter_dec (TermPtr obj_term)
{
  P(ListIter) obj = TCAST<ListIter> (obj_term);
  --(*obj);
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
