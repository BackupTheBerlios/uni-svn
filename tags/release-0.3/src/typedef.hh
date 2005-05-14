#ifndef _UNI_TYPEDEF_HH_
#define _UNI_TYPEDEF_HH_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

using boost::shared_ptr;
using boost::weak_ptr;
using boost::dynamic_pointer_cast;
using boost::const_pointer_cast;
using std::list;
using std::map;
using std::pair;
using std::string;
using std::vector;

namespace NAMESPACE
{
#define P(TYPE)  shared_ptr<TYPE>
#define WP(TYPE) weak_ptr<TYPE>
#define CAST dynamic_pointer_cast

#define DEF_PTR(CLASS) class CLASS; typedef P(CLASS) CLASS ## Ptr; typedef WP(CLASS) CLASS ## PtrW;
#define DEF_PTRX(CLASS,PREFIX) class CLASS; typedef P(CLASS) PREFIX ## Ptr; typedef WP(CLASS) PREFIX ## PtrW;

  DEF_PTR (Abs);
  DEF_PTR (App);
  DEF_PTR (Bif);
  DEF_PTR (Bool);
  DEF_PTR (Cons);
  DEF_PTR (Consh);
  DEF_PTR (Const);
  DEF_PTR (CPtr);
  DEF_PTR (CMem);
  DEF_PTR (Dict);
  DEF_PTR (Exc);
  DEF_PTR (Func);
  DEF_PTR (Int);
  DEF_PTR (List);
  DEF_PTR (Proj);
  DEF_PTR (Raw);
  DEF_PTR (Ret);
  DEF_PTR (Scope);
  DEF_PTR (Seq);
  DEF_PTR (Space);
  DEF_PTR (Str);
  DEF_PTR (Sym);
  DEF_PTR (Temp);
  DEF_PTR (Term);
  DEF_PTR (Tok);
  DEF_PTR (Type);

  DEF_PTRX (Family, Fam);
  DEF_PTRX (BoundVar, Var);

  DEF_PTRX (SrcInfo, Src);

  typedef vector<TermPtr>       TermVec;
  typedef map<TermPtr,TermPtr>  TermMap;
  typedef list<TermPtr>         TermList;
  typedef pair<TermPtr,TermPtr> TermPair;

  class Context;
};

#endif
