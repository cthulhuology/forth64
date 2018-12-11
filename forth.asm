
BITS 64

;;; This is the lexicon, we'll put this somewhere eventually
lexicon:	dq lexicon
		db 7,'lexicon'
code:		dq 0xdeadbeef
		db 4,'code',0,0,0
scratch:	dq 0xcafebabe
		db 7,'scratch'
pad:		dq 0xfeedface
		db 3,'pad',0,0,0,0
source:		dq 0xc0de
		db 6,'source',0
plus:		dq add_op
		db 1,'+',0,0,0,0,0,0
minus:		dq sub_op
		db 1,'-',0,0,0,0,0,0
star:		dq mul_op
		db 1,'*',0,0,0,0,0,0
slash:		dq div_op
		db 1,'/',0,0,0,0,0,0
percent:	dq mod_op
		db 1,'%',0,0,0,0,0,0
negate:		dq neg_op
		db 1,'~',0,0,0,0,0,0
amp:		dq and_op
		db 1,'&',0,0,0,0,0,0
pipe:		dq or_op
		db 1,'|',0,0,0,0,0,0
carrot:		dq xor_op
		db 1,'^',0,0,0,0,0,0
bang:		dq not_op
		db 1,'!',0,0,0,0,0,0
equal:		dq equ_op
		db 1,'=',0,0,0,0,0,0
lesser:		dq lesser_op
		db 1,'<',0,0,0,0,0,0
greater:	dq greater_op
		db 1,'>',0,0,0,0,0,0
branch:		dq branch_op
		db 1,'?',0,0,0,0,0,0
fetch:		dq fetch_op
		db 1,'@',0,0,0,0,0,0
fetch_plus:	dq fetch_plus_op
		db 2,'@+',0,0,0,0,0
store:		dq store_op
		db 1,'$',0,0,0,0,0,0
store_plus:	dq store_plus_op
		db 1,'$+',0,0,0,0,0
comma:		dq comma_op
		db 1,',',0,0,0,0,0,0
return:		dq ret_op
		db 1,';',0,0,0,0,0,0
call_word:	dq call_op
		db 1,'.',0,0,0,0,0,0
lit:		dq lit_op
		db 1,'#',0,0,0,0,0,0
quote:		dq quote_op
		db 1,'`',0,0,0,0,0,0
tick:		dq tick_op
		db 1,39,0,0,0,0,0,0
string:		dq string_op
		db 1,'\"',0,0,0,0,0,0
comments:	dq comment_op
		db 1,'(',0,0,0,0,0,0
comm_end:	dq comment_end_op
		db 1,')',0,0,0,0,0,0
immediate:	dq immediate_op
		db 1,'[',0,0,0,0,0,0


;; This is the code section, need to put this somewhere too
lexicon_const:		nop
code_const:		nop
_var:		nop
add_op:			nop
sub_op:			nop
mul_op:			nop
div_op:			nop
mod_op:			nop
neg_op:			nop
and_op:			nop
or_op:			nop
xor_op:			nop
not_op:			nop
equ_op:			nop
lesser_op:		nop
greater_op:		nop
branch_op:		nop
fetch_op:		nop
fetch_plus_op:		nop
store_op:		nop
store_plus_op:		nop
comma_op:		nop
ret_op:			nop
call_op:		nop
lit_op:			nop
quote_op:		nop
tick_op:		nop
string_op:		nop
comment_op:		nop
comment_end_op:		nop
immediate_op:		nop
