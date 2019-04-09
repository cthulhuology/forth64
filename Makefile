all: forth forth64

forth: forth.asm
	yasm forth.asm
	hexdump -C forth

test: forth64
	./forth64

forth64: forth64.c
	gcc -o forth64  forth64.c

