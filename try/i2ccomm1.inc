		#include  "flags.inc"            ; required include file

		GLOBAL		write_string         ; make variable viewable for other modules
		GLOBAL 		read_string          ; make variable viewable for other modules

		EXTERN		sflag_event          ; reference linkage for variable  
		EXTERN		eflag_event          ; reference linkage for variable
		EXTERN		i2cState             ; reference linkage for variable
		EXTERN		read_count           ; reference linkage for variable
		EXTERN		write_count          ; reference linkage for variable
		EXTERN		write_ptr            ; reference linkage for variable
		EXTERN		read_ptr             ; reference linkage for variable
		EXTERN		temp_address         ; reference linkage for variable

		EXTERN		init_i2c             ; reference linkage for function
		EXTERN		service_i2c          ; reference linkage for function
