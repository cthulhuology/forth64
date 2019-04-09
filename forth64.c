// forth64.c
//
// this is a bootstrapping exercise to test out some ideas for the engine
// the general rules are that:
//
// 	1. no functions should be called.
// 	2. no jumps should be invoked except for the main loop
// 	3. each iteration of the loop all data should be passed
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

// We are going to define our stack item as a uint64_t
typedef unsigned long long stack;

// For each opcode in our MISC we define an opcode
// a few of these will have immediate values
enum ops {
	NOP,		// no operation
	LIT,		// imm, load the immediate value into tos
	RET,		// return to address at rtos
	CALL,		// imm, push current ip to rs, and set ip to immediate value
	JMP,		// imm, set ip to immediate value
	BR,		// imm, set ip to immediate value if tos is non-0
	ADD,		// add nos + tos
	SUB,		// subtract nos - tos
	MUL,		// multiply nos * tos
	DIV,		// divide nos / tos
	MOD,		// modulus nos % tos
	NEG,		// negate tos
	AND,		// binary and nos & tos
	OR,		// binary or nos | tos
	XOR,		// binary xor nos ^ tos
	NOT,		// binary compliment tos
	EQ,		// compare nos == tos
	LT,		// compare nos < tos
	GT,		// compare nos > tos
	STORE,		// store nos at tos *tos = nos
	STORE_PLUS,	// store nos at tos, increment tos
	FETCH,		// fetch value at tos to tos
	FETCH_PLUS,	// fetch value at tos to nos, increment tos
	COMMA,		// write the value of tos to a code address
	QUOTE,		// imm, write immedate value to code address
	TICK,		// imm, place address of definition on stack
	COMMENT,	// consume input buffer till COMMENT_END
	COMMENT_END,	// resume compilation
	IMMEDIATE,	// switch to immediate scratchpad
	IMMEDIATE_END,	// evaluate immediate scratchpad & resume compilation
	STRING,		// imm, place address of compiled string literal in tos
	OPEN,		// open a file, leave fd in tos
	READ,		// read cell from fd in tos
	WRITE,		// write nos cell to fd in tos
	CLOSE,		// close fd in tos
	RECV,		// receive a message from socket in tos
	SEND,		// send nos to socket in tos
	SOCKET,		// open a socket fd in tos
	LISTEN,		// listen to port nos on socket tos

	HALT = 0xcafebabe
};

// dump the contents of the MISC registers and the data and retrn stacks
void dump(stack* cp, stack* dsp, stack* rsp, stack tos, stack nos, stack op) {
	fprintf(stdout,"tos: %-16p\tds[0] %-16p\trs[0] %-16p\n", tos, dsp[0], rsp[0]);
	fprintf(stdout,"nos: %-16p\tds[1] %-16p\trs[1] %-16p\n", nos, dsp[1], rsp[1]);
	fprintf(stdout,"cp:  %-16p\tds[2] %-16p\trs[2] %-16p\n", cp , dsp[2], rsp[2]);
	fprintf(stdout,"op:  %-16p\tds[3] %-16p\trs[3] %-16p\n", op , dsp[3], rsp[3]);
	fprintf(stdout,"     \t\t\tds[4] %-16p\trs[4] %-16p\n",      dsp[4], rsp[4]);
	fprintf(stdout,"     \t\t\tds[5] %-16p\trs[5] %-16p\n",      dsp[5], rsp[5]);
	fprintf(stdout,"     \t\t\tds[6] %-16p\trs[6] %-16p\n",      dsp[6], rsp[6]);
	fprintf(stdout,"     \t\t\tds[7] %-16p\trs[7] %-16p\n",      dsp[7], rsp[7]);
}

