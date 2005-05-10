#ifndef _UNI_FRAME_HH_
#define _UNI_FRAME_HH_

#include <typedef.hh>

#include <vector>

namespace NAMESPACE
{
  typedef pair <AppPtr,TermPtr> AppType;

  class Frame : private vector<AppType>
  {
    VAL_PROPERTY (int, flags);

  public:

    Frame (int flags) : _flags (flags) { }

  public:

    unsigned int size () const { return vector<AppType>::size(); }

    void push (AppPtr app);
    void pop (unsigned int n);
    AppPtr pop () { AppPtr r = top_root(); pop_back(); return r; }

    void set_root (unsigned int i, AppPtr app) { assert (size() > i); (*this)[size()-i-1].first = app; }
    void set_type (unsigned int i, TermPtr term) { assert (size() > i); (*this)[size()-i-1].second = term; }

    AppPtr  get_root (unsigned int i) const { assert (size() > i); return (*this)[size()-i-1].first; }
    TermPtr get_type (unsigned int i) const { assert (size() > i); return (*this)[size()-i-1].second; }

    AppPtr  top_root () const { return size() ? back().first : AppPtr(); }
    TermPtr top_type () const { return size() ? back().second : TermPtr(); }

    AppPtr  bottom_root () const { return size() ? front().first : AppPtr(); }
    TermPtr bottom_type () const { return size() ? front().second : TermPtr(); }
  };
};

#endif
