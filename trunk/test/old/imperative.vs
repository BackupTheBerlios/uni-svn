import "base";
import "arith";
import "dict";

define "while" (BOOL->RAW->VOID) (\x(\y(ifeq true x (y; while x y;) void)));
define "for" (RAW->BOOL->RAW->RAW->VOID) (\i(\c(\n(\x(i; while c (n; x;))))));

defaty "ELSE";

define "ifx"  (BOOL->RAW->ELSE->UPR) (\x(\t(\f(ifeq x true t (dict_get f "action")))));
defsty "else" PREFIX (add PREC_NOR 1);
define "else" (RAW->ELSE)
(\x{
    r := dict_new ELSE;
    r.action = x;
    return r;
});

define "ff" (BOOL->STR)
(\x(
    ifx (x)
        "yes"
    else
        "no sir!"
));
