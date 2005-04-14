import "base";

#### dummy tokens #####
defsty "the" DUMMY PREC_SEP;
defsty "a"   DUMMY PREC_SEP;
defsty "an"  DUMMY PREC_SEP;
defsty "of"  DUMMY PREC_SEP;
defsty "to"  DUMMY PREC_SEP;
defsty "in"  DUMMY PREC_SEP;

defaty "REF";
defaty "BE_BLOCK";
defaty "BE_TYPE";
defaty "BE_REF";

defsty "s" PREFIX (add PREC_NOR 20);
define "s" (RAW->DEP) (\x x);

defsty "be" LINFIX (add PREC_NOR 1);

define "be" (RAW->ANY->BE_BLOCK)
  (\x(\y{
    defmap "r" (dict_new BE_BLOCK);
    dict_set r "name" (rstr x);
    dict_set r "value" y;
    return r;
  }));

define "be" (REF->ANY->BE_REF)
  (\x(\y{
    defmap "r" (dict_new BE_REF);
    dict_set r "value" y;
    dict_set r "obj" (dict_get x "obj");
    dict_set r "key" (dict_get x "key");
    return r
  }));

define "let" (BE_BLOCK->VOID)
  (\x(defmap (dict_get x "name") (dict_get x "value")));

define "let" (BE_REF->VOID)
  (\x(dict_set (dict_get x "obj") (dict_get x "key") (dict_get x "value")));

defsty "'" LINFIX (add PREC_NOR 4);
define "'" (ANY->RAW->DEP) (\x(\y(dict_get x (str y))));
define "'" (ANY->RAW->REF)
  (\x(\y{
    defmap "r" (dict_new REF);
    dict_set r "obj" x;
    dict_set r "key" (str y);
    return r;
  }));

defsty "list" PREFIX (add PREC_NOR 3);
define "list" (TYPE->DEP) (\x(list_new x));

defsty "new" PREFIX (add PREC_NOR 3);
define "new" (TYPE->DEP) (\x(dict_new x));

defsty "type" PREFIX (add PREC_NOR 3);

define "print" (ANY->VOID) (\x(raw_out (str x); raw_out (cstr "\n")));

defsty "." EOS PREC_SEP;

define "append" (ANY->ANY->VOID) (\x(\y(list_add y x)));

defsty "everything" LINFIX (sub PREC_NOR 1);
define "everything" (ANY->ANY->VOID) (\f(\x(list_vis x f)));

defaty "THEN_BLOCK";

defsty "if" PREFIX (sub PREC_NOR 4);
define "if" (BOOL->THEN_BLOCK->VOID)
  (\x(\y(
    ifeq true x (dict_get y "true") (dict_get y "false")
  )));

defsty "then" PREFIX (sub PREC_NOR 3);
define "then" (THEN_BLOCK->THEN_BLOCK) (\x x);
define "then" (RAW->THEN_BLOCK)
  (\x{
    let r be a new THEN_BLOCK.
    dict_setr r "true"  x.
    dict_setr r "false" void.
    return r.
  });

defsty "otherwise" LINFIX (sub PREC_NOR 2);
define "otherwise" (RAW->RAW->THEN_BLOCK)
  (\x(\y{
    let r be a new THEN_BLOCK.
    dict_setr r "true"  x.
    dict_setr r "false" y.
    return r.
  }));

##########################
let Student be the type "Student".
let Alan be a new Student.
let Alan's name  be "Alan Turing".
let Alan's major be "Mathematics".
let Alan's minor be "Computer Science".

print Alan's name.
print Alan's major.
print Alan's minor.

let StudentList be a list of Student.
append Alan to StudentList.

let ll be a list of STR.
append "Chen" to ll.
append "Ji" to ll.
append "Sex" to ll.

# let email be a procedure that
# given from, to, subject, body
# it will:

