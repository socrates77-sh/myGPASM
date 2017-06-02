#include "mc30p011.inc"

	GLOBAL	init_timer1
	GLOBAL	init_ports

;----------------------------------------------------------------------
;  ******************* INITIALIZE PORTS  *************************
;----------------------------------------------------------------------
INIT_CODE		CODE

init_ports
	movai 0xff
	;iost 5
	;iost 6
	return

;----------------------------------------------------------------------
;  ******************* INITIALIZE TIMER1 MODULE  *******************
;----------------------------------------------------------------------
init_timer1
	movai b'00110000'  
	movra MCR
	return

	end
