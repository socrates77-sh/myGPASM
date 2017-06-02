;list
	#include mc30p011.inc               ; processor specific variable definitions
	__CONFIG _CONFIG0, (_FCPU_4T & _WDT_OFF)
	__CONFIG _CONFIG1, ( _SMT_OFF & _CP_ALL)

#include  "mastri2c.inc"             ;
#include  "i2ccomm1.inc"             ; required include file

#define  ADDRESS     0x01                ; Slave I2C address

;----------------------------------------------------------------------
;   ********************* RESET VECTOR LOCATION  ********************
;----------------------------------------------------------------------
RESET_VECTOR  CODE    0x000              ; processor reset vector
	movai high start
	movra PCLATH
	goto start

;----------------------------------------------------------------------
;  ******************* INTERRUPT VECTOR LOCATION  *******************
;----------------------------------------------------------------------
INT_VECTOR   CODE    0x004               ; interrupt vector location
	movra w_temp
	movar STATUS
	clrr status_temp
	movra status_temp
	movar PCLATH
	movra pclath_temp
	clrr PCLATH

	jbset P1, 2
	goto test_buscoll
	call service_i2c

test_buscoll
	jbset P1, 1
	goto test_timer1
	jbset P1, 4
	goto test_timer1
	call service_buscoll


; TEST FOR TIMER1 ROLLOVER EVENT		
test_timer1
	jbset P1, 4
	goto exit_isr
	call service_i2c

exit_isr 
	clrr STATUS
	movar pclath_temp
	movra PCLATH
	movar status_temp
	movra STATUS
	retie


;----------------------------------------------------------------------
;   ******************* MAIN CODE START LOCATION  ******************
;----------------------------------------------------------------------
MAIN    CODE
start
	call init_ports
	call init_timer1
	call init_i2c
	clrr eflag_event
	clrr sflag_event
	clrr i2cState
	call CopyRom2Ram
	call init_vars


;*******************************************************************
;                     MAIN LOOP BEGINS HERE
;******************************************************************* 
main_loop 
	clrwdt
	jbclr eflag_event, ack_error
	call service_ackerror
	jbset sflag_event, rw_done
	goto main_loop

	call string_compare
	call init_vars
	goto main_loop

;----------------------------------------------------------------------
;   *************** Bus Collision Service Routine ******************
;----------------------------------------------------------------------
service_buscoll  
	clrr i2cState
	call init_vars
	return

;----------------------------------------------------------------------
;   ************* Acknowledge Error Service Routine ***************
;----------------------------------------------------------------------
service_ackerror
	bclr eflag_event, ack_error
	clrr i2cState
	call init_vars
	return

;----------------------------------------------------------------------
;   *****  INITIALIZE VARIABLES USED IN SERVICE_I2C FUNCTION  ******
;----------------------------------------------------------------------
init_vars
	movai 21
	movra write_count
	movra read_count

	movai write_string
	movra write_ptr

	movai read_string
	movra read_ptr

	movai ADDRESS
	movra temp_address
	return

;----------------------------------------------------------------------
;  *******************  Compare Strings   ************************
;----------------------------------------------------------------------
;Compare the string written to and read back from the Slave
string_compare
	movai read_string
	movra ptr1
	movai write_string
	movra ptr2

loop
	movar ptr1
	movra FSR
	movar INDF
	movra temp_hold
	movar ptr2
	movra FSR
	movar INDF
	rsubra temp_hold
	jbset STATUS, Z
	goto not_equal
	jbclr STATUS, Z
	goto end_string
	incr ptr1
	incr ptr2
	goto loop

not_equal
	movai 1
	xorra P1
	goto exit

end_string
	movai 0x2
	xorra P1
exit
	bclr sflag_event, rw_done
	return

;----------------------------------------------------------------------
;  *******************  Program Memory Read   *******************
;----------------------------------------------------------------------
;Read the message from location MessageTable
CopyRom2Ram
	movai write_string
	movra FSR
	return
;----------------------------------------------------------------------
;----------------------------------------------------------------------

	end

