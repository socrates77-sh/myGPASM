		LIST
;mc31p11.inc   Standard Header File, Version 1.00 by Sinomcu
		NOLIST
;==========================================================================
;
;       Verify Processor
;
;==========================================================================

        IFNDEF __31P11
           MESSG "Processor-header file mismatch.  Verify selected processor."
        ENDIF

;==========================================================================
;
;       Register Definitions
;
;==========================================================================

;----- Register Files------------------------------------------------------

INDF                         EQU     H'0000'
FSR                          EQU     H'0001'
PCL                          EQU     H'0002'
STATUS                       EQU     H'0003'
MCR                          EQU     H'0004'
INTECON                      EQU     H'0005'
INTFLAG                      EQU     H'0006'
IOP0                         EQU     H'0007'
OEP0                         EQU     H'0008'
PUP0                         EQU     H'0009'
DKWP0                        EQU     H'000A'
IOP1                         EQU     H'000B'
OEP1                         EQU     H'000C'
PUP1                         EQU     H'000D'
DKWP1                        EQU     H'000E'
DKW                          EQU     H'000F'

;----- STATUS Bits --------------------------------------------------------

Z                            EQU     H'0002'
DC                           EQU     H'0001'
C                            EQU     H'0000'

;----- MCR Bits --------------------------------------------------------

GIE                          EQU     H'0007'
TO                           EQU     H'0005'
PD                           EQU     H'0004'

;----- INTECON Bits --------------------------------------------------------

INT1IE                       EQU     H'0003'
INT0IE                       EQU     H'0002'

;----- INTFLAG Bits --------------------------------------------------------

INT1IF                       EQU     H'0003'
INT0IF                       EQU     H'0002'

;----- DKW Bits --------------------------------------------------------

DKWE                         EQU     H'0007'
RSEL                         EQU     H'0006'

;==========================================================================
;
;       RAM Definition
;
;==========================================================================

        __MAXRAM H'3F'
        __BADRAM H'00'-H'0F'
;==========================================================================
;
;       Configuration Bits
;
;==========================================================================

_CONFIG0                     EQU     H'8000'
_CONFIG1                     EQU     H'8001'

;OPBIT0
_CP_ALL                      EQU     H'0FFF'
_WDT_ALWAYS_OFF              EQU     H'1FFC'
_WDT_SLEEP_OFF               EQU     H'1FFD'
_WDT_ALWAYS_ON               EQU     H'1FFF'
_FCPU_2X                     EQU     H'1FF3'
_FCPU_4X                     EQU     H'1FF7'
_FCPU_8X                     EQU     H'1FFB'
_FCPU_16X                    EQU     H'1FFF'

;OPBIT1

		LIST
