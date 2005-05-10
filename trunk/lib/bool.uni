import "base";

define "if"  (BOOL->RAW->RAW->UPR)  (\x(\t(\f(ifeq x true t f))));
define "not" (BOOL->BOOL) (\x(ifeq true x false true));
define "and" (BOOL->BOOL->BOOL) (\x(\y(ifeq true x (ifeq true y true false) false)));
define "or"  (BOOL->BOOL->BOOL) (\x(\y(ifeq true x true (ifeq true y true false))));
define "xor" (BOOL->BOOL->BOOL) (\x(\y(and (or x y) (not (and x y)))));
