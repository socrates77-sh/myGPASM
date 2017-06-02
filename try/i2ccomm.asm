#include mc30p011.inc 
#include  "i2ccomm.inc"       

#define  FOSC        D'8000000'
#define  I2CClock    D'400000'
#define  ClockValue  (((FOSC/I2CClock)/4) -1) ; 
	

;----------------------------------------------------------------------
;   ***********************  I2C Service  *************************
;----------------------------------------------------------------------
I2C_COMM	CODE	
service_i2c:
	movra DDR1
	movai high I2CJump
	movra PCLATH
	movai i2cSizeMask
	andar i2cState
	addai low (I2CJump + 1)
	jbclr STATUS, C
	incr PCLATH
I2CJump:
	movra PCL

	goto WrtStart
	goto SendWrtAddr
	goto WrtAckTest
	goto WrtStop

	goto ReadStart
	goto SendReadAddr
	goto ReadAckTest
	goto ReadData
	goto ReadStop

I2CJumpEnd		 
	Fill (return),  (I2CJump-I2CJumpEnd) + i2cSizeMask 




;----------------------------------------------------------------------
;   ********************* Write data to Slave   *********************
;----------------------------------------------------------------------
; Generate I2C bus start condition               [ I2C STATE -> 0 ]
WrtStart
	movar write_ptr
	movra FSR
	incr i2cState
	bset P0, 0
	return

; Generate I2C address write (R/W=0)             [ I2C STATE -> 1 ]
SendWrtAddr
	bclr STATUS, C
	rlr temp_address
	incr i2cState
	return

; Test acknowledge after address and data write  [ I2C STATE -> 2 ]
WrtAckTest
	jbset P0, 1
	goto WrtData
	bset eflag_event, ack_error
	clrr i2cState
	return

; Generate I2C write data condition
WrtData
	movar INDF
	jbset P0, 2
	goto send_byte
	incr i2cState
send_byte
	movra P1
	return

; Generate I2C bus stop condition                [ I2C STATE -> 3 ]
WrtStop
	jbset P0, 3
	goto no_error
	bset eflag_event, ack_error
	clrr i2cState
	goto stop1
no_error
	incr i2cState
stop1
	bset P1, 1
	return

;----------------------------------------------------------------------
;   ********************* Read data from Slave   *********************
;----------------------------------------------------------------------
; Generate I2C start condition                   [ I2C STATE -> 4 ]
ReadStart
	movar read_ptr
	movra FSR
	incr i2cState
	bset P1, 0
	return

; Generate I2C address write (R/W=1)             [ I2C STATE -> 5 ]
SendReadAddr 
	bset STATUS, C
	rlr temp_address
	incr i2cState
	return

; Test acknowledge after address write           [ I2C STATE -> 6 ]
ReadAckTest
	jbset P1, 2
	goto StartReadData
	bset eflag_event, ack_error
	clrr i2cState
	return

StartReadData
	incr i2cState
	return

; Read slave I2C                                 [ I2C STATE -> 7 ]
ReadData
	djzr read_count
	goto SendReadAck

; Send Not Acknowledge
SendReadNack
	movra INDF
	incr i2cState
	return

; Send Acknowledge
SendReadAck
	movra INDF
	incr FSR
	goto $-1
	bset P1, 2
	return

; Generate I2C stop condition                    [ I2C STATE -> 8 ]
ReadStop
	bclr P1, 2
	clrr i2cState
	bset sflag_event, rw_done
	return
	
;----------------------------------------------------------------------
;   ******************* Generic bus idle check ***********************
;----------------------------------------------------------------------
; test for i2c bus idle state; not implemented in this code (example only)
i2c_idle   
	movra P1
	andai 0x1f
	jbset STATUS, Z
	goto $-3
	return

;----------------------------------------------------------------------
;  ******************* INITIALIZE MSSP MODULE  *******************
;----------------------------------------------------------------------

init_i2c
	movai ClockValue
	movai b'00011000'
	;iost 5
	return

	end
