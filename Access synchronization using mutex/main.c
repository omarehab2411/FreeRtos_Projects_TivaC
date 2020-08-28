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



//create a semaphore handler
SemaphoreHandle_t Semaphore;

void vPeriodicTask(void *pvParameters)
{
	
	// Establish the task's period.
	const TickType_t xDelay = pdMS_TO_TICKS(1000);
	TickType_t xLastWakeTime = xTaskGetTickCount();

	for (;;) {		
		// Block until the next release time.
		vTaskDelayUntil(&xLastWakeTime, xDelay);
		
		 if(xSemaphoreTake(Semaphore,pdMS_TO_TICKS(500))==pdPASS)
		{
		//access shared resource	
	  gpio_write(GPIOF,pin2,gpio_pin_data_high);
		xSemaphoreGive(Semaphore);
		}
		else 
		{
		//semaphore not given or timeout passed
		}
		
		
	}

}


void vPeriodicTask2(void *pvParameters)
{
	
	// Establish the task's period.
	const TickType_t xDelay = pdMS_TO_TICKS(100);
	TickType_t xLastWakeTime = xTaskGetTickCount();

	for (;;) 
	{		
		// Block until the next release time.
    if(xSemaphoreTake(Semaphore,pdMS_TO_TICKS(50))==pdPASS)
		{
		//access shared resource	
	  gpio_write(GPIOF,pin3,gpio_pin_data_high);
		xSemaphoreGive(Semaphore);
		}
		else 
		{
		//semaphore not given or timeout passed
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
	
	//create mutex
	Semaphore=xSemaphoreCreateMutex();
	
	//create two tasks that access resource (LED)
	xTaskCreate(vPeriodicTask,  "My Task1 " ,200,  NULL,  2, NULL);
	xTaskCreate(vPeriodicTask2, "My Task2 " ,130,  NULL,  1, NULL);
	
	// Startup of the FreeRTOS scheduler.  The program should block here.
	vTaskStartScheduler();

	// The following line should never be reached.  Failure to allocate enough
	//	memory from the heap would be one reason.
	for (;;);
	
}

