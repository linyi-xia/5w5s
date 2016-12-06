
#ifndef __ADC_MODULE_H__
#define __ADC_MODULE_H__


//=====================================================================
// PUBLIC FUNCTION DELCLARATIONS
//=====================================================================
void adcModulePinInit( void ); 
void adcModuleADCInit( void ); 
void adcSamplingInputs( void );
void ADConversionCompleted_isr(void);
void ADCStartSampling (void);
#endif // __ADC_MODULE_H__