// run the program at code, with the stacks dsp and rsp
void forth(stack* code, stack* dsp, stack* rsp) {

	stack *cp = code;
	stack op = *cp;
	stack tos;
	stack nos;
	
	for(;;) switch(op) {
	case NOP:
		op = *(++cp);
		break;
	case LIT:
		*(++dsp) = *(++cp);
		op = *(++cp);
		break;
	case RET:
		cp = (stack*)*rsp;
		--rsp;
		op = *(++cp);
		break;
	case CALL:
		*(++rsp) = (stack)(cp+1);
		cp = cp + *(++cp);
		op = *cp;
		break;
	case JMP:
		cp = cp + *(++cp);
		op = *cp;
		break;
	case BR:
		tos = *dsp;
		--dsp;
		cp =  (!tos) ? cp + *(cp+1) : cp+2; // then jump
		op = *cp;
		break;
	case ADD:
		tos = *dsp;
		nos = *(--dsp);
		*dsp = tos + nos;
		op = *(++cp);
		break;
	case SUB:
		tos = *dsp;
		nos = *(--dsp);
		*dsp = nos - tos;
		op = *(++cp);
		break;
	case MUL:
		tos = *dsp;
		nos = *(--dsp);
		*dsp = nos * tos;
		op = *(++cp);
		break;
	case DIV:
		tos = *dsp;
		nos = *(--dsp);
		*dsp = nos / tos;
		op = *(++cp);
		break;
	case MOD:
		tos = *dsp;
		nos = *(--dsp);
		*dsp = nos % tos;
		op = *(++cp);
		break;
	case NEG: // a ~ -- -a
		*dsp = -(*dsp);
		op = *(++cp);
		break;
	case AND: // a b & -- a&b
		tos = *dsp;
		nos = *(--dsp);
		*dsp = nos & tos;
		op = *(++cp);
		break;
	case OR: // a b | -- a|b
		tos = *dsp;
		nos = *(--dsp);
		*dsp = nos | tos;
		op = *(++cp);
		break;
	case XOR: // a b ^ -- a^b
		tos = *dsp;
		nos = *(--dsp);
		*dsp = nos ^ tos;
		op = *(++cp);
		break;
	case NOT: // a ! -- !a
		*dsp = ~(*dsp);
		op = *(++cp);
		break;
	case EQ: // a b = -- flag 
		tos = *dsp;
		nos = *(--dsp);
		*dsp = (nos == tos) ? -1 : 0;
		op = *(++cp);
		break;
	case LT: // a b < -- flag
		tos = *dsp;
		nos = *(--dsp);
		*dsp = ((long long)(nos) < (long long)(tos)) ? -1 : 0;
		op = *(++cp);
		break;
	case GT: // a b > -- flag
		tos = *dsp;
		nos = *(--dsp);
		*dsp = ((long long)nos > (long long)tos) ? -1 : 0;
		op = *(++cp);
		break;
	case STORE: // value addr $ -- 
		tos = *dsp;
		nos = *(--dsp);
		*(stack*)tos = nos;
		--dsp;
		op = *(++cp);
		break;
	case STORE_PLUS: // value addr $+ -- addr+1
		tos = *dsp;
		nos = *(--dsp);
		*(stack*)tos = nos;
		*(dsp) = tos + sizeof(stack);
		op = *(++cp);
		break;
	case FETCH:
		tos = *dsp;
		*dsp = *(stack*)tos;
		op = *(++cp);
		break;
	case FETCH_PLUS:
		tos = *dsp;
		*dsp = *(stack*)tos;
		*(++dsp) = tos + sizeof(stack);
		op = *(++cp);
		break;
	case COMMA:
		op = *(++cp);
		break;
	case QUOTE:
		op = *(++cp);
		break;
	case TICK:
		op = *(++cp);
		break;
	case COMMENT:
		op = *(++cp);
		break;
	case COMMENT_END:
		op = *(++cp);
		break;
	case IMMEDIATE:
		op = *(++cp);
		break;
	case IMMEDIATE_END:
		op = *(++cp);
		break;
	case STRING:
		op = *(++cp);
		break;
	case OPEN:
		op = *(++cp);
		break;
	case READ:
		op = *(++cp);
		break;
	case WRITE:
		op = *(++cp);
		break;
	case CLOSE:
		op = *(++cp);
		break;
	case RECV:
		op = *(++cp);
		break;
	case SEND:
		op = *(++cp);
		break;
	case SOCKET:
		op = *(++cp);
		break;
	case LISTEN:
		op = *(++cp);
		break;
	case HALT:
	default:
		return;
	}
}

// Runs the code at code, and returns 0 if the data and retn
// stacks match the interpreted stacks at the end.
// All stacks are initialized to 0
// returns non-zero if they do not match

stack memory[8]; 			// a sample RAM for testing
stack ds[] = {0,0,0,0,0,0,0,0};		// a sample data stack for testing
stack rs[] = {0,0,0,0,0,0,0,0};		// a sample return stack for testing

int test(stack* code, stack* data, stack* retn, stack* mem) {
	int retval = 0;
	memset(memory,0,sizeof(memory));
	memset(ds,0,sizeof(ds));
	memset(rs,0,sizeof(rs));
	forth(code,ds,rs);
<<<<<<< HEAD
	fprintf(stderr,"test result\n");
	dump(0,ds,rs,0,0,0);
	dump(0,data,retn,0,0,0);
	return memcmp(ds,data,8*sizeof(stack)) | memcmp(rs,retn,8*sizeof(stack));
=======
	retval = memcmp(ds,data,8*sizeof(stack)) | memcmp(rs,retn,8*sizeof(stack)) | memcmp(memory,mem,8*sizeof(stack));
	if (retval) {
		fprintf(stderr,"test result\n");
		dump(0,ds,rs,0,0,0);
		dump(0,data,retn,0,0,0);
	}
	return retval;
>>>>>>> f90f3b5b11dec68212365e16676878a2a6f44758
}

struct test_struct {
	stack data[8];
	stack retn[8];
	stack code[8];
	stack memory[8];
};
typedef struct test_struct Test;


