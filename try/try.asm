;include ../header/mc30p01a.inc
#include mc30p011.inc


	;processor mc30p011
	;__CONFIG _CONFIG1, _BOR_24 & _MCLR_ON
	;__CONFIG _CONFIG2, _CP_ON & _OSCM_LP
	;__CONFIG 0x2003, _BOR_24 & _MCLR_ON
	;__CONFIG 0x2004, _CP_ON & _OSCM_LP

	title "title1"
	subtitle "subtitle1"


	constant pos=0x20
temp equ pos+10

	ORG 202h
start:
	dt 0
	addar temp
	andar temp
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


