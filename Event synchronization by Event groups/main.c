#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <event_groups.h>
#include "gpio.h"

/*
This program sets bits in event group shen variable x reaches certain values
then task1 wait for all events to happen in the event group to ligh LEDS on tiva board
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

	for (;;) {		
		
	//wait for all events to happen when it does light the LEDS of tiva c
	if	( xEventGroupWaitBits(eventGroup,event1|event1|event3,1,1,20) )
	{
	 gpio_write(GPIOF,pin2|pin3|pin1,gpio_pin_data_high);
	}
  else 
	{
	//events bits not set 
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
		//when events is reached signal it in the event group
		switch(x)
		{
			case 1:
				xEventGroupSetBits(eventGroup,event1);
			break;
			
			case 2:
				xEventGroupSetBits(eventGroup,event2);
			break;
			
			case 3:
				xEventGroupSetBits(eventGroup,event3);
			break;
		
			default:
			break;
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

