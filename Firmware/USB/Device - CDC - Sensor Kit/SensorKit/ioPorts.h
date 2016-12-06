#ifndef __IO_PORTS_H__
#define __IO_PORTS_H__

#include "system.h"

/** LED ************************************************************/
#define mLED_S         		LATBbits.LATB5

// for motion should be Blue and mic should be Green
// for new board LED2(RA1) --Green, LED1(RA0)--Blue
#define mLED_1              LATAbits.LATA0     
#define mLED_2              LATAbits.LATA1	   

// for old board LED1(RA0)--Green, LED2(RA1)--Blue
//#define mLED_1              LATAbits.LATA1
//#define mLED_2              LATAbits.LATA0

#define mGetLED_1()         mLED_1
#define mGetLED_2()         mLED_2
#define mGetLED_S()         mLED_S

#define mLED_1_On()         mLED_1 = 1;
#define mLED_2_On()         mLED_2 = 1;
#define mLED_S_On()         mLED_S = 1;

#define mLED_1_Off()        mLED_1 = 0;
#define mLED_2_Off()        mLED_2 = 0;
#define mLED_S_Off()        mLED_S = 0;

#define mLED_1_Toggle()     mLED_1 = !mLED_1;
#define mLED_2_Toggle()     mLED_2 = !mLED_2;
#define mLED_S_Toggle()     mLED_S = !mLED_S;


//=====================================================================
// PUBLIC FUNCTION DELCLARATIONS
//=====================================================================
void ioPorts_init ( void );


#endif // __IO_PORTS_H__