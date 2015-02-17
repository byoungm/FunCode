#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Normal Types
typedef int INT32;
typedef long INT64;
typedef unsigned long UINT64;


#define arrLen(myArray) ( sizeof(myArray) / sizeof(myArray[0]) )
#define TIMER_INTERRUPT_AMOUNT 500 // (ms) Amount of time which has passed between each timer interrupt

// Map to timer registers
#define TIMER_HIGH_REGISTER 0
#define TIMER_LOW_REGISTER 0


//*****************************************************************************
//
//    Global Variables for Scheduler 
//
//*****************************************************************************
static UINT64 lastUpdatedCurrentTime = 0; // Keep track of time, When this overflows it resets all other timer past time to 0

// Struct to hold the data needed for an event
typedef struct 
{
	UINT64 period;
	UINT64 lastEvent ;

}SchedulerEvent_t;


union
{
	// Struct of Events
    struct SchedulerEvents_s 
    {
        SchedulerEvent_t motorControlReset;
        SchedulerEvent_t otherEvent;

    }s;

    #define NUMBER_SCHEDULER_EVENTS (sizeof(struct SchedulerEvents_s) / sizeof(SchedulerEvent_t))
    // Array of events
    SchedulerEvent_t arr[ NUMBER_SCHEDULER_EVENTS ];     
    
}schedulerEvents = 
{
	.s = 
	{
		.motorControlReset = { 1000, 0},
		.otherEvent = { 100, 0}
	}
};

//*****************************************************************************
//  Name:         interruptAddTime( void )
//
//  Description:  Adds the apprioate amount of time to lastUpdatedCurrentTime
//                variable.
//
//  Parameters:   None
//
//  Global Data:  None
//
//  Return Value: None
//*****************************************************************************
void interruptAddTime()
{
	lastUpdatedCurrentTime += TIMER_INTERRUPT_AMOUNT;
}

//*****************************************************************************
//  Name:         getCurrentTime( void )
//
//  Description:  Gets the current reference time for the event scheduler
//
//  Parameters:   None
//
//  Global Data:  lastUpdatedCurrentTime
//
//  Return Value: UINT64 - Current time in ms
//*****************************************************************************
UINT64 getCurrentTime()
{
	UINT64 timerValue = TIMER_HIGH_REGISTER;
	// Shift high register and add lower register
	timerValue = (timerValue << 8) + TIMER_LOW_REGISTER;
	return (lastUpdatedCurrentTime + timerValue);
}

//*****************************************************************************
//  Name:         main(int argc, char** argv)
//
//  Description:  Main entry point into the program
//
//  Parameters:   argc - Argument Count
//                argv - Pointer to Strings of input arguments
//
//  Global Data:  None
//
//  Return Value: SUCCESS
//*****************************************************************************
INT32 main(INT32 argc, char** argv)
{
	for (INT32 i = 0; i < arrLen(schedulerEvents.arr) ; i++)
	{
		printf("Event Index: %d, peroid(ms) = %ld, lastEvent(ms) = %ld\n", 
			i, schedulerEvents.arr[i].period, schedulerEvents.arr[i].lastEvent );
	}
	
	return 0;
}		

