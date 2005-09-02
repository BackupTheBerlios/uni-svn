#ifndef _UNI_STYLE_HH_
#define _UNI_STYLE_HH_

#define MASK_FIX         0x0000FFFF
#define MASK_PRC         0xFFFF0000
#define GET_FIX(x)       (((unsigned int)x) & MASK_FIX)
#define GET_PRC(x)       (((unsigned int)x) >> 16)
#define MAKE_STYLE(f,p)  ((((unsigned int)f) & MASK_FIX) | (((unsigned int)p) << 16))

namespace NAMESPACE
{ 
  const int CONS = 0;      // parse and construct application trees and resolve families.
  const int BIND = 1 << 0; // bind families to defined expressions.
  const int META = 1 << 1; // perform meta reductions.
  const int PURE = 1 << 2; // perform pure (no side effect) reductions.
  const int CTXT = 1 << 3; // perform machine sensitive reductions.
  const int NICE = 1 << 4; // perform nicely behaved reductions.
  const int SIDE = 1 << 5; // perform side effect (nice or not nice) reductions.

  const int ALL_META = BIND | META;
  const int ALL_PURE = BIND | META | PURE;
  const int ALL_CTXT = BIND | META | PURE | CTXT;
  const int ALL_NICE = BIND | META | PURE | CTXT | NICE;
  const int ALL      = BIND | META | PURE | CTXT | NICE | SIDE;

  enum { PREFIX  = 1 << 0,
	 POSTFIX = 1 << 1,
	 LINFIX  = 1 << 2,
	 RINFIX  = 1 << 3,
	 LOUTFIX = 1 << 4,
	 ROUTFIX = 1 << 5,
	 EOS     = 1 << 6,
	 GROUP   = 1 << 7,
	 DUMMY   = 1 << 8 };

  const unsigned int PREC_MIN = 0x0000;
  const unsigned int PREC_NOR = 0x8000;
  const unsigned int PREC_SEP = 0xFFF0;
  const unsigned int PREC_MAX = 0xFFFF;
};

#endif
