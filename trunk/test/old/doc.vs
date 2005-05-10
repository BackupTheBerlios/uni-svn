defout "[" "]";
defsty "[ ]" EOS PREC_SEP;
define "[ ]" (proj RAW VOID) (\x(slot_set "curr" (dict_new ANY)));

defsty "->" LINFIX (add PREC_NOR 1);
define "->" (proj ANY (proj ANY STR)) ((\x(\y(concat (str x) (concat "$\to$" (str y))))));

defsty ":" LINFIX (sub PREC_NOR 1);
define ":" (proj SYM (proj STR VOID))
(\x(\y(
    scope_push;
    defmap "curr" (slot_get "curr");
    defmap "name" (str x);
    ifeq true (dict_has curr name)
        (dict_set curr name (concat (concat (dict_get curr name) " ") y))
        (dict_set curr name y);
    scope_pop;
)));

undef "type";
