
// Adds support for PIC32 Peripheral library functions and macros
#include "system.h"
#include "myTimers.h"
#include "microphoneSensor.h"

#include "ioPorts.h"    // For Testing

#define MIC_SOUND_VOL_THRESH		   	50		//reading for 100mv -- 20->100mv; 40->200mv; 100->500mv; 
#define MIC_SOUNDL_ACTIVE_CNT_THRESH	3		//number of sound active data processing period -- max input voltage within the period is bigger than the MIC_SOUNDL_ACTIVE_CNT_THRESH
#define MIC_SOUND_DETECT_FRAME			10   	// time used for detect sound -- number of data processing periods  10 * 500ms = 5s

//#define MIC_SOUND_VOL_THRESH			31		//reading for 100mv -- 31->100mv; 62->200mv; 155->500mv; 310->1v; 465->1.5v; 620->2v
//#define MIC_SOUNDL_ACTIVE_CNT_THRESH	5		//number of sound active data processing period -- max input voltage within the period is bigger than the MIC_SOUNDL_ACTIVE_CNT_THRESH
//#define MIC_SOUND_DETECT_FRAME			10   	// time used for detect sound -- number of data processing periods  10 * 500ms = 5s


#define MIC_PARTY_VOL_THRESH			62		//reading for 200mv -- 62->200mv; 155->500mv; 310->1v; 465->1.5v; 620->2v
#define MIC_PARTY_ACT_CNT_THRESH		5		//number of party active data processing period -- max input voltage within the period is bigger than the MIC_PARTY_ACTIVE_CNT_THRESH
#define MIC_PARTY_DETECT_FRAME			3   	// number of MIC_SOUND_DETECT_FRAME: 3 * 500ms * 10 = 15s
#define MIC_PARTY_ACT_FRAME_THRESH		2		//number of party active data frame

unsigned int dataProcessPeriodCnt;		

unsigned int detectFramCnt;						

unsigned int soundActivePeriodCnt;

unsigned int partyActivePeriodCnt;

unsigned int partyActiveFrameCnt;

unsigned int micReading;

BOOL soundInputFlag;
BOOL partyModeFlag;

void microphoneSensorInit(void)
{
	// Config Microphone Sensor input1 pin as RA2/AN2 and input2 pin as RA3/AN3 as Analog In
	TRISAbits.TRISA2 = 1;
	ANSELAbits.ANSA2 = 1;

	TRISAbits.TRISA3 = 1;
	ANSELAbits.ANSA3 = 1;


 	dataProcessPeriodCnt = 0;	
	detectFramCnt = 0;			
	soundActivePeriodCnt = 0;

	partyActivePeriodCnt = 0;
	partyActiveFrameCnt = 0;

	micReading = 0;

	soundInputFlag = FALSE;
	partyModeFlag = FALSE;
}




void microphoneSensorUpdateReading( unsigned int curMicReading )
{
	if ( getAdcDataProcessingTimerFlag() == TRUE )   // processing after start sampling timer
	{
		if ( curMicReading > micReading )
		{
			micReading = curMicReading;
		}
    }
}


void microphoneSensorDataProcessing( void )
{
//    EventType evt;

	dataProcessPeriodCnt++;

	if ( micReading >= MIC_SOUND_VOL_THRESH )
	{
		soundActivePeriodCnt++;
	}

	if ( micReading >= MIC_PARTY_VOL_THRESH )
	{
		partyActivePeriodCnt++;
	}


	if ( dataProcessPeriodCnt == MIC_SOUND_DETECT_FRAME )
	{
		detectFramCnt++;

		if ( soundActivePeriodCnt >= MIC_SOUNDL_ACTIVE_CNT_THRESH)
		{

mLED_2_On();  
sendUSBMsg('S');
			// send USB soundInputDetectedMsg Here
//			evt.eType = soundInputDetectedEvent;
//	        putEventIntoEventQueue(&evt);
			soundInputFlag = TRUE;
		}
		else
		{
			if ( soundInputFlag == TRUE )
			{

mLED_2_Off();   
sendUSBMsg('s');
				// send USB noSoundInputDetectedMsg Here
//				evt.eType = noSoundInputDetectedEvent;
//	        	putEventIntoEventQueue(&evt);
			}

			soundInputFlag = FALSE;
		}

/*
		if ( partyActivePeriodCnt >= MIC_PARTY_ACT_CNT_THRESH)
		{
			partyActiveFrameCnt++;
		}

		if ( detectFramCnt == MIC_PARTY_DETECT_FRAME)
		{
			if ( partyActiveFrameCnt >= MIC_PARTY_ACT_FRAME_THRESH)
			{
				evt.eType = conversationOnEvent;
				putEventIntoEventQueue(&evt);

				partyModeFlag = TRUE;
			}
			else
			{
				if (  partyModeFlag == TRUE )
				{
					evt.eType = conversationOffEvent;
					putEventIntoEventQueue(&evt);
				}

				partyModeFlag = FALSE;
			}
		
			detectFramCnt = 0;
			partyActiveFrameCnt = 0;
		}
*/
		dataProcessPeriodCnt = 0;
		soundActivePeriodCnt = 0;
		partyActivePeriodCnt = 0;
	}

	micReading = 0;
}

BOOL mic_isInPartyMode( void )
{
	return partyModeFlag;
}

