#include mc30p011.inc

start:
	movra DDR1
	andra 0x20
	;clrr d'11'
	clra
	movai T0CNT

	bclr 22h, 1
	jbset 30h, 1

	addai 10
	movai 20
	retai 22

	clrwdt
	retie
	return
	stop
	nop
	daa
	dsa

	adcar 23h

	call sub1
	goto lb1
lb1:
	nop
sub1:
	daa
	return

	;int
	;option

	;iost 7

	;movra 44h
	;addar 43h

	;lgoto start
	;lcall start

	fill 0x102, 5
	end


