#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include "gpio.h"


typedef enum
{
off,on
}ledstate;
ledstate blue;

void vPeriodicTask(void *pvParameters)
{
	
	// Establish the task's period.
	const TickType_t xDelay = pdMS_TO_TICKS(1000);
	TickType_t xLastWakeTime = xTaskGetTickCount();

	for (;;) {		
		
		switch (blue)
		{
			case off:
					gpio_write(GPIOF,pin2,gpio_pin_data_high);
			    blue=on;
				break;
			
			case on:
				gpio_write(GPIOF,pin2,gpio_pin_data_low);
			  blue=off;
				break;
			
			default:
				break;
		
		}
		// Block until the next release time.
		vTaskDelayUntil(&xLastWakeTime, xDelay);
	}

}

	
int main()
{
	//init blue led state
	blue=off;
	
	//init gpio pins
	gpio_clk_enable(portF);
  gpio_digital_enable(GPIOF,pin2);
  gpio_set_direction(GPIOF,pin2,gpio_pin_dir_out);
	
	//create blink led task
	xTaskCreate(vPeriodicTask, "My Task ",200, NULL, 1, NULL);
	// Startup of the FreeRTOS scheduler.  The program should block here.
	vTaskStartScheduler();

	// The following line should never be reached.  Failure to allocate enough
	//	memory from the heap would be one reason.
	for (;;);
	
}

