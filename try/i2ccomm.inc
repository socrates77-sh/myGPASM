#include  "flags.inc"            ; required include file

i2cSizeMask		EQU		0x0F


	GLOBAL		sflag_event          ; make variable viewable for other modules 
	GLOBAL		eflag_event          ; make variable viewable for other modules
	GLOBAL		i2cState             ; make variable viewable for other modules
	GLOBAL		read_count           ; make variable viewable for other modules
	GLOBAL		write_count          ; make variable viewable for other modules
	GLOBAL		write_ptr            ; make variable viewable for other modules
	GLOBAL		read_ptr             ; make variable viewable for other modules
	GLOBAL		temp_address         ; make variable viewable for other modules

	GLOBAL		init_i2c             ; make function viewable for other modules
	GLOBAL		service_i2c          ; make function viewable for other modules



;*******    GENERAL PURPOSE VARIABLES
GPR_DATA		UDATA
sflag_event      RES     1               ; variable for i2c general status flags
eflag_event      RES     1               ; variable for i2c error status flags
i2cState         RES     1               ; I2C state machine variable
read_count       RES     1               ; variable used for slave read byte count
write_count      RES     1               ; variable used for slave write byte count
write_ptr        RES     1               ; variable used for pointer (writes to)
read_ptr         RES     1               ; variable used for pointer (reads from)
temp_address     RES     1               ; variable used for passing address to functions
		

