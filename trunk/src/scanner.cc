#include <app.hh>
#include <context.hh>
#include <scanner.hh>
#include <scope.hh>
#include <string.hh>
#include <tok.hh>

using std::make_pair;

namespace NAMESPACE
{
  RawPtr
  Scanner::scan (Context* context, std::istream& input, std::ostream& error)
  {
    RawPtr raw = Raw::create();

    _context = context;
    _raws.clear();
    _raws.push_back (make_pair (raw, 0));
    xscan (input, error);

    return raw;
  }

  void
  Scanner::new_ctor (const char* str, const char* ctor, int lineno, int colno)
  {
    TermPtr term = Tok::create (ctor, true);
    term = App::create (term, MStr::create(str));
    term->src (_mk_src (lineno, colno));
    _raws.back().first->append (term);
  }

  void
  Scanner::new_tok (const char* str, int lineno, int colno)
  {
    TermPtr term = Tok::create (str);
    term->src (_mk_src (lineno, colno));
    _raws.back().first->append (term);
  }

  void
  Scanner::group_begin (int level)
  {
    RawPtr new_raw = Raw::create();
    _raws.back().first->append (new_raw);
    _raws.push_back (make_pair (new_raw, level));
  }

  void
  Scanner::group_end (int level)
  {
    while ((!_raws.empty()) && (level < _raws.back().second))
      _raws.pop_back();
  }

  SrcPtr
  Scanner::_mk_src (int line_begin, int col_begin, int line_end, int col_end)
  {
    SrcPtr src (new SrcInfo);
    src->line_begin   = line_begin;
    src->line_end     = line_end;
    src->column_begin = col_begin;
    src->column_end   = col_end;

    return src;
  }
};
