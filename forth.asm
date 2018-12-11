
BITS 64

;; macros to make life easier
%define imm(x) x|0x80


SECTION .text
;; this does the cold boot of the system
boot:
			nop

;; This is the source code section
code_section:
			align 8, db 0x90
lexicon_const:		nop
			align 8, db 0x90
code_const:		nop
			align 8, db 0x90
_var:			nop
			align 8, db 0x90

add_op:			
			mov rbx, 0xfeedfacedeadbeef
			pop rbx
			add rax,rbx
			ret
			align 8, db 0x90

sub_op:			nop
			align 8, db 0x90
mul_op:			nop
			align 8, db 0x90
div_op:			nop
			align 8, db 0x90
mod_op:			nop
			align 8, db 0x90
;; unary negation
neg_op:			neg rax
			ret
			align 8, db 0x90

and_op:			nop
			align 8, db 0x90
or_op:			nop
			align 8, db 0x90
xor_op:			nop
			align 8, db 0x90

;; unary not
not_op:			not rax
			ret
			align 8, db 0x90

equ_op:			nop
			align 8, db 0x90
lesser_op:		nop
			align 8, db 0x90
greater_op:		nop
			align 8, db 0x90
branch_op:		nop
			align 8, db 0x90
fetch_op:		nop
			align 8, db 0x90
fetch_plus_op:		nop
			align 8, db 0x90
store_op:		nop
			align 8, db 0x90
store_plus_op:		nop
			align 8, db 0x90
comma_op:		nop
			align 8, db 0x90
ret_op:			nop
			align 8, db 0x90
call_op:		nop
			align 8, db 0x90
lit_op:			push rax
			mov rax,0xcafebabecafebabe
			align 8, db 0x90
quote_op:		nop
			align 8, db 0x90
tick_op:		nop
			align 8, db 0x90
string_op:		nop
			align 8, db 0x90
comment_op:		nop
			align 8, db 0x90
comment_end_op:		nop
			align 8, db 0x90
immediate_op:		nop

;; this is the pointer to the current end of the code section
code_end:


;; The scratch section is used for compilation of immediate code
align 8192,db 0
scratch_section:
db 'scratch'
scratch_end:

;; This is the start of the lexicon
align 8192,db 0
SECTION .data
lexicon_section:
lexicon:	dq lexicon_section
		db 7,'lexicon'
lexicon_star:	dq lexicon_ptr
		db 4,'*lex',0,0,0
code:		dq code_section
		db 4,'code',0,0,0
code_star:	dq code_ptr
		db 5,'*code',0,0
data:		dq data_section
		db 4,'data',0,0,0
data_star:	dq data_ptr
		db 5,'*data',0,0
scratch:	dq scratch_section
		db 7,'scratch'
scratch_star:	dq scratch_ptr
		db 5,'*scra',0,0
pad:		dq pad_section
		db 3,'pad',0,0,0,0
pad_star:	dq pad_ptr
		db 4,'*pad',0,0,0
source:		dq source_section
		db 6,'source',0
plus:		dq add_op
		db imm(1),'+',0,0,0,0,0,0
minus:		dq sub_op
		db imm(1),'-',0,0,0,0,0,0
star:		dq mul_op
		db imm(1),'*',0,0,0,0,0,0
slash:		dq div_op
		db imm(1),'/',0,0,0,0,0,0
percent:	dq mod_op
		db imm(1),'%',0,0,0,0,0,0
negate:		dq neg_op
		db imm(1),'~',0,0,0,0,0,0
amp:		dq and_op
		db imm(1),'&',0,0,0,0,0,0
pipe:		dq or_op
		db imm(1),'|',0,0,0,0,0,0
carrot:		dq xor_op
		db imm(1),'^',0,0,0,0,0,0
bang:		dq not_op
		db imm(1),'!',0,0,0,0,0,0
equal:		dq equ_op
		db imm(1),'=',0,0,0,0,0,0
lesser:		dq lesser_op
		db imm(1),'<',0,0,0,0,0,0
greater:	dq greater_op
		db imm(1),'>',0,0,0,0,0,0
branch:		dq branch_op
		db imm(1),'?',0,0,0,0,0,0
fetch:		dq fetch_op
		db imm(1),'@',0,0,0,0,0,0
fetch_plus:	dq fetch_plus_op
		db imm(2),'@+',0,0,0,0,0
store:		dq store_op
		db imm(1),'$',0,0,0,0,0,0
store_plus:	dq store_plus_op
		db imm(2),'$+',0,0,0,0,0
comma:		dq comma_op
		db imm(1),',',0,0,0,0,0,0
return:		dq ret_op
		db imm(1),';',0,0,0,0,0,0
call_word:	dq call_op
		db imm(1),'.',0,0,0,0,0,0
lit:		dq lit_op
		db imm(1),'#',0,0,0,0,0,0
quote:		dq quote_op
		db imm(1),'`',0,0,0,0,0,0
tick:		dq tick_op
		db imm(1),39,0,0,0,0,0,0
string:		dq string_op
		db imm(1),'"',0,0,0,0,0,0
comments:	dq comment_op
		db imm(1),'(',0,0,0,0,0,0
comm_end:	dq comment_end_op
		db imm(1),')',0,0,0,0,0,0
immediate:	dq immediate_op
		db imm(1),'[',0,0,0,0,0,0

;; this is the pointer to the end of the current lexicon
lex_end:


;; this is the data segment for immediate code
align 8192,db 0
pad_section:
db 'pad'

pad_end:
times 1000 dq 0

;; this is the data segment for runtime code
data_section:
lexicon_ptr:	dq lex_end
code_ptr: 	dq code_end
scratch_ptr:	dq scratch_end
pad_ptr:	dq pad_end
data_ptr:	dq data_end
source_ptr:	dq source_end

data_end:

;; This is the start of the source code to our program
align 8192,db 0
source_section:
db 'source'
source_end:

align 8192,db 0

