/*********************************************************************
 * FileName:        buttonInput.c
 * Description:     The module is used to config button input
 *                  pin and read the button input including debounce
 *                  processing.
 *
 ********************************************************************/

#include "system.h"
#include "myTimers.h"
#include "buttonInput.h"

#include "ioPorts.h"    // For Testing


/*----------------------------------------------------------------------------
* public Functions
*----------------------------------------------------------------------------*/
void buttonInputPinInit( void )
{
	/* PORTB RB1 input for the button */
	TRISBbits.TRISB1 = 1;
	ANSELBbits.ANSB1 = 0;
}

void buttonInputExtInit( void )
{
	//Enable INT1 with raising edge. It is always high priority
	INTCON2bits.INTEDG1 = 1; // raising edge
	INTCON3bits.INT1IE = 1;
}

void button_isr (void)
{
	startButtonInputDebounceTimer();
}

BOOL buttonInput_isButtonPressed (void)
{
	if ( PORTBbits.RB1 == BUTTON_INPUT_PIN_NORMAL )
	{
		// button is not being pressed
		return FALSE;
	}
	else
	{
		// button is being pressed
		return TRUE;
	}
}
