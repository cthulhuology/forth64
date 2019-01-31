BITS 64

;; macros to make life easier
%define imm(x) x|0x80

;; compile a counted string with the data for an opcode
%macro op 1
align 16, db 0x90
%1_op:
	db %1_op.end - %1_op.start
.start:
%endmacro

%macro endop 0
.end:
	ret
%endmacro

%macro lex 3
align 16, db 0
%1_def:
dq %3
db %1_def.end - %1_def.start
.start:
	db %2
.end:
%endmacro

SECTION .text
;; this does the cold boot of the system
boot:

align 16, db 0x90
compile:
	mov rsi,rax
	xor rcx,rcx
	mov cl,[rsi]
	inc rsi
	mov rdi,[code_ptr]
	rep movsb
	mov [code_ptr],rdi
	ret

align 16, db 0x90
lookup:
	mov rsi,rax	; word being looked up
	mov rdi,[lexicon_ptr]
	sub rdi,24	; drop to start of the string field
.loop:
	mov cl,[rsi]	; load the 8 byte length
	inc cl		; add one to the length
	rep cmpsb
	je .found
	mov rsi,rax
	sub rdi,56	; move to the next word
	cmp rdi,lexicon_section	; check if we're still processing words
	jae .loop
.notfound:
	mov rax,0	; not found
	ret
.found:
	mov rax,[rdi-32] ; found, fetch value
	ret

	
	
	

;; Counted strings containing each compilcation unit

op neg
	neg rax
endop

op add
	pop rbx
	add rax,rbx
endop

op sub
	pop rbx
	xchg rax,rbx
	sub rax,rbx
endop

op mul
	pop rbx
	xor rdx,rdx
	imul rbx
	push rdx
endop

op div
	pop rbx
	xor rdx,rdx
	xchg rax,rbx
	idiv rbx
endop	

op mod
	pop rbx
	xor rdx,rdx
	xchg rax,rbx
	idiv rbx
	mov rax,rdx
endop	

op not
	not rax
endop

op and
	pop rbx
	and rax,rbx
endop

op or
	pop rbx
	or rax,rbx
endop

op xor
	pop rbx
	xor rax,rbx
endop

op equ
	pop rbx
	cmp rax,rbx
	je .truth
	xor rax,rax
	jmp .done 
.truth:
	xor rax,rax
	not rax	
.done:
endop

op lesser
	pop rbx
	cmp rax,rbx
	jl .truth
	xor rax,rax
	jmp .done 
.truth:
	xor rax,rax
	not rax	
.done:
endop

op greater
	pop rbx
	cmp rax,rbx
	jg .truth
	xor rax,rax
	jmp .done 
.truth:
	xor rax,rax
	not rax	
.done:
endop

op branch
	pop rbx	; flag
	pop rdx
	xchg rax,rdx
	test rbx,rbx
	jz .done 
	call rdx
.done:
endop

op fetch
	mov rax,[rax]
endop

op fetch_plus
	mov rbx,[rax]
	push rbx
	lea rax,[rax+8]
endop

op store
	pop rbx
	mov [rax],rbx
	pop rax
endop

op store_plus
	pop rbx
	mov [rax],rbx
	lea rax,[rax+8]
endop

op comma
	mov rbx,[data_ptr]
	mov [rbx],rax
	lea rbx,[rbx+8]
	mov [data_ptr],rbx
	pop rax
endop

op ret
	ret
endop

op call
	pop rbx
	xchg rax,rbx
	call rbx
endop

op lit
	push rax
	mov rax, qword 0
endop

op quote
	nop
endop

op tick
	nop
endop

op comment
	nop
endop

op comment_end
	nop
endop

op immediate
	nop
endop

op immediate_end
	nop
endop

op string
	nop
endop

;; This is the source code section
code_section:
	align 8, db 0x90
lexicon_const:
	nop
	align 8, db 0x90
code_const:
	nop
	align 8, db 0x90
_var:	
	nop
	align 8, db 0x90


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

lex lexicon,'lexicon',lexicon_section
lex lexptr,'#lex',lexicon_ptr
lex code,'code',code_section
lex codeptr,'#code',code_ptr
lex data,'data',data_section
lex dataptr,'#data',data_ptr
lex scratch,'scratch',scratch_section
lex scratchptr,'#scratch',scratch_ptr
lex pad,'pad',pad_section
lex patptr,'#pad',pad_ptr
lex source,'source',source_section
lex sourceptr,'#source',source_ptr
lex plus,'+',add_op
lex minus,'-',sub_op
lex star,'*',mul_op
lex slash,'/',div_op
lex percent,'%',mod_op
lex negate,'~',neg_op
lex amp,'&',and_op
lex bar,'|',or_op
lex hat,'^',xor_op
lex bang,'!',not_op
lex equ,'=',equ_op
lex lesser,'<',lesser_op
lex greater,'>',greater_op
lex branch,'?',branch_op
lex fetch,'@',fetch_op
lex fetch_plus,'@+',fetch_plus_op
lex store,'$',store_op
lex store_plus,'$+',store_plus_op
lex comma,',',comma_op
lex return,';',ret_op
lex call_word,'.',call_op
lex lit,'#',lit_op
lex quote,'`',quote_op
lex tick,39,tick_op
lex string,'"',string_op
lex comments,'(',comment_op
lex comm_end,')',comment_end_op
lex immediate,'[',immediate_op
lex immediate_end,']',immediate_end_op

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

