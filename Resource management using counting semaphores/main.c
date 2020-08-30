#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "gpio.h"

/*
This program shows how to use counting semaphores to restrict number of tasks that can access a resource 
where a 3 tasks instance is created from led task which simply takes the semaphore by decreasing its count if it's 
count is not equal to zero which means the resource has not reach max number of tasks to acces it after task instance take 
the semaphore it turn on red led on tiva board for max of 3 task instance that hasap period of 1 seconds the red led 
will be on for 3 seconds before being turned off.
*/


//constant that define maximum number of tasks that is allowed to access LED resource
#define maxTasksAllowed (3)

//create a semaphore handler
SemaphoreHandle_t Semaphore;

//variable to hold count of semaphore for debugging
int semaphore_count=0;

void vLedTask(void *pvParameters)
{
  
	// Establish the task's period.
	const TickType_t xDelay = pdMS_TO_TICKS(1000);
	TickType_t xLastWakeTime = xTaskGetTickCount();

	for (;;) {		
		
		// Block until the next release time.
		vTaskDelayUntil(&xLastWakeTime, xDelay);
		
		//if semaphore is taken which means this task instance has access to the LED
	  if( xSemaphoreTake(Semaphore,0)==pdPASS )
		{
		//turn on the corresponding led 
		gpio_write(GPIOF,pin1,gpio_pin_data_high);
		semaphore_count=uxSemaphoreGetCount(Semaphore);
		}
		else
		{
		//semaphore not taken
		gpio_write(GPIOF,pin1,gpio_pin_data_low);
		}
	
	}

}

int main()
{	
	
	//init gpio pins for leds on tiva board
	gpio_clk_enable(portF);
  gpio_digital_enable(GPIOF,pin2|pin3|pin1);
  gpio_set_direction(GPIOF,pin2|pin3|pin1,gpio_pin_dir_out);
	
	//create counting semaphore by initializing its value to 3 which means it can only be given 3 times allowing 3s tasks to access the resource
	Semaphore=xSemaphoreCreateCounting(maxTasksAllowed,maxTasksAllowed);
	
	//create three instances of the same tasks that access resource (LED)
	xTaskCreate(vLedTask, "Task 1 " ,130, NULL , 1, NULL);
	xTaskCreate(vLedTask, "Task 2 " ,130, NULL , 1, NULL);
	xTaskCreate(vLedTask, "Task 3 " ,130, NULL , 1, NULL);
	
	// Startup of the FreeRTOS scheduler.  The program should block here.
	vTaskStartScheduler();

	// The following line should never be reached.  Failure to allocate enough
	//	memory from the heap would be one reason.
	for (;;);
	
}

