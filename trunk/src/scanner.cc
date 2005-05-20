#include <app.hh>
#include <context.hh>
#include <int.hh>
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
  Scanner::int_ctor (const char* str, int lineno, int colno)
  {
    TermPtr term;
    if (TermPtr f = _context->special_sym ("int_ctor"))
      term = App::create (f, MStr::create (str));
    else
      term = Int::create (str);

    term->src (_mk_src (lineno, colno));
    _raws.back().first->append (term);
  }

  void
  Scanner::str_ctor (const char* str, int lineno, int colno)
  {
    TermPtr term = MStr::create (str);
    SrcPtr  src  = _mk_src (lineno, colno);

    term->src (src);

    if (TermPtr f = _context->special_sym ("str_ctor")) {
      term = App::create (f, term);
      term->src (src);
    }

    _raws.back().first->append (term);
  }

  void
  Scanner::sym_ctor (const char* str, int lineno, int colno)
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