// Run the interpreter on test programs
int main(int argc, char** argv) {
	int i = 0;
<<<<<<< HEAD
	int max_tests = 8;
=======
>>>>>>> f90f3b5b11dec68212365e16676878a2a6f44758
	Test tests[] = {
	{ 
		.data = { 0,0,0,0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = {NOP, NOP, NOP, NOP, NOP, NOP, NOP, HALT },
		.memory = { 0,0,0,0,0,0,0,0 }
	},
	{
		.data = { 0,1,0,0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, 1, NOP, NOP, NOP, NOP, NOP, HALT },
		.memory = { 0,0,0,0,0,0,0,0 }
	},
	{
		.data = { 0,3,2,0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, 1, LIT, 2, ADD, NOP, NOP, HALT },
		.memory = { 0,0,0,0,0,0,0,0 }
	},
	{
		.data = { 0,-3,2,0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, 1, LIT, 2, ADD, NEG, NOP, HALT },
		.memory = { 0,0,0,0,0,0,0,0 }
	},
	{
		.data = { 0,5,10,0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, 15, LIT, 10, SUB, NOP, NOP, HALT },
		.memory = { 0,0,0,0,0,0,0,0 }
	},
	{
		.data = { 0,50,10,0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, 5, LIT, 10, MUL, NOP, NOP, HALT },
		.memory = { 0,0,0,0,0,0,0,0 }
	},
	{
		.data = { 0,5,10,0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, 50, LIT, 10, DIV, NOP, NOP, HALT },
		.memory = { 0,0,0,0,0,0,0,0 }
	},
	{
		.data = { 0,0,10,0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, 50, LIT, 10, MOD, NOP, NOP, HALT },
		.memory = { 0,0,0,0,0,0,0,0 }
	},
	{
		.data = { 0,50,-10,0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, 50, LIT, 10, NEG, NOP, NOP, HALT },
		.memory = { 0,0,0,0,0,0,0,0 }
	},
	{
		.data = { 0,2,2,0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, 7, LIT, 2, AND, NOP, NOP, HALT },
		.memory = { 0,0,0,0,0,0,0,0 }
	},
	{
		.data = { 0,15,8,0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, 7, LIT, 8, OR, NOP, NOP, HALT },
		.memory = { 0,0,0,0,0,0,0,0 }
	},
	{
		.data = { 0,1,-2,0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, -1, LIT, -2, XOR, NOP, NOP, HALT },
		.memory = { 0,0,0,0,0,0,0,0 }
	},
	{
		.data = { 0,-1,1,0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, -1, LIT, -2, NOT, NOP, NOP, HALT },
		.memory = { 0,0,0,0,0,0,0,0 }
	},
	{
		.data = { 0,0,-2,0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, -1, LIT, -2, EQ, NOP, NOP, HALT },
		.memory = { 0,0,0,0,0,0,0,0 }
	},
	{
		.data = { 0,-1,-1,0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, -1, LIT, -1, EQ, NOP, NOP, HALT },
		.memory = { 0,0,0,0,0,0,0,0 }
	},
	{
		.data = { 0,0,-2,0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, -1, LIT, -2, LT, NOP, NOP, HALT },
		.memory = { 0,0,0,0,0,0,0,0 }
	},
	{
		.data = { 0,-1,-2,0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, -1, LIT, -2, GT, NOP, NOP, HALT },
		.memory = { 0,0,0,0,0,0,0,0 }
	},
	{
		.data = { 0,66,&memory[1],0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, 66, LIT, &memory[1], STORE, NOP, NOP, HALT },
		.memory = { 0,66,0,0,0,0,0,0 }
	},
	{
		.data = { 0,&memory[2],&memory[1],0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, 66, LIT, &memory[1], STORE_PLUS, NOP, NOP, HALT },
		.memory = { 0,66,0,0,0,0,0,0 }
	},
	{
		.data = { 0,66,&memory[1],0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, 66, LIT, &memory[1], STORE, LIT, &memory[1], FETCH, HALT },
		.memory = { 0,66,0,0,0,0,0,0 }
	},
	{
		.data = { 0,66,&memory[2],0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, 66, LIT, &memory[1], STORE, LIT, &memory[1], FETCH_PLUS, HALT },
		.memory = { 0,66,0,0,0,0,0,0 }
	},
	{
		.data = { 0,-15,0,0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, 15, JMP, 3, LIT, 1, NEG, HALT }
	},
	{
		.data = { 0,0,0,0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { CALL, 1, NOP, NOP, NOP, NOP, NOP, HALT }
	},
	};
	int max_tests = 21;
	for (i = 0; i < max_tests; ++i) {
		fprintf(stderr, "test[%d] %s\n", i, test(tests[i].code,tests[i].data,tests[i].retn,tests[i].memory) ? "fail": "pass");	
	}
	return 0;
}
