#include "system.h"
#include "myTimers.h"
#include "cnModule.h"
#include "adcModule.h"
#include "buttonInput.h"
#include "irRemoteControl.h"
#include "lightSensor.h"
#include "microphoneSensor.h"

#include "ioPorts.h"    // For Testing

void highPriority_isr(void);
void lowPriority_isr(void);

// config settings
#pragma config FOSC = INTOSCIO     	// Internal oscillator
#pragma config nPWRTEN = OFF		// Power up timer disabled 
  
#pragma config BOREN = OFF 			// BOR disabled in hardware (SBOREN is ignored)  

#pragma config WDTEN = OFF			// WDT disabled in hardware (SWDTEN ignored)  

#pragma config PBADEN = OFF			// PORTB<5:0> pins are configured as digital I/O on Reset    
									// at reset set PORTB(0-4) to digi IO to enable INT0

#pragma config LVP = OFF			// Single-Supply ICSP disabled for debugging

// interrupt handlers registered here
#pragma code high_vector_section=0x08
void 
high_vector (void)
{
	_asm GOTO highPriority_isr _endasm
}

#pragma code low_vector_section=0x18
void 
low_vector (void)
{
  _asm GOTO lowPriority_isr _endasm
}
#pragma code

#pragma interrupt highPriority_isr 
void 
highPriority_isr(void)
{
	if ( INTCONbits.INT0IF == 1)
	{
		button_isr();
		/* clear the interrupt flag */
		INTCONbits.INT0IF = 0;
	}
    else if ( INTCONbits.IOCIF == 1)
	{
		changeNotice_isr();
		/* clear the interrupt flag */
		INTCONbits.IOCIF = 0;
	}
}
 
#pragma interruptlow lowPriority_isr 
void 
lowPriority_isr(void)
{
	if ( PIR1bits.TMR2IF == 1 )
	{
		/* clear the interrupt flag */
		timer2_isr();
	    // clear the interrupt flag
		PIR1bits.TMR2IF = 0;
	}
    else if ( PIR1bits.ADIF == 1 ) 
	{
		ADConversionCompleted_isr();
		/* clear the interrupt flag */
		PIR1bits.ADIF = 0;
	}
}

void	SysInit( void )
{

/*
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //STEP 1. Configure cache, wait states and peripheral bus clock
	  // Configure the device for maximum performance but do not change the PBDIV
    // Given the options, this function will change the flash wait states, RAM
    // wait state and enable prefetch cache but will not change the PBDIV.
    // The PBDIV value is already set via the pragma FPBDIV option above..
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
*/

	// set system frequency to 16MHz
	OSCCONbits.IRCF0 = 1;
	OSCCONbits.IRCF1 = 1;
	OSCCONbits.IRCF2 = 1;

	// wait for the sensor stablized -- 3s for now
	Delay10KTCYx(10);  // wait 100,000T CY 

    // init for all module here: config ports, interrupts, timer....
//    eventInit();
    timersInit();
    ioPorts_init();
	adcModulePinInit();		// Including lightSensorInit() and microphoneSensorInit()
	buttonInputPinInit();
buttonInputExtInit();
	cnModulePinInit();			 // Including motionSensorInit
cnModuleCNInit();
adcModuleADCInit();

	/* Enable interrupt priority */
	RCONbits.IPEN = 1;
	
	/* Clear the peripheral interrupt flags */
	PIR1 = 0;
	
	/* Enable interrupts */
	INTCONbits.GIEH = 1;
	INTCONbits.GIEL = 1;

	// start checking sensor input timers
	startAdcSamplingTimer();
	startAdcDataProcessingTimer();
turnOnLED2();
}

