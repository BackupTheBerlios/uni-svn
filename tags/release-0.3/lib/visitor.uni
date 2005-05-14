import "struct";

struct INSEQ (
    symbol var;
    any    seq;
);

defsty "in" LINFIX (add PREC_NOR 1);
define "in" (SYM->ANY->INSEQ)
(\x(\y{
    INSEQ inseq;
    inseq.var = x;
    inseq.seq = y;
    return inseq;
}));

define "foreach" (INSEQ->RAW->VOID)
(\inseq(\action(list_vis inseq.seq (\\ inseq.var action))));
