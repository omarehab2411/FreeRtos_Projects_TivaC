#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "gpio.h"

/*
This program uses counting semaphore to latch number of events occured from interrupt and defer its execution 
to the periodic task since events from ISR is latched then no interrupt processing will be missed 
the isr here is triggered when switch 1 is pressed on tiva board and the task will respond by taking the semphore annd 
turn on LED on tiva board accordingly
*/


typedef enum 
{
red,blue,green
}leds;
leds led;

//create a semaphore handler
SemaphoreHandle_t Semaphore;
//semaphore maximum count
#define MaxCount (10)
//semaphore initial value is zero as it is used for counting events not resource management
#define InitialValue (0)


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
	  switch(led)
		{
			case red:
		//switch pressed turn on Red LED on tiva board
		gpio_write(GPIOF,pin1,gpio_pin_data_high);
    //turn off other LEDS
		gpio_write(GPIOF,pin2|pin3,gpio_pin_data_low);
		//next led is blue led
			led=blue;
			break;
		
			case blue:
    //switch pressed turn on blue LED on tiva board
		gpio_write(GPIOF,pin2,gpio_pin_data_high);
    //turn off other LEDS
		gpio_write(GPIOF,pin1|pin3,gpio_pin_data_low);
		//next led is green led
		led=green;		
			break;
			
			case green:
		//switch pressed turn on blue LED on tiva board
		gpio_write(GPIOF,pin3,gpio_pin_data_high);
    //turn off other LEDS
		gpio_write(GPIOF,pin1|pin2,gpio_pin_data_low);
		//next led is red led
		led=red;
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
	//init LED
	led=red;
	//init gpio pins for leds and interrupt of SW1 on tiva board
	gpio_clk_enable(portF);
  gpio_digital_enable(GPIOF,pin4|pin1|pin2|pin3);
  gpio_set_direction(GPIOF,pin1|pin2|pin3,gpio_pin_dir_out);
	gpio_set_direction(GPIOF,pin4,gpio_pin_dir_in);
	gpio_resistor_enable(GPIOF,pin4,gpio_pin_pullUp);
	//setting priority should be equal or higher than value specified by configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY in the 
	//freertos config file if interrupt priority is lower than this value the interrupt will be masked automatically by freertos kernel
	gpio_interrupt_enable(GPIOF,pin4,gpio_INT_level,30,5);

	
	//create counting semaphore with initial value 0 to e used to count interrupts occured to be defred further by 
	//the periodic task created the ISR here will fire very time switch 1 on tiva board is pressed and task will 
	//take semaphore whenever it is possible to turn on LED of tiva board
	Semaphore=xSemaphoreCreateCounting(MaxCount,InitialValue);
	
	//create three instances of the same tasks that access resource (LED)
	xTaskCreate(vLedTask, "Task 1 " ,300, NULL , 1, NULL);

	
	// Startup of the FreeRTOS scheduler.  The program should block here.
	vTaskStartScheduler();

	// The following line should never be reached.  Failure to allocate enough
	//	memory from the heap would be one reason.
	for (;;);
	
}


