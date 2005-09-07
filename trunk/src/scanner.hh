/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

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
    RawPtr scan (Machine* machine, std::istream& input, std::ostream& error);

  protected:

    virtual void xscan (std::istream& input, std::ostream& error) = 0;

    void new_ctor (const char* str, const char* ctor, int lineno = -1, int colno = -1);
    void new_tok (const char* str, int lineno = -1, int colno = -1);

    void group_begin (int level);
    void group_end (int level);

  private:

    Machine* _machine;
    vector < pair<RawPtr, int> > _raws;

  private:

    static SrcPtr _mk_src (int line_begin = -1,
			   int col_begin  = -1,
			   int line_end   = -1,
			   int col_end    = -1);
  };
};

#endif
