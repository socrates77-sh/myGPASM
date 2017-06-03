

;mc33p116 header file 


;define must write at first row
       
INDF0   EQU     0X01B0
INDF1   EQU     0X01B1
INDF2   EQU     0X01B2
HIBYTE  EQU     0X01B3
FSR0    EQU     0X01B4
FSR1    EQU     0X1B5
PCL     EQU     0X1B6
PFLAG   EQU     0X1B7
MCR     EQU     0X1B8
INDF3   EQU     0X1B9   
INTE    EQU     0X1BA
INTFLAG EQU     0X1BB
OSCM    EQU     0X1BC
LVDCR   EQU     0X1BD  ;
LXTCR   EQU     0X1BE  ;
LXTCP   EQU     0X1BF  ;
IOP0    EQU     0X1C0
OEP0    EQU     0X1C1
PUP0    EQU     0X1C2
DKWP0   EQU     0X1C3  ;
IOP1    EQU     0X1C4
OEP1    EQU     0X1C5
PUP1    EQU     0X1C6
LCDIOSP1 EQU    0X1C7 ;
IOP2    EQU     0X1C8
OEP2    EQU     0X1C9
PUP2    EQU     0X1CA
LCDIOSP2 EQU    0X1CB ;
IOP3    EQU     0X1CC
OEP3    EQU     0X1CD
PUP3    EQU     0X1CE
LCDIOSP3 EQU    0X1CF ;
IOP4    EQU     0X1D0	 ;
OEP4    EQU     0X1D1  ;
PUP4    EQU     0X1D2  ;
LCDIOSP4 EQU    0X1D3 ;
IOP5    EQU     0X1D4  ;
OEP5    EQU     0X1D5  ;
PUP5    EQU     0X1D6  ;
LCDIOSP5 EQU    0X1D7 ;


T0CR0    EQU     0X1D8  ;
T0CR1    EQU     0X1D9  ;
T0LOADH  EQU     0X1DC  ;
T0LOADL  EQU     0X1DD  ;
T0DATAH  EQU     0X1DE  ;
T0DATAL  EQU     0X1DF  ;
T0LATRH  EQU     0X1E0  ;
T0LATRL  EQU     0X1E1  ;
T0LATFH  EQU     0X1E2  ;
T0LATFL  EQU     0X1E3  ;

T1CR     EQU     0X1E4  ;
T1DATA   EQU     0X1E5  ;
T1LOAD   EQU     0X1E6  ;

ADCR0   EQU     0X1E8  ;
ADCR1   EQU     0X1E9  ;
ADCR2   EQU     0X1EA  ;
ADDRH   EQU     0X1EC  ;
ADDRL   EQU     0X1ED  ;

OPCR0   EQU     0X1F0  ;
OPCR1   EQU     0X1F1  ;
LCDCR0  EQU     0X1F2  ;
LCDCR1  EQU     0X1F3  ;
DKW     EQU     0X1F4  ;
KBCR    EQU     0X1F5  ;
LCDDRV  EQU     0X1F6  ;

;PFLAG
#define         Z			PFLAG,2
#define         DC		PFLAG,1
#define         C			PFLAG,0

;int 
#define         GIE       MCR,7
#define         TO        MCR,5
#define         PD        MCR,4
#define         MINT11		MCR,3
#define         MINT10		MCR,2
#define         MINT01		MCR,1
#define         MINT00		MCR,0
  

;INTE
; 1=enable, 0=disable
#define         ADIE    INTE,7
#define         LVDIE   INTE,5
#define         KBIE    INTE,4
#define         INT1IE  INTE,3
#define         INT0IE  INTE,2
#define         T1IE    INTE,1
#define         T0IE    INTE,0

;INTFLAG
;1=active ,
#define         ADIF    INTFLAG,7 
#define         T0RF    INTFLAG,6
#define         LVDIF   INTFLAG,5
#define         KBIF    INTFLAG,4
#define         INT1IF  INTFLAG,3
#define         INT0IF  INTFLAG,2
#define         T1IF    INTFLAG,1
#define         T0IF    INTFLAG,0


;-------------------------------------------------------
;timer0
;----------------------------
;T0CR0
#define			TC0EN				T0CR0,7
#define 		T0MOD1			T0CR0,6
#define			T0MOD0			T0CR0,5
#define 		T0PTS1			T0CR0,4
#define			T0PTS0			T0CR0,3
#define 		T0PR2 			T0CR0,2
#define			T0PR1				T0CR0,1
#define 		T0PR0 			T0CR0,0
;T0CR1
#define			FPWMEN 			T0CR1,7
#define 		PWMEN  			T0CR1,6
#define			PWMOS 			T0CR1,5
#define 		FPWMS 			T0CR1,4
#define			FPWMA1			T0CR1,3
#define 		FPWMA0 			T0CR1,2
#define			FPWMB1			T0CR1,1
#define 		FPWMB0 			T0CR1,0

