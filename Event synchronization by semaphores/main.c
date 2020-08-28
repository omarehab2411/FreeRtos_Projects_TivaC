#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "gpio.h"

/*
This program is trying to show how to access a shared resource using mutex
the shared resource here is the LED on the tiva board where two tasks T1 T2 try to 
access the LED by acquiring the mutex first from each other.
*/

// we want to synchronize task 1 to an event when x=1000
int x=0;

//create a semaphore handler
SemaphoreHandle_t Semaphore;

void vPeriodicTask1(void *pvParameters)
{
	
	// Establish the task's period.
	const TickType_t xDelay = pdMS_TO_TICKS(100);
	TickType_t xLastWakeTime = xTaskGetTickCount();

	for (;;) {		
		
		 if(xSemaphoreTake(Semaphore,pdMS_TO_TICKS(50))==pdPASS)
		{
		//event occured so light on LED	
	  gpio_write(GPIOF,pin2,gpio_pin_data_high);
		x=0;
		}
		else 
		{
		//semaphore not given or timeout passed
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
		//when event is reached which is x=1000;
		if(x==60)
		{
		//signal event occurence by signaling (giving a semaphore)
		xSemaphoreGive(Semaphore);
		}
	 	vTaskDelayUntil(&xLastWakeTime, xDelay);
	}

}
	


int main()
{	
	//init gpio pins for green and blue led on tiva board
	gpio_clk_enable(portF);
  gpio_digital_enable(GPIOF,pin2|pin3);
  gpio_set_direction(GPIOF,pin2|pin3,gpio_pin_dir_out);
	
	//create binary semaphore
	Semaphore=xSemaphoreCreateBinary();
	
	//create two tasks that access resource (LED)
	xTaskCreate(vPeriodicTask1, "My Task1 " ,200,  NULL, 2, NULL);
	xTaskCreate(vPeriodicTask2, "My Task2 " ,130,  NULL, 1, NULL);
	
	// Startup of the FreeRTOS scheduler.  The program should block here.
	vTaskStartScheduler();

	// The following line should never be reached.  Failure to allocate enough
	//	memory from the heap would be one reason.
	for (;;);
	
}

