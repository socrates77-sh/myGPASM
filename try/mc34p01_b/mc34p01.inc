		LIST
;mc34p01.inc    Standard Header File, Version 1.00 by Sinomcu
		NOLIST

;==========================================================================
;
;	Verify Processor
;
;==========================================================================

	IFNDEF __34P01
		MESSG "Processor-header file mismatch.  Verify selected processor."
	ENDIF

;==========================================================================
;
;	Register Definitions
;
;==========================================================================

;----- Register Files ---------------------------------------------------

INDF      		EQU		H'0000'
T0CNT     		EQU		H'0001'
PCL       		EQU		H'0002'
STATUS    		EQU		H'0003'
FSR       		EQU		H'0004'
P0        		EQU		H'0005'
P1        		EQU		H'0006'
MCR       		EQU		H'0008'
KBIM      		EQU		H'0009'
PCLATH    		EQU		H'000A'
PDCON     		EQU		H'000B'
ODCON     		EQU		H'000C'
PUCON     		EQU		H'000D'
INTECON   		EQU		H'000E'
INTFLAG   		EQU		H'000F'
T0CR      		EQU		H'0041'
DDR0      		EQU		H'0045'
DDR1      		EQU		H'0046'
TMCR      		EQU		H'004B'
T1CR      		EQU		H'004C'
T1CNT     		EQU		H'004D'
T1LOAD    		EQU		H'004E'
T1DATA    		EQU		H'004F'

;----- STATUS Bits ---------------------------------------------------

C         		EQU		H'0000'
DC        		EQU		H'0001'
Z         		EQU		H'0002'
PD        		EQU		H'0003'
TO        		EQU		H'0004'
RST       		EQU		H'0007'

;----- P0 Bits ---------------------------------------------------

P00       		EQU		H'0000'
P01       		EQU		H'0001'
P02       		EQU		H'0002'
P03       		EQU		H'0003'

;----- P1 Bits ---------------------------------------------------

P10       		EQU		H'0000'
P11       		EQU		H'0001'
P12       		EQU		H'0002'
P13       		EQU		H'0003'
P14       		EQU		H'0004'
P15       		EQU		H'0005'
P16       		EQU		H'0006'
P17       		EQU		H'0007'

;----- MCR Bits ---------------------------------------------------

EIS       		EQU		H'0006'
WDTEN     		EQU		H'0007'

;----- KBIM Bits ---------------------------------------------------

KBIM0     		EQU		H'0000'
KBIM1     		EQU		H'0001'
KBIM2     		EQU		H'0002'
KBIM3     		EQU		H'0003'
KBIM4     		EQU		H'0004'
KBIM5     		EQU		H'0005'
KBIM6     		EQU		H'0006'
KBIM7     		EQU		H'0007'

;----- PDCON Bits ---------------------------------------------------

P00PD     		EQU		H'0000'
P01PD     		EQU		H'0001'
P02PD     		EQU		H'0002'
P03PD     		EQU		H'0003'
P10PD     		EQU		H'0004'
P11PD     		EQU		H'0005'
P12PD     		EQU		H'0006'

;----- ODCON Bits ---------------------------------------------------

P10OD     		EQU		H'0000'
P11OD     		EQU		H'0001'
P12OD     		EQU		H'0002'
P14OD     		EQU		H'0004'
P15OD     		EQU		H'0005'
P16OD     		EQU		H'0006'
P17OD     		EQU		H'0007'

;----- PUCON Bits ---------------------------------------------------

P10PU     		EQU		H'0000'
P11PU     		EQU		H'0001'
P12PU     		EQU		H'0002'
P14PU     		EQU		H'0004'
P15PU     		EQU		H'0005'
P16PU     		EQU		H'0006'
P17PU     		EQU		H'0007'

;----- INTECON Bits ---------------------------------------------------

T0IE      		EQU		H'0000'
KBIE      		EQU		H'0001'
INT0IE    		EQU		H'0002'
GIE       		EQU		H'0007'

;----- INTFLAG Bits ---------------------------------------------------

T0IF      		EQU		H'0000'
KBIF      		EQU		H'0001'
INT0IF    		EQU		H'0002'

;----- T0CR Bits ---------------------------------------------------

T0PTA     		EQU		H'0003'
T0SE      		EQU		H'0004'
T0PTS     		EQU		H'0005'
INT0M     		EQU		H'0006'

;----- DDR0 Bits ---------------------------------------------------

DDR00     		EQU		H'0000'
DDR01     		EQU		H'0001'
DDR02     		EQU		H'0002'
DDR03     		EQU		H'0003'

;----- DDR1 Bits ---------------------------------------------------

DDR10     		EQU		H'0000'
DDR11     		EQU		H'0001'
DDR12     		EQU		H'0002'
DDR14     		EQU		H'0004'
DDR15     		EQU		H'0005'
DDR16     		EQU		H'0006'
DDR17     		EQU		H'0007'

;----- TMCR Bits ---------------------------------------------------

T1IF      		EQU		H'0000'
T1IE      		EQU		H'0001'
TBS       		EQU		H'0007'

;----- T1CR Bits ---------------------------------------------------

BUZOUT    		EQU		H'0005'
PWMOUT    		EQU		H'0006'
TMR1EN    		EQU		H'0007'

;==========================================================================
;
;	RAM Definition
;
;==========================================================================

	__MAXRAM H'3F'
	;__BADRAM H'0'-H'6', H'8'-H'F'

;==========================================================================
;
;	Configuration Bits
;
;==========================================================================

_CONFIG1  		EQU		H'2001'
_CONFIG0  		EQU		H'2000'

;OPBIT1
_CP_ALL         	EQU		H'1FFF'
_OSC_IRC_RTC    	EQU		H'3FFF'
_OSC_IRC        	EQU		H'37FF'
_OSC_HXT        	EQU		H'2FFF'
_OSC_LXT        	EQU		H'27FF'
_SMT_OFF        	EQU		H'3FFF'
_SMT_ON         	EQU		H'3DFF'
_RD_FROM_PIN    	EQU		H'3FFF'
_RD_FROM_REG    	EQU		H'3EFF'

;OPBIT0
_WDT_ON         	EQU		H'3FFF'
_WDT_OFF        	EQU		H'37FF'
_PWRT_ON        	EQU		H'3FFF'
_PWRT_OFF       	EQU		H'3BFF'
_TWDT_18MS      	EQU		H'3FFF'
_TWDT_4P5MS     	EQU		H'3EFF'
_TWDT_288MS     	EQU		H'3DFF'
_TWDT_72MS      	EQU		H'3CFF'
_FCPU_4T        	EQU		H'3FFF'
_FCPU_2T        	EQU		H'3F7F'
_IRC_8M         	EQU		H'3FFF'
_IRC_4M         	EQU		H'3FEF'
_IRC_2M         	EQU		H'3FDF'
_IRC_1M         	EQU		H'3FCF'
_IRC_444K       	EQU		H'3FAF'
_MCLR_ON        	EQU		H'3FFF'
_MCLR_OFF       	EQU		H'3FF7'
_VBOR_36        	EQU		H'3FFF'
_VBOR_30        	EQU		H'3FFE'
_VBOR_26        	EQU		H'3FFD'
_VBOR_24        	EQU		H'3FFC'
_VBOR_22        	EQU		H'3FFB'
_VBOR_20        	EQU		H'3FFA'
_VBOR_18        	EQU		H'3FF9'
_VBOR_15        	EQU		H'3FF8'

		LIST
