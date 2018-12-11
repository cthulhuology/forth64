all: forth

forth: forth.asm
	yasm forth.asm
	hexdump -C forth
