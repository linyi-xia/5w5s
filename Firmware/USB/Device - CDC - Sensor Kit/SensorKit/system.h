#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <stdio.h>
#include <string.h>
#include <p18cxxx.h>
#include <delays.h>

#include <GenericTypeDefs.h>
//#include "event.h"


#ifndef NULL
#define NULL	0
#endif	// NULL

#define SUMULATE_TIME_DIV	1      // real world
//#define SUMULATE_TIME_DIV	60				// when 60: 1min --> 1sec,  30 min --> 30sec
											// when 30: 1min --> 2sec,  30 min --> 60sec 1min
											// when 10: 1min --> 6sec,  30 min --> 180sec=3min 
void sendUSBMsg(char byte);


#endif // __SYSTEM_H__
