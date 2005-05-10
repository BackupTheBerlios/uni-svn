defsty "->" RINFIX (add PREC_NOR 2);
define "->" (proj ANY (proj ANY TYPE)) (\x(\y(proj x y)));

defsty ":=" LINFIX (sub PREC_NOR 5);
define ":=" (RAW->ANY->VOID) (\x(\y(defmap (rstr x) y)));

defsty "<=" LINFIX (sub PREC_NOR 5);
define "<=" (RAW->ANY->VOID) (\x(\y(defvar (rstr x) y)));

defsty "=" LINFIX (sub PREC_NOR 5);
define "=" (RAW->RAW->VOID) (\x(\y(redef (rstr x) y)));

defsty "==" LINFIX (sub PREC_NOR 4);
define "==" (ANY->ANY->BOOL) (\x(\y(ifeq x y true false)));

defsty "!=" LINFIX (sub PREC_NOR 4);
define "!=" (ANY->ANY->BOOL) (\x(\y(ifeq x y false true)));

defout "{" "}";
define "{ }" (RAW->UPR) (\x(scope_push; retpop x;));

define "debug" VOID (set_attr "step_break" 1);

defsty "$" PREFIX (add PREC_NOR 4);
define "$" (ANY->STR) str;

define " " (VOID->RAW->UPR) (\x(\y y));

name_set "str_ctor" "cstr";
