/*********************************************************************
 * FileName:        myTimers.c
 * Dependencies:	plib.h
 *
 * Processor:       PIC18
 *
 * Complier:        MPLAB C18
 *                  MPLAB IDE
 *********************************************************************
 * Description: The module provides functions for user to create their own 
 * timers. It uses of peripheral library macros and functions 
 * supporting the PIC18 General Purpose 8-bit Timer0. Internal Clock source 
 * is used and enabled T0 overflow interrupt. Prescale 1:16.
 *
 * Note: Internal clock frequency = fosc / 4;
 *       for fosc = 16MHz, Internal Clock Frequency = 4MHz/16 = 0.25MHz
 *       one Timer0 reading will be 4us and overflow period = 256 * 4us = 1024us about 1ms
 ********************************************************************/
#include "myTimers.h"

#include "adcModule.h"
#include "buttonInput.h"
#include "irRemoteControl.h"
#include "lightSensor.h"
#include "microphoneSensor.h"

#include "ioPorts.h"   


//###########################################
//###########################################

static long mSecTicks;

int myCnt;

/* adcSamplingTimer */
static BOOL adcSamplingTimerEnableFlag;
static int adcSamplingTimerCount;
#define ADC_SAMPLING_TIMEOUT  			50  //(50*3)  						// adc sampling timeout every 50msec

/* adcDataProcessingTimer */
static BOOL adcDataProcessingTimerEnableFlag;
static int adcDataProcessingTimerCount;
#define ADC_DATA_PROCESSING_TIMEOUT  				(500*3)  						// adc data processing timeout every 500msec

/* buttonInputDebounceTimer */
static BOOL buttonInputDebounceTimerEnableFlag;
static int buttonInputDebounceTimerCount;
#define BUTTON_INPUT_DEBOUNCE_TIMEOUT  				(100*3) 						//  button input debounce timeout for 100msec

/* irDataPulseTimeoutTimer - one time timer */
static BOOL irDataPulseTimeoutTimerEnableFlag;      
static int irDataPulseTimeoutTimerCount;
#define IR_DATA_PULSE_TIMEOUT  						(2*3)						//  2ms: wait data pulse end edge for 1ms after rxed pulse start edge 

/* irDataSpaceTimeoutTimer - one time timer */
static BOOL irDataSpaceTimeoutTimerEnableFlag;      
static int irDataSpaceTimeoutTimerCount;
#define IR_DATA_SPACE_TIMEOUT  					50	//(50*3)						//  1s: wait data space end edge for 2s after rxed space start edge


/* generalPurpTimer */
static BOOL generalPurpTimerEnableFlag;      
static int generalPurpTimerCount;
static unsigned int GENERAL_PURP_TIMER_TICK;  								// user define the timeout time ( number of timer tick) for the general purpose timer

/*********************************************************************
* Function Name       : timersInit
* Object              : Init timer1 counter for 1msec interrupt
* Input Parameters    : none
* Output Parameters   : none
 ********************************************************************/
void timersInit ( void )
{
    /* init the timer1 mSecTicks counter */
    mSecTicks=0;

    /* adcSamplingTimer Init*/
    adcSamplingTimerEnableFlag = FALSE;
    adcSamplingTimerCount = 0;

    /* adcDataProcessingTimer Init*/
    adcDataProcessingTimerEnableFlag = FALSE;
    adcDataProcessingTimerCount = 0;

    /* buttonInputDebounceTimer Init*/
    buttonInputDebounceTimerEnableFlag = FALSE;
    buttonInputDebounceTimerCount = 0;

    /* irDataPulseTimeoutTimer Init*/
    irDataPulseTimeoutTimerEnableFlag = FALSE;
    irDataPulseTimeoutTimerCount = 0;

    /* irDataSpaceTimeoutTimer Init*/
    irDataSpaceTimeoutTimerEnableFlag = FALSE;
    irDataSpaceTimeoutTimerCount = 0;

	/* generalPurpTimer */
	generalPurpTimerEnableFlag = FALSE;      
	generalPurpTimerCount = 0;
	GENERAL_PURP_TIMER_TICK = 0;


	/* Configure Timer2
  	/* Enable the timer interrupt */
//    PR2 = 250;   				// 4us x250=1ms
    PR2 = 0xFF;   				// 4us x250=1ms
    T2CONbits.T2CKPS1 = 1; 		// prescaler 1:16

    PIE1bits.TMR2IE = 1;
    IPR1bits.TMR2IP = 0;		// low priority

	/* Enable the timer */
	T2CONbits.TMR2ON = 1;

	/* Configure Timer1 for IR remote control signal pulse width time tick = 250ns counter*/
	T1CONbits.TMR1CS = 0b00;    // system clock (Fosc/4)
	T1CONbits.T1CKPS = 0b00;    // prescale 1:1
	T1CONbits.RD16 = 1;    		// 16-bit operation
	T1CONbits.TMR1ON = 1;    	// turn on Timer1/3
}//* End