;-------------------------------------------------------
;timer1
;----------------------------
;T1CR
#define			TC1EN 			T1CR,7
#define 		T1MOD1 			T1CR,6
#define			T1MOD0 			T1CR,5
#define 		T1PTS1 			T1CR,4
#define			T1PTS0			T1CR,3
#define 		T1PR2 			T1CR,2
#define			T1PR1 			T1CR,1
#define 		T1PR0 			T1CR,0


;DKW
#define					DKWE		DKW,7
#define					IROS  	DKW,6
#define					IROT  	DKW,5
#define					WSEL1		DKW,4
#define					WSEL0 	DKW,3
#define					RSEL  	DKW,2
#define					DRVS  	DKW,1
#define					DSELEN	DKW,0

;LCDCR0
#define					LCDEN		LCDCR0,7
#define					LCDSP1	LCDCR0,6
#define					LCDSP0	LCDCR0,5
#define					LCDDUTY	LCDCR0,4
#define					VLCDS		LCDCR0,3
#define					LCDSPEED		LCDCR0,2
#define					LCDRS1	LCDCR0,1
#define					LCDRS0		LCDCR0,0

;ADCR0
#define					ADCHS3		ADCR0,7
#define					ADCHS2  	ADCR0,6
#define					ADCHS1  	ADCR0,5
#define					ADCHS0		ADCR0,4
#define					ADCKS1 		ADCR0,3
#define					ADCKS0  	ADCR0,2
#define					ADEOC  		ADCR0,1
#define					ADON			ADCR0,0

;ADCR1
#define					ADIOS7		ADCR1,7
#define					ADIOS6  	ADCR1,6
#define					ADIOS5  	ADCR1,5
#define					ADIOS4		ADCR1,4
#define					ADIOS3 		ADCR1,3
#define					ADIOS2  	ADCR1,2
#define					ADIOS1 		ADCR1,1
#define					ADIOS0		ADCR1,0

;ADCR2
#define					VREF  		ADCR2,6
#define					VLDOSL1  	ADCR2,5
#define					VLDOSL0		ADCR2,4
#define					TPS1			ADCR2,3
#define					TPS0		  ADCR2,2
#define					VREFRES 	ADCR2,1
#define					VREXT			ADCR2,0

;IOP0
#define					IOP07		IOP0,7
#define					IOP06  	IOP0,6
#define					IOP05  	IOP0,5
#define					IOP04		IOP0,4
#define					IOP03 	IOP0,3
#define					IOP02  	IOP0,2
#define					IOP01 	IOP0,1
#define					IOP00		IOP0,0

;IOP1
#define					IOP17		IOP1,7
#define					IOP16  	IOP1,6
#define					IOP15  	IOP1,5
#define					IOP14		IOP1,4
#define					IOP13 	IOP1,3
#define					IOP12  	IOP1,2
#define					IOP11 	IOP1,1
#define					IOP10		IOP1,0

;IOP2
#define					IOP27		IOP2,7
#define					IOP26  	IOP2,6
#define					IOP25  	IOP2,5
#define					IOP24		IOP2,4
#define					IOP23 	IOP2,3
#define					IOP22  	IOP2,2
#define					IOP21 	IOP2,1
#define					IOP20		IOP2,0

;IOP3
#define					IOP37		IOP3,7
#define					IOP36  	IOP3,6
#define					IOP35  	IOP3,5
#define					IOP34		IOP3,4
#define					IOP33 	IOP3,3
#define					IOP32  	IOP3,2
#define					IOP31 	IOP3,1
#define					IOP30		IOP3,0
             
;IOP4
#define					IOP47		IOP4,7
#define					IOP46  	IOP4,6
#define					IOP45  	IOP4,5
#define					IOP44		IOP4,4
#define					IOP43 	IOP4,3
#define					IOP42  	IOP4,2
#define					IOP41 	IOP4,1
#define					IOP40		IOP4,0

;IOP5
#define					IOP55  	IOP5,5
#define					IOP54		IOP5,4
#define					IOP53 	IOP5,3
#define					IOP52  	IOP5,2
#define					IOP51 	IOP5,1
#define					IOP50		IOP5,0

;OSCM
#define					STBL  	OSCM,5
#define					STBH		OSCM,4
#define					CLKS  	OSCM,2
#define					LFEN 		OSCM,1
#define					HFEN		OSCM,0