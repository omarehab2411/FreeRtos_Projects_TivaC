#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <event_groups.h>
#include "gpio.h"

/*
This program synchronize two tasks T1 and T2 wehn they reach certain time after both of them reaach synchronization point
they synchroize with each other and blink red and blue LED on tiva c
*/


//bits to set in event group
#define event1 (1<<0)
#define event2 (1<<1)
#define event3 (1<<3)

//resource to synchronize events to 
int x=0;

//create an event group handler
EventGroupHandle_t eventGroup;


void vPeriodicTask1(void *pvParameters)
{
	// Establish the task's period.
	const TickType_t xDelay = pdMS_TO_TICKS(100);
	TickType_t xLastWakeTime = xTaskGetTickCount();
  static int x=0;
	for (;;) {		
		x++;
	//synchronization point reached
	if	( x==60 )
	{
		//set bit in event group and wait indefinetly to synchronize with task2
	 if(xEventGroupSync(eventGroup,event1,event1|event2,portMAX_DELAY)== (event1|event2) )
	 {
		//both task synchronized
	 gpio_write(GPIOF,pin3,gpio_pin_data_high);
	 }
	}
  else 
	{
	//not synchronized yet
	}		
	
	// Block until the next release time.
	vTaskDelayUntil(&xLastWakeTime, xDelay);
	}

}


void vPeriodicTask2(void *pvParameters)
{
	
	// Establish the task's period.
	const TickType_t xDelay = pdMS_TO_TICKS(1000);
	TickType_t xLastWakeTime = xTaskGetTickCount();

	for (;;) 
	{		
		x++;
//when events synchronization point is reached send a bit in event group
	if(x==10)
	 {
	//wait indefinetly for synchronization with task1
 if(xEventGroupSync(eventGroup,event2,event1|event2,portMAX_DELAY)==(event1|event2))
	{ 
		 //task1 and task2 both  synchronized
		 gpio_write(GPIOF,pin2,gpio_pin_data_high);
	}
	 
	 }
	 	vTaskDelayUntil(&xLastWakeTime, xDelay);
	}

}
	


int main()
{	
	//init gpio pins for green and blue led on tiva board
	gpio_clk_enable(portF);
  gpio_digital_enable(GPIOF,pin2|pin3|pin1);
  gpio_set_direction(GPIOF,pin2|pin3|pin1,gpio_pin_dir_out);
	
	//create event group
	eventGroup=xEventGroupCreate();
	
	//create two tasks one that set events and one that blocks for events
	xTaskCreate(vPeriodicTask1, "My Task1 " ,200,  NULL, 2, NULL);
	xTaskCreate(vPeriodicTask2, "My Task2 " ,130,  NULL, 1, NULL);
	
	// Startup of the FreeRTOS scheduler.  The program should block here.
	vTaskStartScheduler();

	// The following line should never be reached.  Failure to allocate enough
	//	memory from the heap would be one reason.
	for (;;);
	
}

