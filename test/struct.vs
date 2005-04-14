import "base";
import "dict";
import "bool";

defaty "STRUCT";
defaty "MEMBER";

### the struct scope ###########################################################
scope_push;

defsty "$=" LINFIX (sub PREC_NOR 6);
define "$=" (MEMBER->ANY->VOID)
(\x(\y(
    if (not (is_sub (typeof y) x.type))
       (throw E_CAST y (typeof y) x.type)
       (x.ival = y)
)));

define "mk_mem" (TYPE->ANY->RAW->MEMBER)
(\t(\i(\n{
    s := slot_get "curr_struct";
    m := dict_new MEMBER;
    m.type = t;
    m.ival = i;
    dict_set s (rstr n) m;
    return m;
})));

define "any"    (RAW->MEMBER) (mk_mem ANY 0);
define "int"    (RAW->MEMBER) (mk_mem INT 0);
define "string" (RAW->MEMBER) (mk_mem STR "");
define "symbol" (RAW->MEMBER) (mk_mem SYM z);

defmap "struct_scope" scope_popx;
################################################################################

define "struct" (SYM->RAW->VOID)
(\x(\y(
    defmap (str x) (dict_new STRUCT);
    scope_pushx struct_scope;
    slot_set "curr_struct" (bind (tok x));
    y;
    slot_set "curr_struct" nil;
    scope_pop;
)));

define " " (STRUCT->SYM->VOID)
(\x(\y(
    defmap (str y) (dict_new x);
    dict_vis x (\xx(\yy(dict_set (bind (tok y)) xx (dict_get yy "ival"))));
)));

struct chen (
    int tian $= 99;
    string ran $= "xyz";
);
