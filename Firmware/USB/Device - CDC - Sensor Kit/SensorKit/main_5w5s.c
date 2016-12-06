#include "system.h"
//#include "event.h"



//-----------------------------------------------------------------------------
// Function Name       : Main
// Object              : Software entry point
//-----------------------------------------------------------------------------
void main( void )
{
//    EventType	event;

    // Init the system
    SysInit();	

    while(1)
    {
/*
        getEventFromEventQueue(&event);
		
        if (!SysHandleEvent(&event))
        {	
	        //warning: event not handled!!
	        event.eType = nullEvent;
        }
*/
    }

//	return -1;
}

