#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "gpio.h"

/*
This program illustrates how to syncrhonize execution of a task to ISR by using a binary semaphore the program 
syncrhonize the task to ISR of switch1 on tiva board where when switch is pressed interrupt is fired which 
gives the semaphore and the task since it is blocking on the semaphore will be changed to ready state and then shen 
context switch occur the task will run to do the processing we want to do which in this case turn on Red LED on Tiva board
*/




//create a semaphore handler
SemaphoreHandle_t Semaphore;

//Interrupt service routine of GPIOF 
void GPIOF_Handler (void)
{
//variable that will be updated by the semaphore API to indicate a context switch is needed so ISR call schedule 
BaseType_t contextRequst;
//initialize it to fals which means no context required now 
contextRequst=pdFALSE;
//give semaphore by incrementing it to indicate ISR occured 
xSemaphoreGiveFromISR(Semaphore,&contextRequst);
//If higher priority task is unblocked due to giving the semaphore then do a context switch by calling the scheduler
portYIELD_FROM_ISR(contextRequst);
}


void vLedTask(void *pvParameters)
{
	// Establish the task's period.
	const TickType_t xDelay = pdMS_TO_TICKS(100);
	TickType_t xLastWakeTime = xTaskGetTickCount();
	for (;;) {		
		//wait till semaphore is taken which means the switch on tiva board has been pressed so task is synchronized with event from ISR
	  xSemaphoreTake(Semaphore,portMAX_DELAY);
	  //switch pressed turn on Red LED on tiva board
		gpio_write(GPIOF,pin1,gpio_pin_data_high);
		// Block until the next release time.
		vTaskDelayUntil(&xLastWakeTime, xDelay);
	}
}

int main()
{	
	
	//init gpio pins for leds and interrupt of SW1 on tiva board
	gpio_clk_enable(portF);
  gpio_digital_enable(GPIOF,pin4|pin1);
  gpio_set_direction(GPIOF,pin1,gpio_pin_dir_out);
	gpio_set_direction(GPIOF,pin4,gpio_pin_dir_in);
	gpio_resistor_enable(GPIOF,pin4,gpio_pin_pullUp);
	//setting priority should be equal or higher than value specified by configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY in the 
	//freertos config file if interrupt priority is lower than this value the interrupt will be masked automatically by freertos kernel
	gpio_interrupt_enable(GPIOF,pin4,gpio_INT_level,30,5);

	
	//create binary semaphore to be used to synchrnoize LED task to ISR which indicated switch 1 on tiva is pressed
	Semaphore=xSemaphoreCreateBinary();
	
	//create three instances of the same tasks that access resource (LED)
	xTaskCreate(vLedTask, "Task 1 " ,300, NULL , 1, NULL);

	
	// Startup of the FreeRTOS scheduler.  The program should block here.
	vTaskStartScheduler();

	// The following line should never be reached.  Failure to allocate enough
	//	memory from the heap would be one reason.
	for (;;);
	
}


