all: forth

forth: forth.asm
	yasm forth.asm
	hexdump -C forth

test: test.asm
	yasm test.asm
	hexdump -C test