/*----------------------------------------------------------------------------
* Function Name       : timer2_isr
* Object              : C handler interrupt function called every 1msec
* Output Parameters   : increment counts
*----------------------------------------------------------------------------*/
void timer2_isr (void)
{
//    EventType evt;

	if ( PIR1bits.TMR2IF == 0 ) return;

    mSecTicks++;

	/* adcSamplingTimer - repeat timer */
    if (adcSamplingTimerEnableFlag == TRUE)
    {
        adcSamplingTimerCount++;
        if( adcSamplingTimerCount == ADC_SAMPLING_TIMEOUT )
        {
            adcSamplingTimerCount = 0;			// reset the timer counter
			
			ADCStartSampling();

            // send a adcDataProcessingTimer timeout event to the event queue.
  //          evt.eType = adcSamplingTimerTimeoutEvent;
  //          putEventIntoEventQueue(&evt);
        }
    }

	/* adcDataProcessingTimer - repeat timer */
    if (adcDataProcessingTimerEnableFlag == TRUE)
    {
        adcDataProcessingTimerCount++;
        if( adcDataProcessingTimerCount == ADC_DATA_PROCESSING_TIMEOUT )
        {
            adcDataProcessingTimerCount = 0;			// reset the timer counter
 
			lightSensorDataProcessing();
			microphoneSensorDataProcessing();

            // send a adcDataProcessingTimer timeout event to the event queue.
//            evt.eType = adcDataProcessingTimerTimeoutEvent;
//            putEventIntoEventQueue(&evt);
          
        }
    }

    /* buttonInputDebounce - one time timer */
    if (buttonInputDebounceTimerEnableFlag == TRUE)
    {
        buttonInputDebounceTimerCount++;
        if( buttonInputDebounceTimerCount == BUTTON_INPUT_DEBOUNCE_TIMEOUT )
        {
			if ( buttonInput_isButtonPressed() == TRUE )
			{
			    // send USB buttonInputDetectedMsg Here
//mLED_2_Toggle();
sendUSBMsg('B');

//			    evt.eType = buttonInputDetectedEvent;
//			    putEventIntoEventQueue(&evt);
			}

            // stop this timer
            buttonInputDebounceTimerEnableFlag = FALSE;
            buttonInputDebounceTimerCount = 0;
        }
    }
 

    /* irDataPulseTimeoutTimer - one time timer */
    if ( irDataPulseTimeoutTimerEnableFlag == TRUE )
    {
        irDataPulseTimeoutTimerCount++;
        if( irDataPulseTimeoutTimerCount == IR_DATA_PULSE_TIMEOUT )
        {
			irDataPulseTimeoutProcessing();

            // send a irDataPulseTimeoutTimer timeout event to the event queue.
  //          evt.eType = irDataPulseTimeoutTimerTimeoutEvent;
  //          putEventIntoEventQueue(&evt);

            // stop this timer
            irDataPulseTimeoutTimerEnableFlag = FALSE;
            irDataPulseTimeoutTimerCount = 0;
        }
    }

    /* irDataSpaceTimeoutTimer - one time timer */
    if ( irDataSpaceTimeoutTimerEnableFlag == TRUE )
    {
        irDataSpaceTimeoutTimerCount++;
        if( irDataSpaceTimeoutTimerCount == IR_DATA_SPACE_TIMEOUT )
        {
			 irDataSpaceTimeoutProcessing();

            // send a irDataSpaceTimeoutTimer timeout event to the event queue.
//            evt.eType = irDataSpaceTimeoutTimerTimeoutEvent;
//            putEventIntoEventQueue(&evt);

            // stop this timer
            irDataSpaceTimeoutTimerEnableFlag = FALSE;
            irDataSpaceTimeoutTimerCount = 0;
        }
    }

    /* generalPurpTimer - one time timer */
    if ( generalPurpTimerEnableFlag == TRUE )
    {
        generalPurpTimerCount++;
        if( generalPurpTimerCount == GENERAL_PURP_TIMER_TICK )
        {
			// timer out handler
			mLED_S_Toggle();

            // stop this timer
            generalPurpTimerEnableFlag = FALSE;
            generalPurpTimerCount = 0;
			GENERAL_PURP_TIMER_TICK = 0;
        }
    }
}


