import "arith";

define "mk_body"
((typetmp CONS RAW)->RAW->ANY)
(\xs
 (\body
  (ifeq nil (tail xs)
   (\\ (bind (head xs)) body)
   (\\ (bind (head xs)) (mk_body (tail xs) body)))));

defmap "TYPED_FUNC" (type "TYPED_FUNC");

defsty "=" LINFIX (PREC_NOR - 3);
defsty ":" LINFIX (PREC_NOR + 1);

define ":" (SYM->TYPE->VOID)
(\x
 (\y
  (slot_set (caststr x) y)));

define "=" (RAW->RAW->VOID)
(\x
 (\y
  (let xs (cons_t2 x)
   (let n (caststr (bind (head xs)))
    (define n (slot_get n) (mk_body (tail xs) y))))));
