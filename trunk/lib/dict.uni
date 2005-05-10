import "base";

defaty "REF";

defsty "." LINFIX (add PREC_NOR 4);
define "." (ANY->RAW->DEP) (\x(\y(dict_get x (str y))));
define "." (ANY->RAW->REF)
(\x(\y{
	u := dict_new REF;
	dict_set u "obj" x;
	dict_set u "key" (str y);
	return u;
}));

define "=" (REF->ANY->VOID) (\x(\y(set x y)));
