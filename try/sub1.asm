
processor mc30p01a
;#include "sub1.inc"

	;abc equ 1fh
	
	global abc;
	global aaa;
	
GPR_DATA		UDATA
abc RES 1;
aaa RES 1;	
	
	

	extern lab_abc

	;abc equ 10h




PROG CODE
funtion0:
	 ;movia ddd
	 movr abc
	 
	 nop
	 nop
	 goto lab_abc
	 
	 end
