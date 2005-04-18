import "base";

defsty "+" LINFIX (add PREC_NOR 3);
define "+" (INT->INT->INT) add;

defsty "-" LINFIX (PREC_NOR + 3);
define "-" (INT->INT->INT) sub;

defsty "*"  LINFIX (PREC_NOR + 5);
define "*" (INT->INT->INT) mul;

defsty "/"  LINFIX (PREC_NOR + 4);
define "/" (INT->INT->INT) div;

defsty "++" PREFIX (PREC_NOR + 4);
define "++" (INT->INT) (\x(set_int x (x+1); return x));

defsty "--" PREFIX (PREC_NOR + 4);
define "--" (INT->INT) (\x(set_int x (x-1); return x));

defsty "<"  LINFIX (PREC_NOR + 2);
define "<" (INT->INT->BOOL) less;

defsty ">"  LINFIX (PREC_NOR + 2);
define ">" (INT->INT->BOOL) greater;
