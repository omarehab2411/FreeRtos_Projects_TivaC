#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "gpio.h"

/*
This program shows how to use idel task hook to do some functionalities for example 
1-calcaulate processing time
2-drive processor to sleep mode
3-run continous code 

in this task we just flash LED on tiva board to show how hook is defined
NOTE!!! you should first change configUSE_IDLE_HOOK in freertos configuration to 1 to use idle task hook
*/


typedef enum 
{
red,blue,green
}leds;
leds led;

//idle task hook prottype
void vApplicationIdleHook( void );

//idle task hook that is executed once every idle task iteration
void vApplicationIdleHook( void )
{
 switch(led)
		{
			case red:
		// turn on Red LED on tiva board
		gpio_write(GPIOF,pin1,gpio_pin_data_high);
    //turn off other LEDS
		gpio_write(GPIOF,pin2|pin3,gpio_pin_data_low);
		//next led is blue led
			led=blue;
			break;
		
			case blue:
    //turn on blue LED on tiva board
		gpio_write(GPIOF,pin2,gpio_pin_data_high);
    //turn off other LEDS
		gpio_write(GPIOF,pin1|pin3,gpio_pin_data_low);
		//next led is green led
		led=green;		
			break;
			
			case green:
		//turn on blue LED on tiva board
		gpio_write(GPIOF,pin3,gpio_pin_data_high);
    //turn off other LEDS
		gpio_write(GPIOF,pin1|pin2,gpio_pin_data_low);
		//next led is red led
		led=red;
			break;
		
			default:
			break;
		}
}




void vLedTask(void *pvParameters)
{
	// Establish the task's period.
	const TickType_t xDelay = pdMS_TO_TICKS(100);
	TickType_t xLastWakeTime = xTaskGetTickCount();
	for (;;) {		
		
		// Block until the next release time.
		vTaskDelayUntil(&xLastWakeTime, xDelay);
	}
}

int main()
{	
	//init LED
	led=red;
	//init gpio pins for leds and interrupt of SW1 on tiva board
	gpio_clk_enable(portF);
  gpio_digital_enable(GPIOF,pin4|pin1|pin2|pin3);
  gpio_set_direction(GPIOF,pin1|pin2|pin3,gpio_pin_dir_out);


	//create a task
	xTaskCreate(vLedTask, "Task 1 " ,300, NULL , 1, NULL);

	
	// Startup of the FreeRTOS scheduler.  The program should block here.
	vTaskStartScheduler();

	// The following line should never be reached.  Failure to allocate enough
	//	memory from the heap would be one reason.
	for (;;);
	
}


