
#include "system.h"
#include "irRemoteControl.h"
#include "myTimers.h"
#include "ioPorts.h"    // For Testing

// Implementation for Dish Network IR remote control below
#define IR_DATA_BITS_LENGTH			16   // 16 bits data for Dish Network IR remote

// using 16bits timer Timer1 as a timer, as defined in timers.c: Timer1's tick = 800ns
#define START_CONDITION_SPACE_MIN	24000*3	   // 7500			// 	6ms <=> 7500 * 800ns = 6ms; (measured as 6.26ms - 6.45ms ==> 6ms - 7ms)
#define START_CONDITION_SPACE_MAX	28000*3   //	8750			// 	7ms <=> 8750 * 800ns = 7ms; (measured as 6.26ms - 6.45ms ==> 6ms - 7ms)

#define DATA_LOG_0_SPACE_MIN		6000*3  //	1875			// 	1.5ms <=> 1875 * 800ns = 1.51ms; (measured as 1.74ms - 1.96ms ==> 1.5ms - 2.0ms)
#define DATA_LOG_0_SPACE_MAX		8000*3  //	2500			// 	2.0ms <=> 2500 * 800ns = 2.02ms; (measured as 1.74ms - 1.96ms ==> 1.5ms - 2.0ms)

#define DATA_LOG_1_SPACE_MIN		10000*3   //	3125			// 	2.5ms <=> 3125 * 800ns = 2.5ms; (measured as 2.76ms - 3.10ms ==> 2.5ms - 3.5ms)
#define DATA_LOG_1_SPACE_MAX		14000*3   //	4375			// 	3.5ms <=> 4375 * 800ns = 3.5ms; (measured as 2.76ms - 3.10ms ==> 2.5ms - 3.5ms)

#define PULSE_MIN					600*3  //	250				// 	150us <=> 250 * 800ns = 200us; (measured as 200us - 500us)
#define PULSE_MAX					2000*3  //	625				// 	500us <=> 625 * 800ns = 500us; (measured as 200us - 500us)

#define DISH_MODE_SAT 					63				// decimal value of the SAT mode Bit10 - Bit15: 111111 = 63(dec)
#define DISH_POWER						61				// decimal value of the Power Bit0 - Bit5:   111101 = 61(dec) 

typedef enum _irDataStatusEnum
{
    DATA_IDLE = 0,
    DATA_START_CONDITON,
    DATA_RECEIVING,
} irDataStatusEnum;

typedef enum irWaveformStateEnum
{
	WAVEFORM_IDLE = 0,
    PULSE_START,
    SPACE_START,
} irWaveformStateEnum;


irDataStatusEnum irDataStatus;
irWaveformStateEnum irWaveformState;

unsigned int numOfBitsRxed;


unsigned int timerStartTick;
unsigned int timerEndTick;

unsigned int pulseWidth;
unsigned int spaceWidth;

BOOL repeatDataflag;

unsigned int rxDataBits[IR_DATA_BITS_LENGTH];


void initRxData (void)
{
	irDataStatus = DATA_IDLE;
	irWaveformState = WAVEFORM_IDLE;
	numOfBitsRxed = 0;
	timerStartTick = 0;
	timerEndTick = 0;
	pulseWidth = 0;
	spaceWidth = 0;

	memset(rxDataBits,0, sizeof(rxDataBits));
}

void discardRxData (void)
{
	initRxData();
	repeatDataflag = FALSE;
}

void processingRxedIrData(void)
{
//    EventType evt;
	unsigned int cmdData;
	unsigned int addrData;
	unsigned int modeData;

 	if ( repeatDataflag == FALSE )
	{
		cmdData = 32 * rxDataBits[0] + 16 * rxDataBits[1] + 8 * rxDataBits[2] + 4 * rxDataBits[3] + 2 * rxDataBits[4] + rxDataBits[5];
		addrData = 8 * rxDataBits[6] + 4 * rxDataBits[7] + 2 * rxDataBits[8] + rxDataBits[9];
		modeData = 32 * rxDataBits[10] + 16 * rxDataBits[11] + 8 * rxDataBits[12] + 4 * rxDataBits[13] + 2 * rxDataBits[14] + rxDataBits[15];

		// send remote control event to the event queue
		if (( modeData == DISH_MODE_SAT ) && ( cmdData == DISH_POWER ))
		{
			sendUSBMsg('P');
			
			mLED_S_Toggle();
			startGeneralPurpTimer(500*3);  // toggle after 500ms
			
			// send USB remotePowerButtonDetectedMsg Here
			// remote power button is pressed 
 //       	evt.eType = remotePowerButtonDetectedEvent;
		} 
		else
		{
			// send USB remoteOtherButtonDetectedMsg Here

			sendUSBMsg('O');

			mLED_S_Toggle();
			startGeneralPurpTimer(150*3);  // toggle after 150ms


			// remote other than power button is pressed
  //      	evt.eType = remoteOtherButtonDetectedEvent;
		} 
 
//        putEventIntoEventQueue(&evt);


		repeatDataflag = TRUE;
	}
}

//=====================================================================
// PUBLIC FUNCTION DELCLARATIONS
//=====================================================================
void irRemoteControlInit( void )
{
	initRxData();
	repeatDataflag = FALSE;
}


