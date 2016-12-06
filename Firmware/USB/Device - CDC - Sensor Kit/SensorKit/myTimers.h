#ifndef __MYTIMERS_H__
#define __MYTIMERS_H__

#include <timers.h>
#include "system.h"


#define NUM_OF_T1_TICKS_100_USEC			125     // 100us = 125 * 800ns

//=====================================================================
// PUBLIC FUNCTION DELCLARATIONS
//=====================================================================
void timer2_isr (void);

void timersInit ( void );
long TimeGetTicks( void );
void mSecDelay( long delay);

// adcSamplingTimer - repeat timer
void startAdcSamplingTimer(void);
void stopAdcSamplingTimer(void);
BOOL getAdcSamplingTimerFlag (void);

// adcDataProcessingTimer - repeat timer
void startAdcDataProcessingTimer(void);
void stopAdcDataProcessingTimer(void);
BOOL getAdcDataProcessingTimerFlag (void);

// buttonInputDebounceTimer - one time timer
void startButtonInputDebounceTimer(void);
void stopButtonInputDebounceTimer(void);
BOOL getButtonInputDebounceFlag (void);

// sensorInputActiveTimer - one time timer
void startSensorInputActiveTimer(void);
void stopSensorInputActiveTimer(void);
void resetSensorInputActiveTimer(void);
BOOL getSensorInputActiveTimerFlag (void);

// irDataPulseTimeoutTimer - one time timer
void startIrDataPulseTimeoutTimer(void);
void stopIrDataPulseTimeoutTimer(void);
BOOL getIrDataPulseTimeoutTimerFlag (void);

// irDataSpaceTimeoutTimer - one time timer
void startIrDataSpaceTimeoutTimer(void);
void stopIrDataSpaceTimeoutTimer(void);
BOOL getIrDataSpaceTimeoutTimerFlag (void);

// usecGenTimer - one time timer
void startUsecGenTimer(unsigned int numOfUsec);
void stopUsecGenTimer(void);
BOOL getUsecGenTimerFlag (void);

// generalPurpTimer - one time timer
void startGeneralPurpTimer(unsigned int numOfMsec);
void stopGeneralPurpTimer(void);
BOOL getGeneralPurpTimerFlag (void);

#endif // __MYTIMERS_H__