//-----------------------------------------------------------------------------
// return the number of ticks since startup.
//-----------------------------------------------------------------------------
long	TimeGetTicks()
{
  return (mSecTicks);
}


//-----------------------------------------------------------------------------
// delay for the specified time in milliseconds
//-----------------------------------------------------------------------------
void mSecDelay(long delay)
{
  long startTicks;

  startTicks = TimeGetTicks();

  while( TimeGetTicks() < (startTicks + delay) );
}

//-----------------------------------------------------------------------------
// adcSamplingTimer
//-----------------------------------------------------------------------------
void startAdcSamplingTimer(void)
{
    adcSamplingTimerCount = 0;
    adcSamplingTimerEnableFlag = TRUE;
}

void stopAdcSamplingTimer(void)
{
    adcSamplingTimerCount = 0;
    adcSamplingTimerEnableFlag = FALSE;
}

BOOL getAdcSamplingTimerFlag (void)
{
    return adcSamplingTimerEnableFlag;
}

//-----------------------------------------------------------------------------
// adcDataProcessingTimer
//-----------------------------------------------------------------------------
void startAdcDataProcessingTimer(void)
{
    adcDataProcessingTimerCount = 0;
    adcDataProcessingTimerEnableFlag = TRUE;
}

void stopAdcDataProcessingTimer(void)
{
    adcDataProcessingTimerCount = 0;
    adcDataProcessingTimerEnableFlag = FALSE;
}

BOOL getAdcDataProcessingTimerFlag (void)
{
    return adcDataProcessingTimerEnableFlag;
}


//-----------------------------------------------------------------------------
// buttonInputDebounceTimer
//-----------------------------------------------------------------------------
void startButtonInputDebounceTimer(void)
{
    buttonInputDebounceTimerCount = 0;
    buttonInputDebounceTimerEnableFlag = TRUE;
}

void stopButtonInputDebounceTimer(void)
{
    buttonInputDebounceTimerCount = 0;
    buttonInputDebounceTimerEnableFlag = FALSE;
}

BOOL getButtonInputDebounceTimerFlag (void)
{
    return buttonInputDebounceTimerEnableFlag;
}


//-----------------------------------------------------------------------------
// irDataPulseTimeoutTimer - one time timer
//-----------------------------------------------------------------------------
void startIrDataPulseTimeoutTimer(void)
{
	irDataPulseTimeoutTimerCount = 0;
    irDataPulseTimeoutTimerEnableFlag = TRUE;
}

void stopIrDataPulseTimeoutTimer(void)
{
    irDataPulseTimeoutTimerCount = 0;
    irDataPulseTimeoutTimerEnableFlag = FALSE;
}

BOOL getIrDataPulseTimeoutTimerFlag (void)
{
    return irDataPulseTimeoutTimerEnableFlag;
}


//-----------------------------------------------------------------------------
// irDataSpaceTimeoutTimer - one time timer
//-----------------------------------------------------------------------------
void startIrDataSpaceTimeoutTimer(void)
{
	irDataSpaceTimeoutTimerCount = 0;
    irDataSpaceTimeoutTimerEnableFlag = TRUE;
}

void stopIrDataSpaceTimeoutTimer(void)
{
    irDataSpaceTimeoutTimerCount = 0;
    irDataSpaceTimeoutTimerEnableFlag = FALSE;
}

BOOL getIrDataSpaceTimeoutTimerFlag (void)
{
    return irDataSpaceTimeoutTimerEnableFlag;
}

//-----------------------------------------------------------------------------
// generalPurpTimer - one time timer
//-----------------------------------------------------------------------------
void startGeneralPurpTimer(unsigned int numOfMsec)
{
	GENERAL_PURP_TIMER_TICK = numOfMsec;
	generalPurpTimerCount = 0;
    generalPurpTimerEnableFlag = TRUE;
}

void stopGeneralPurpTimer(void)
{
    generalPurpTimerCount = 0;
    generalPurpTimerEnableFlag = FALSE;
}


BOOL getGeneralPurpTimerFlag (void)
{
    return generalPurpTimerEnableFlag;
}

