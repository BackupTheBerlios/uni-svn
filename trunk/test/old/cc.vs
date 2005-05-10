import "base";

defout "{" "}";
defsty "{ }" EOS PREC_SEP;
define "{ }" (RAW->ANY) (\x(push_scope; reduce_in_shieldx x pop_scope));

defsty "+" LINFIX (add PREC_NOR 3);
define "+" (INT->INT->INT) add;

defsty "-" LINFIX (PREC_NOR + 3);
define "-" (INT->INT->INT) sub;

defsty "*"  LINFIX (PREC_NOR + 3);
define "*" (INT->INT->INT) mul;

defsty "/"  LINFIX (PREC_NOR + 3);
define "/" (INT->INT->INT) div;

defsty ":" RINFIX (PREC_NOR + 2);
deftmp ":" T (T -> T -> (typetmp CONS T)) (\x(\y(cons x (cons y nil))));
deftmp ":" T (T -> (typetmp CONS T) -> (typetmp CONS T)) (\x(\y(cons x y)));

defmap "ELSE_BLOCK" (type "ELSE_BLOCK");

defsty "else" PREFIX (PREC_NOR + 1);
define "else" (ANY->ELSE_BLOCK) (\x(as ELSE_BLOCK x));

defsty "==" LINFIX (PREC_NOR - 2);
define "==" (ANY->ANY->BOOL) (\x(\y(ifeq x y true false)));

define "if" (BOOL->ANY->ANY) (\x(\y(ifeq x true y void)));
define "if" (BOOL->ANY->ELSE_BLOCK->ANY) (\x(\y(\z(ifeq x true y (sa z)))));

defsty "++" LINFIX (PREC_NOR - 1);
define "++" (STR->STR->STR) (\x(\y(concat x y)));

define "str" (STR->STR) (\x x);
define "str" (BOOL->STR) (\x(ifeq true x "true" "false"));

defsty "=" LINFIX (sub PREC_NOR 3);
define "=" (RAW->RAW->VOID) (\x(\y(let xs (cons_t2 x) (defmap (str (bind (head xs))) (\\ (bind (nth 1 xs)) y)))));

#define "=" (INT->INT->VOID) (\x(\y(set_int x y)));
#define "=" (STR->STR->VOID) (\x(\y(set_str x y)));

define "print" (ANY->VOID) (\x(raw_out ((str x) ++ (cstr "\n"))));

defmap "ARG" (type "ARG");

#define "mk_proto"
#       (TYPE->SYM->ARG->VOID)
#       (\r(\n(\p(print n; print p))));

#define "mk_func"
#(TYPE->SYM->ARG->RAW->VOID)
#(\r(\n(\p(\b(
#    define (str n)
#    ((elem 0 (sa p))->r)
#    (\\ (as SYM (elem 1 (sa p))) b))))));

#define "int" (SYM->VOID) (\x(defmap (str x) 0));
define "int"
(SYM->(cons TYPE SYM)->VOID)
(\s(\p(print (head p); print (tail p);)));
define "int" (SYM->(cons TYPE SYM))  (\x(cons INT x));
#define "int" (SYM->RAW->VOID) (\x(\y(mk_proto INT x (reduce_in_scope __arg_scope y))));
#(\x(\y(\z(mk_func INT x y z))));

define "ggg" ((cons TYPE SYM)->VOID) (\x(print "yes right"));

define "bool" (SYM->RAW->VOID) (\x(\y(print "SHIT")));

defmap "CATCH_BLOCK" (type "CATCH_BLOCK");

name_set "exc_handler" "__catch";

defsty "catch" PREFIX (PREC_NOR+1);

define "catch"
       (RAW->CATCH_BLOCK)
       (\x(as CATCH_BLOCK x));

define
"try"
(RAW->CATCH_BLOCK->VOID)
(\b(\c( {
    define "__catch" (EXC->VOID) (print "YEAH!");
    reduce_in_shield b;
    } )));

define
"step"
(VOID)
(set_attr "step_break" 1);

define "len" (nil -> INT) (\x 0);

deftmp "len" T ((typetmp CONS T) -> INT) (\x(ifeq x nil 0 (len (tail x)) + 1));

deftmp "nth" T
(INT -> (typetmp CONS T) -> T)
(\n(\x(ifeq 0 n (head x) (nth (n-1) (tail x)))));

