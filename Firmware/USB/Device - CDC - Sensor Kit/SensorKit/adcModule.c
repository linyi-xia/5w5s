/*********************************************************************
 * FileName:        adcModule.c
 * Description:     The module is used to config ADC10 module for the 
 *                  analog inputs configuration and sampling and reading.
 *
 *
 ********************************************************************/
#include "system.h"
#include "adcModule.h"

#include "lightSensor.h"
#include "microphoneSensor.h"

void setMicInputChannel(void);
BOOL isCurChannelMic(void);
void setLightInputChannel(void);
BOOL isCurChannelLight(void);
// Private Functions

/* set Analog channel AN3 for microphone inputs - CHS<4:0>=00011 */
void setMicInputChannel(void)
{
	ADCON0bits.CHS = 0b00011;  
}

BOOL isCurChannelMic(void)
{
	if (ADCON0bits.CHS == 0b00011)
//	if ((ADCON0bits.CHS4 == 0) && (ADCON0bits.CHS3 == 0) && (ADCON0bits.CHS2 == 0) && (ADCON0bits.CHS1 == 1) && (ADCON0bits.CHS0 == 1))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/* set Analog channel AN14 for light inputs - CHS<4:0>=01110 */
void setLightInputChannel(void)
{
	ADCON0bits.CHS = 0b01110;  

/*
	ADCON0bits.CHS4 = 0;  
	ADCON0bits.CHS3 = 1;  
	ADCON0bits.CHS2 = 1;  
	ADCON0bits.CHS1 = 1;  
	ADCON0bits.CHS0 = 0;  
*/
}

BOOL isCurChannelLight(void)
{
	if (ADCON0bits.CHS == 0b01110)
//	if ((ADCON0bits.CHS4 == 0) && (ADCON0bits.CHS3 == 1) && (ADCON0bits.CHS2 == 1) && (ADCON0bits.CHS1 == 1) && (ADCON0bits.CHS0 == 0))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


// Public Functions
void	adcModulePinInit( void )
{
	lightSensorInit();
	microphoneSensorInit();
}

void	adcModuleADCInit( void )
{
	/* Configure the ADC, most of this is the
	* default settings:
	* Fosc/64
	* 20Tad
	* Interrupt with low priority
	* Analog switch between AN3 for mic and AN14 for light
	* Internal voltage references
	* ADFM = 0 -- Left Justfied Formating
	*/


	/* FOSC/64 clock select */
	ADCON2bits.ADCS0 = 0;
	ADCON2bits.ADCS1 = 1;
	ADCON2bits.ADCS2 = 1;
	
	/* TACQ = 20Tad */
	ADCON2bits.ACQT0 = 1;   
	ADCON2bits.ACQT1 = 1;   
	ADCON2bits.ACQT2 = 1;   

	/* ADFM = 1 -- Right Justfied Formating */
	ADCON2bits.ADFM = 1;

	/* Internal reference  Vss and Vdd */
	ADCON1 = 0x00;
	
	/* enable AN interrupt with low priority */
	PIE1bits.ADIE = 1;
	IPR1bits.ADIP = 0;

	/* Clear the peripheral interrupt flags */
	PIR1 = 0;
	
	/* set Analog channel AN3 for microphone inputs - CHS<4:0>=00011 */	
	setMicInputChannel();

	/* Turn on the ADC */
	ADCON0bits.ADON = 1;
	
	/* Give the ADC time to get ready. */
	Delay100TCYx (2);

//	ADCON0bits.GO = 1;
}

void ADCStartSampling (void)
{
	/* set Analog channel AN3 for microphone inputs - CHS<4:0>=00011 */	
	setMicInputChannel();

	ADCON0bits.GO = 1;
}


//ADC ISR
//Specify ADC Interrupt Routine, Priority Level = 6
void ADConversionCompleted_isr(void) 
{
	unsigned int offset;	// buffer offset to point to the base of the idle buffer
	unsigned int lightSensorReading;
	unsigned int micSensorReading;

	if ( isCurChannelMic() == TRUE )
	{
		micSensorReading = ADRESH;
		micSensorReading = ( micSensorReading << 8 ) + ADRESL;

		// processing readings
		microphoneSensorUpdateReading(micSensorReading);

		//switch channel from MIC to channel Light input
		setLightInputChannel();
		ADCON0bits.GO = 1;
	}
	else if ( isCurChannelLight() == TRUE )
	{
		lightSensorReading = ADRESH;
		lightSensorReading = ( lightSensorReading << 8 ) + ADRESL;

		// processing readings
		lightSensorUpdateReading(lightSensorReading);

		//switch channel from light input to MIC input
//		setMicInputChannel();
//		ADCON0bits.GO = 1;
	}


}


