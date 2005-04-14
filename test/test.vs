import "base";
import "using";

define "assert" (BOOL->VOID) (\x(ifeq true x void (raw_out "[FAIL]\n")));

assert (true);
assert (true);

### test built-in types ###
assert (typeof 1    == INT);
assert (typeof xx   == SYM);
assert (typeof "x"  == STR);
assert (typeof INT  == TYPE);
assert (typeof STR  == TYPE);
assert (typeof SYM  == TYPE);
assert (typeof TYPE == TYPE);

### test built-in arithmetic primitives ###
assert (1 == 1);
assert (2 == 2);
assert (add 1 1 == 2);
assert (add (add 1 2) (add 3 4) == 10);

### test scope primitives ###
scope_push;                          # create a new scope (set-opaque)
defmap "tv_scope" 7;                 # define a symbol
assert (tv_scope == 7);              # test the symbol just defined
scope_pop;                           # discard the scope just created
assert (typeof tv_scope == SYM);     # the symbol should be "forgotten"

### test brace (scope) function ###
assert (typeof xx == SYM);           # the symbol should be still undefined
{                                    # create a new scope (set-opaque)
    defmap "tv_scope" 7;             # define the symbol in this scope
    assert (true);                   # what the $&#%!
    assert (true);                   # what the $&#%!
    assert (tv_scope == 7);          # test the symbol just defined
};                                   # discard the scope just created
assert (typeof tv_scope == SYM);     # the symbol should be "forgotten"

### test function binding ###
{
    define "ff" (RAW->STR) (\x "f");
    define "ff" (INT->INT) (\x x);
    assert (ff 3 == 3);
    assert (ff "test" == "f");
};

{
    define "ff" (INT->INT) (\x x);
    define "ff" (RAW->STR) (\x "f");
    assert (ff 3 == 3);
    assert (ff "test" == "f");
};

### test namespace primitives ###
defmap "test_ns" new_ns;             # define a new namespace
set_ns test_ns;                      # set this as the current namespace
defmap "xx" 7;                       # define a symbol in this namespace
assert (typeof xx == SYM);           # it should be invisible outside the namespace
assert (get_ns test_ns "xx" == 7);   # ...but visible inside the namespace
set_ns nil;                          # reset the current namespace
defmap "xx" 9;                       # define a new symbol in public namespace
assert (xx == 9);                    # the new symbol should be visible everywhere
assert (get_ns test_ns "xx" == 7);   # ...but the old symbol should still exist
undef "xx";                          # undefine the symbol
undef test_ns "xx";                  # undefine the symbol in the namespace
undef "test_ns";                     # undefine the namespace
assert (typeof xx == SYM);
assert (typeof test_ns == SYM);

### test boolean functions ###
using "bool" (
    assert (not false);
    assert (not (not true));
    assert (not (not (not false)));
    assert (not (not (not (not true))));

    assert (or  true false);
    assert (or  false true);
    assert (not (or false false));

    assert (and true true);
    assert (not (and true false));
    assert (not (and false true));
    assert (not (and false false));

    assert (xor true  true  == false);
    assert (xor false false == false);
    assert (xor true  false == true);
    assert (xor false true  == true);
);

### test infix arithmetic functions ###
using "arith" (
    assert (1 + 1 == 2);
    assert (1 + 2 == 3);
    assert (2 + 1 == 3);
    assert (1 - 1 == 0);
    assert (2 - 1 == 1);
);

### test dictionary manipulation functions ###
using "dict" {
    xx := (dict_new ANY);
    xx.test_str = "test_dict";
    xx.test_int = 3;
    assert (xx.test_str == "test_dict");
    assert (xx.test_int == 3);
};

### test structure definition functions ###
using "struct" {
    struct test_struct (
        int field1;
        int field2 $= 9;
    );

    test_struct test_var;

    assert (test_var.field1 == 0);
    assert (test_var.field2 == 9);

    test_var.field1 = 7;
    test_var.field2 = 8;

    assert (test_var.field1 == 7);
    assert (test_var.field2 == 8);
};
