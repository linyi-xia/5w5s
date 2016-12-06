#include <p18cxxx.h>
#include "ioPorts.h"    

//=====================================================================
// PUBLIC FUNCTION DELCLARATIONS
//=====================================================================
void ioPorts_init ( void )
{
	// Init LEDs: LED1 - RA0; LED2 - RA1; LEDSwitch - RB5
	#define mInitAllLEDs()      LATA &= 0b11111100; TRISA &= 0b11111100; ANSELA &= 0b11111100;
    TRISAbits.TRISA0 = 0;
	ANSELAbits.ANSA0 = 0;
	LATAbits.LATA0 = 0;	
	
    TRISAbits.TRISA1 = 0;
	ANSELAbits.ANSA1 = 0;
	LATAbits.LATA1 = 0;	
	
    TRISBbits.TRISB5 = 0;
	ANSELBbits.ANSB5 = 0;
	LATBbits.LATB5 = 0;	

    // intitiate unused pins as digital outputs with low
    // configure PORTA unused pins RA2,4,5,6,7as digital output with low
    TRISAbits.TRISA2 = 0;
	ANSELAbits.ANSA2 = 0;

    TRISAbits.TRISA4 = 0;

	TRISAbits.TRISA5 = 0;
	ANSELAbits.ANSA5 = 0;

    TRISAbits.TRISA6 = 0;

	TRISAbits.TRISA7 = 0;

	PORTAbits.RA4 = 0;   
	PORTAbits.RA5 = 0;
	PORTAbits.RA6 = 0;
	PORTAbits.RA7 = 0;

	// configure PORTB unused pins RB0,2,3 as digital output with low
    TRISBbits.TRISB0 = 0;
	ANSELBbits.ANSB0 = 0;

	TRISBbits.TRISB2 = 0;
	ANSELBbits.ANSB2 = 0;

    TRISBbits.TRISB3 = 0;
	ANSELBbits.ANSB3 = 0;

	PORTBbits.RB0 = 0;   
	PORTBbits.RB2 = 0;
	PORTBbits.RB3 = 0;

	// configure PORTC unused pins RC0,1,6 as digital output with low
    TRISCbits.TRISC0 = 0;

	TRISCbits.TRISC1 = 0;

    TRISCbits.TRISC6 = 0;
	ANSELCbits.ANSC6 = 0;

	PORTCbits.RC0 = 0;   
	PORTCbits.RC1 = 0;
	PORTCbits.RC6 = 0;
}