void irRemoteControl_irPinChanged( unsigned int changedIrPinState )
{
	if ( changedIrPinState != IR_RX_PIN_NORMAL ) 
	{

		if ( repeatDataflag == TRUE )
		{
			stopIrDataSpaceTimeoutTimer();
			startIrDataPulseTimeoutTimer();
			return;
		}

		stopIrDataSpaceTimeoutTimer();

		if ( irWaveformState == WAVEFORM_IDLE )
		{
			timerStartTick = ReadTimer1();
			irWaveformState = PULSE_START;
			startIrDataPulseTimeoutTimer();
			irDataStatus = DATA_START_CONDITON;

			return;
		}
		else if ( irWaveformState == SPACE_START ) 
		{
			timerEndTick = ReadTimer1();

			// Calculate the space width
			if ( timerStartTick < timerEndTick )  
			{
				spaceWidth = timerEndTick - timerStartTick;
			}
			else   // timer counter overflow
			{	
				spaceWidth = ( 0xFFFF-timerStartTick )+ timerEndTick ;
			}

			switch ( irDataStatus )
			{
		        case DATA_START_CONDITON:
				{
					if (( spaceWidth >= START_CONDITION_SPACE_MIN ) && ( spaceWidth <= START_CONDITION_SPACE_MAX ))
					{
						// Get Valid Start Condition, prepare to receive data bits
						irDataStatus = DATA_RECEIVING;
					}
					else
					{	// invalid start condition -- discard data rxed
						discardRxData();
					}
		
					timerStartTick = timerEndTick;
					irWaveformState = PULSE_START;
					startIrDataPulseTimeoutTimer();
				}
		        break;
	
		        case DATA_RECEIVING:
				{
					if (( spaceWidth >= DATA_LOG_0_SPACE_MIN ) && ( spaceWidth <= DATA_LOG_0_SPACE_MAX ))
					{
						// data bits 0 rxed
						irDataStatus = DATA_RECEIVING;
						rxDataBits[numOfBitsRxed] = 0;
						numOfBitsRxed++;
	
						timerStartTick = timerEndTick;
						irWaveformState = PULSE_START;
						startIrDataPulseTimeoutTimer();
					}
					else if (( spaceWidth >= DATA_LOG_1_SPACE_MIN ) && ( spaceWidth <= DATA_LOG_1_SPACE_MAX )) 
					{	
						// data bits 1 rxed
						irDataStatus = DATA_RECEIVING;
						rxDataBits[numOfBitsRxed] = 1;
						numOfBitsRxed++;

						timerStartTick = timerEndTick;
						irWaveformState = PULSE_START;
						startIrDataPulseTimeoutTimer();
					}
					else if (( spaceWidth >= START_CONDITION_SPACE_MIN ) && ( spaceWidth <= START_CONDITION_SPACE_MAX )) 
					{	
						// Start Condition rxed, check if the all data bits rxed, process data and restart a new data
						if ( numOfBitsRxed == IR_DATA_BITS_LENGTH )
						{
							processingRxedIrData();
						}
 
						initRxData();  // withour init the repeatDataflag
						irDataStatus = DATA_RECEIVING;
						irWaveformState = PULSE_START;
						startIrDataPulseTimeoutTimer();
						timerStartTick = timerEndTick;
					}
					else
					{	// invalid data -- discard data rxed
						discardRxData();
					}
				}
		        break;

				default:
				{
					// ERROR HERE!!! - discard data received so far
					discardRxData();
					return;
				}
				break;
			}
		}
		else
		{
			// error to discard data received so far
			discardRxData();
			return;
		}
	}
	else
	{

		if ( repeatDataflag == TRUE )
		{
			startIrDataSpaceTimeoutTimer();
			stopIrDataPulseTimeoutTimer();
			return;
		}

		stopIrDataPulseTimeoutTimer();


		if ( irWaveformState == PULSE_START ) 
		{
			timerEndTick = ReadTimer1();

			// Calculate the pulse width
			if ( timerStartTick < timerEndTick )  
			{
				pulseWidth = timerEndTick - timerStartTick;
			}
			else   // timer counter overflow
			{	
				pulseWidth = ( 0xFFFF-timerStartTick )+ timerEndTick ;
			}

			if (( pulseWidth >= PULSE_MIN ) && ( pulseWidth <= PULSE_MAX ))
			{
				irWaveformState = SPACE_START;
				startIrDataSpaceTimeoutTimer();
				timerStartTick = timerEndTick;
			}
			else
			{	
				// invalid pulse width -- discard data rxed
				discardRxData();
			}
		}
		else
		{
			// error to discard data received so far
			discardRxData();
			return;
		}
	}
}


void irDataPulseTimeoutProcessing( void )
{
	// invalid data -- discard	
	discardRxData();
}



void irDataSpaceTimeoutProcessing( void )
{
	// irDataRx ends -- check data and process the valid data
	if (( numOfBitsRxed == IR_DATA_BITS_LENGTH ) && ( repeatDataflag == FALSE ))
	{
		processingRxedIrData();
	}
	
	discardRxData();
}

