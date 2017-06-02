
;	processor mc32p21
;	__idlocs  0x1
;	__CONFIG 0x2000, 0xff33
;	__CONFIG 0x2001, 0x6677

	constant pos=0x20
#define table 0x1234

temp equ pos+10h

	ORG 100h

start:

	movai high table
	movai high (table + 0x0010)
	movai high (table - 0x0010)
	;movai high (table |0x0010)
	movra 0x111
	;adcai 0x11
	isbcai 0x11
	asbcai 0x11
	movai low table
	movra 0x12
	;mulra 0x13
	lgoto 0x0122
	lcall main

	org 0x03fc
main:
	movai 0x11
	
	
;table:
;	dw 0x55aa, 0x8877

	end
