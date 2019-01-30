all: forth forth64

forth: forth.asm
	yasm forth.asm
	hexdump -C forth

test: test.asm
	yasm test.asm
	hexdump -C test

forth64: forth64.c
	gcc -o forth64  forth64.c

