
#ifndef __BUTTON_INPUT_H__
#define __BUTTON_INPUT_H__

#define BUTTON_INPUT_PIN_NORMAL    		0    // button input pin is normal Lo

//=====================================================================
// PUBLIC FUNCTION DELCLARATIONS
//=====================================================================
void button_isr (void);

void buttonInputPinInit( void ); 
void buttonInputExtInit( void ); 
BOOL buttonInput_isButtonPressed (void);

#endif // __BUTTON_INPUT_H__