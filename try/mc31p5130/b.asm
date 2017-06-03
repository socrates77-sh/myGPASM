	org 0x00

	call loop1
	goto loop1

	lcall loop
	lgoto loop

	org 0x330
loop1:
	nop

	org 0x53f
loop:
	nop
	end
