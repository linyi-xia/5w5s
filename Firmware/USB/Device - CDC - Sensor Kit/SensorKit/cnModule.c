/*********************************************************************
 * FileName:        changeNoticeConfig.c
 * Description:     The module is used to config CN enabled pins.
 *
 *
 ********************************************************************/
#include "system.h"
#include "motionSensor.h"
#include "irRemoteControl.h"
#include "cnModule.h"

unsigned int motionSensorState;
unsigned int irRxPinRead;

void cnModulePinInit( void )
{
	/* PORTB RB4 and RC7 input for the motion sensor and IR sensor */
	TRISBbits.TRISB4 = 1;
	ANSELBbits.ANSB4 = 0;

	TRISCbits.TRISC7  = 1;
	ANSELCbits.ANSC7 = 0;

	irRxPinRead = PORTCbits.RC7;
    motionSensorState = PORTBbits.RB4;

	// Init pins used CN
	motionSensorInit();
	irRemoteControlInit();
}

void cnModuleCNInit( void )
{
    // enable change notice, enable discrete pins and weak pullups
	IOCBbits.IOCB4 = 1;
	IOCCbits.IOCC7 = 1;

	INTCONbits.IOCIE = 1;
	INTCON2bits.IOCIP = 1;   // with high priority
	
    // read port(s) to clear mismatch on change notice pins
	irRxPinRead = PORTCbits.RC7;
    motionSensorState = PORTBbits.RB4;

	if ( motionSensorState != MOTION_SENSOR_PIN_NORMAL )
	{
		motionSensorMotionChanged( motionSensorState );
	}

    // clear change notice interrupt flag
    INTCONbits.IOCIF = 0;
}


void changeNotice_isr(void)
{
	unsigned int temp_motion_state;
	unsigned int temp_irRxPinread;

	if ( INTCONbits.IOCIF == 0 ) return;

    // clear the mismatch condition
    temp_motion_state = PORTBbits.RB4;
	temp_irRxPinread = PORTCbits.RC7;

    // clear the interrupt flag
    INTCONbits.IOCIF = 0;

	if ( temp_motion_state != motionSensorState )
	{
		motionSensorMotionChanged( temp_motion_state );
		motionSensorState = temp_motion_state;
	}

	if ( temp_irRxPinread != irRxPinRead )
	{
		irRemoteControl_irPinChanged( temp_irRxPinread );
		irRxPinRead = temp_irRxPinread;
	}
}

