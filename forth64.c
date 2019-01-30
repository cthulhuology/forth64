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
		cp = (stack*)*(++cp);
		op = *cp;
		break;
	case JMP:
		cp = (stack*)*(++cp);
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
		*dsp = nos == tos;
		op = *(++cp);
		break;
	case LT: // a b < -- flag
		tos = *dsp;
		nos = *(--dsp);
		*dsp = nos < tos;
		op = *(++cp);
		break;
	case GT: // a b > -- flag
		tos = *dsp;
		nos = *(--dsp);
		*dsp = nos > tos;
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
		op = *(++cp);
		break;
	case FETCH:
		op = *(++cp);
		break;
	case FETCH_PLUS:
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
int test(stack* code, stack* data, stack* retn) {
	stack ds[] = {0,0,0,0,0,0,0,0};
	stack rs[] = {0,0,0,0,0,0,0,0};
	forth(code,ds,rs);
//	fprintf(stderr,"test result\n");
//	dump(0,ds,rs,0,0,0);
//	dump(0,data,retn,0,0,0);
	return memcmp(ds,data,8*sizeof(stack)) | memcmp(rs,retn,8*sizeof(stack));
}

struct test_struct {
	stack data[8];
	stack retn[8];
	stack code[8];
};
typedef struct test_struct Test;

// Run the interpreter on test programs
int main(int argc, char** argv) {
	int i = 0;
	int max_tests = 5;
	Test tests[] = {
	{ 
		.data = { 0,0,0,0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = {NOP, NOP, NOP, NOP, NOP, NOP, NOP, HALT }
	},
	{
		.data = { 0,1,0,0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, 1, NOP, NOP, NOP, NOP, NOP, HALT }
	},
	{
		.data = { 0,3,2,0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, 1, LIT, 2, ADD, NOP, NOP, HALT }
	},
	{
		.data = { 0,-3,2,0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, 1, LIT, 2, ADD, NEG, NOP, HALT }
	},
	{
		.data = { 0,5,10,0,0,0,0,0 },
		.retn = { 0,0,0,0,0,0,0,0 },
		.code = { LIT, 15, LIT, 10, SUB, NOP, NOP, HALT }
	},
	};
	for (i = 0; i < max_tests; ++i) {
		fprintf(stderr, "test[%d] %s\n", i, test(tests[i].code,tests[i].data,tests[i].retn) ? "fail": "pass");	
	}
	return 0;
}
