
;*******    INTERRUPT CONTEXT SAVE/RESTORE VARIABLES
INT_VAR        UDATA   0x20              ; create uninitialized data "udata" section
w_temp           RES     1               ;
status_temp      RES     1               ;
pclath_temp      RES     1


INT_VAR1       UDATA   0x30              ; reserve location 0xA0
w_temp1          RES     1


;*******    GENERAL PURPOSE VARIABLES
GPR_DATA		UDATA
temp_hold        RES     1               ; temp variable for string compare
ptr1             RES     1               ; used as pointer in string compare 
ptr2             RES     1               ; used as pointer in string compare


STRING_DATA		 UDATA	
write_string     RES      D'3'
read_string      RES      D'3'       



		EXTERN		init_timer1          ; reference linkage for function
		EXTERN		init_ports           ; reference linkage for function

