

	title "title1"
	subtitle "subtitle1"


	constant pos=0x20
temp equ pos+10

	ORG 202h
start:
	dt 0
	movar 0x46
	addar temp
	andar temp
	andra 0x20
	;clrr d'11'
	clra
	;movar T0CNT
	;movar DDR1

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
	mular 2h
	mulra 2h

	adcar 23h

	call sub1
	goto lb1

	;lcall sub1
	;lgoto lb1
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


