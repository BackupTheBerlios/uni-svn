#ifndef _UNI_SCANNER_HH_
#define _UNI_SCANNER_HH_

#include <basicdef.hh>
#include <typedef.hh>

namespace NAMESPACE
{
  struct SrcInfo
  {
    int   line_begin;
    int   line_end;
    short column_begin;
    short column_end;
  };

  class Scanner
  {
  public:

    virtual ~Scanner () { }
    RawPtr scan (Context* context, std::istream& input, std::ostream& error);

  protected:

    virtual void xscan (std::istream& input, std::ostream& error) = 0;

    void new_ctor (const char* str, const char* ctor, int lineno = -1, int colno = -1);
    void new_tok (const char* str, int lineno = -1, int colno = -1);

    void group_begin (int level);
    void group_end (int level);

  private:

    Context* _context;
    vector < pair<RawPtr, int> > _raws;

  private:

    static SrcPtr _mk_src (int line_begin = -1,
			   int col_begin  = -1,
			   int line_end   = -1,
			   int col_end    = -1);
  };
};

#endif
