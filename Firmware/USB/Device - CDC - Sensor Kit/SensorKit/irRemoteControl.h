
#ifndef __IR_REMOTE_CONTROL_H__
#define __IR_REMOTE_CONTROL_H__


// Use General IO pin RC7 for remote IR receive 
#define IR_RX_PIN_NORMAL    1    // remote receiving pin is normal Hi


//=====================================================================
// PUBLIC FUNCTION DELCLARATIONS
//=====================================================================
void irRemoteControlInit( void ); 

void irRemoteControl_irPinChanged( unsigned int changedIrPinState );

void irDataPulseTimeoutProcessing( void );
void irDataSpaceTimeoutProcessing( void );

#endif // __IR_REMOTE_CONTROL_H__