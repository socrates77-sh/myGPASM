#include "mc33p116.inc"

        org 0x00
        goto    start 
start:  
        movai   0xFF ;as output 
        movra   OEP1
        movra   OEP2
        COMR	IOP1
  	LCALL	loop
        
       	org 0x1f00
loop:
        COMR   	IOP2
        LGOTO	start
        
        END
        
        

        
        