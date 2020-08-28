#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include "queue.h"                     
#include "gpio.h"

/*
This program uses queue to send data every 1sec from task2 to task1 which receives data every 1sec from 
task2 and accroding to data sent task1 blink LED on tiva board with different colour
*/



//queue length
#define queueLength (1)
//create queue handle
QueueHandle_t queue;


void vPeriodicTask1(void *pvParameters)
{
char x=0;
// Establish the task's period.
const TickType_t xDelay = pdMS_TO_TICKS(1000);
TickType_t xLastWakeTime = xTaskGetTickCount();
	for (;;) 
	{		
		//receive data from queue into variable X
	if( xQueueReceive(queue,&x,20) )
	{
	switch (x)
	{
		  case 1:
			gpio_write(GPIOF,pin1,gpio_pin_data_high);
			gpio_write(GPIOF,pin2,gpio_pin_data_low);
			gpio_write(GPIOF,pin3,gpio_pin_data_low);
			break;
		
			case 2:
			gpio_write(GPIOF,pin2,gpio_pin_data_high);
			gpio_write(GPIOF,pin1,gpio_pin_data_low);
			gpio_write(GPIOF,pin3,gpio_pin_data_low);
			break;
			
			case 3:
			gpio_write(GPIOF,pin2,gpio_pin_data_low);
			gpio_write(GPIOF,pin1,gpio_pin_data_low);
			gpio_write(GPIOF,pin3,gpio_pin_data_high);
			break;
	}
}
	// Block until the next release time.
	vTaskDelayUntil(&xLastWakeTime, xDelay);
}

}



void vPeriodicTask2(void *pvParameters)
{
	static char x[3]={1,2,3};
	// Establish the task's period.
	const TickType_t xDelay = pdMS_TO_TICKS(1000);
	TickType_t xLastWakeTime = xTaskGetTickCount();
  static int i=0;
	for (;;) 
	{		
		
		if( xQueueSendToBack(queue,&x[i],20) )
		{
		//data sent to queue successfully 
			i++;
			if(i==4)
			{
			 i=0;
			}
		}
		else 
		{
		//data not sent 
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
	
	//create queue 
  queue=xQueueCreate(queueLength,sizeof(char));
	
	//create two tasks one that set events and one that blocks for events
	xTaskCreate(vPeriodicTask1, "My Task1 " ,200,  NULL, 1, NULL);
	xTaskCreate(vPeriodicTask2, "My Task2 " ,130,  NULL, 1, NULL);
	
	// Startup of the FreeRTOS scheduler.  The program should block here.
	vTaskStartScheduler();

	// The following line should never be reached.  Failure to allocate enough
	//	memory from the heap would be one reason.
	for (;;);
	
}

