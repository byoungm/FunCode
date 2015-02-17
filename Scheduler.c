#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Normal Types
typedef long INT64;
typedef unsigned long UINT64;


#define arrLen(myArray) ( sizeof(myArray) / sizeof(myArray[0]) )

static UINT64 lastUpdatedCurrentTime = 0;

typedef struct 
{
	char name[20];
	UINT64 period;
	UINT64 lastEvent ;

}SchedulerEvent_t;


/////////////////////////////////////////
//
//    Event Holder for the Scheduler
//
////////////////////////////////////////
union // Keep shared memory for list of scheduler events in struct and array form
{
	// Struct of Events
    struct SchedulerEvents_s
    {
        SchedulerEvent_t motorControlReset;
        SchedulerEvent_t otherEvent;
    }s;
    // Array of events
    #define NUMBER_SCHEDULER_EVENTS (sizeof(struct SchedulerEvents_s) / sizeof(SchedulerEvent_t))
    SchedulerEvent_t arr[ NUMBER_SCHEDULER_EVENTS ];
    
}schedulerEvents = 
{
	.s = 
	{
		.motorControlReset = {"MotorControlTimer", 1000, 0},
		.otherEvent = {"Other Event", 100, 0}
	}
};

// Methods
UINT64 getCurrentTime()
{
	UINT64 timer0Value = 0;
	return (lastUpdatedCurrentTime + timer0Value);
}


int main()
{
	schedulerEvents.s.otherEvent.lastEvent = 500;
	strcpy(schedulerEvents.s.otherEvent.name, "This works");
	for (int i = 0; i < arrLen(schedulerEvents.arr) ; i++)
	{
		printf("Timer: Name  = %s, peroid(ms) = %ld, lastEvent(ms) = %ld\n", 
			schedulerEvents.arr[i].name, schedulerEvents.arr[i].period, schedulerEvents.arr[i].lastEvent );
	}
	
	return 0;
}		

