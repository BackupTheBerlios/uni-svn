defsty "->" RINFIX (add PREC_NOR 2);
define "->"
       (proj ANY (proj ANY TYPE))
       (\x(\y(proj x y)));

#defout "{" "}";
#define "{ }"
#       (RAW->ANY)
#       (\x(seq_g 1 push_scope pop_scope x));

#define "using"
#(SYM->VOID)
#(\x(import (concat (str x) ".vs")));

#defsty "=" LINFIX (sub PREC_NOR 1);
#define "=" (SYM->RAW->VOID) (\x(\y(defmap (str x) y)));

#defsty ":" RINFIX (add PREC_NOR 1);
#deftmpx ":" (ANY->(cons x y);

defsty "," LINFIX (sub PREC_NOR 1);
#deftmp "," t ((typetmp VEC t)->t->(typetmp VEC t)) (\x(\y(append x y)));
#deftmp "," t (t->t->VEC) (\x(\y(append (append (vec (typeof x)) x) y)));
deftmp "," t (t->t->t) (\x(\y(1)));
