///@file gpio.c
/******************************************************************
* @Title : general purpose input output(GPIO)
* @Filename : gpio.c
* @Author : Omar Ehab
* @Origin Date : 27/07/2020
* @Version : 1.0.0
* @Compiler : Keil
* @Target : Tm4c123gh6pm
* @Notes : this file contains definition of Gpio module functions
*
* MIT License
*Copyright (c) [2020] [Omar Ehab abdelaziz]
*Permission is hereby granted, free of charge, to any person obtaining a copy
*of this software and associated documentation files (the "Software"), to deal
*in the Software without restriction, including without limitation the rights
*to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*copies of the Software, and to permit persons to whom the Software is
*furnished to do so, subject to the following conditions:
*
*The above copyright notice and this permission notice shall be included in all
*copies or substantial portions of the Software.
*THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*SOFTWARE
*******************************************************************/
#include "gpio.h"



//*******************************************************
//                 Function: Gpio_set_mode()
//********************************************************

/**
 *\b Description:this function is used to sed the mode of the gpio pin as either normal gpio or alternate function pin
 *\b PRE-CONDITION:gpio clock should be enabled \br
 *\b POST-CONDITION:pin is configured as either normal gpio or is set as alternate function pin
 *@param[in] pointer to gpio strcuture that contains the gpio registers as structure members     
 *@param[in] mode of pin as either gpio or alternate
 *@return    void 
 */
void gpio_set_mode(GPIOA_Type *pGpio,uint8_t pins,uint8_t mode)
{
if(mode==gpio_pin_alternateMode)
{
pGpio->AFSEL|=pins;
}
else if(mode==gpio_pin_gpioMode)
{
pGpio->AFSEL&=~pins;
}
}
//***********************************************************


//********************************************************
//                 Function: gpio_get_mode()
//********************************************************

/**
 *\b Description:this function return the mode tht this pin is set to if it is a gpio or acting as alternate function pin
 *\b PRE-CONDITION:this function shoul be called after a pin mode has already been set \br
 *\b POST-CONDITION:
 *@param[in] pointer to a structure that contain port registers as struct members
 *@param[in] pin number
 *@return    pin mode          
 */

uint8_t gpio_get_mode(GPIOA_Type *pGpio,uint8_t pins)
{
return (pGpio->AFSEL&pins);
}
//************************************************************



//*******************************************************
//                 Function: gpio_digital_enable()
//********************************************************

/**
 *\b Description: this function is used to enable the digital functionality of a gpio pin 
 *\b PRE-CONDITION: this functioned should be called after been has been set as gpio and after clock has been enabled \br
 *\b POST-CONDITION:pin is acting as digital pin 
 *@param[in] pointer to a structure that contain port registers as struct members    
 *@param[in] pin number 
 *@return    void   
 *@see      gpio_set_mode()  
 */

void gpio_digital_enable(GPIOA_Type *pGpio,uint8_t pins)
{
pGpio->DEN|=pins;
}
//*********************************************************



//*******************************************************
//                 Function: gpio_digital_disable()
//********************************************************

/**
 *\b Description:this function is used to disable the gpio pin digital feature
 *\b PRE-CONDITION:gpio pin should have been already set as digital gpio pin\br
 *\b POST-CONDITION:gpio pin digital feature is disabled 
 *@param[in] pointer to a structure that contain port registers as struct members    
 *@param[in] pin number 
 *@return void      
 */

void gpio_digital_disable(GPIOA_Type *pGpio,uint8_t pins)
{
pGpio->DEN&=~pins;
}
//*******************************************************************




//*******************************************************
//                 Function: gpio_analog_enable()
//********************************************************

/**
 *\b Description:this function is used to set gpio pin as analog pin 
 *\b PRE-CONDITION:pin should already been set into gpio mode \br
 *\b POST-CONDITION:gpio pin is set as an analog pin
 *@param[in] pointer to a structure that contain port registers as struct members    
 *@param[in] pin number  
 *@return void       
 *@see  gpio_set_mode()  
 */
void gpio_analog_enable(GPIOA_Type *pGpio,uint8_t pins)
{
	pGpio->AMSEL|=pins;
}
//*************************************************************************


//*******************************************************
//                 Function: gpio_analog_disable()
//********************************************************

/**
 *\b Description:this function is used to disable gpio pin analog feature 
 *\b PRE-CONDITION:pin should already been set into analog gpio mode \br
 *\b POST-CONDITION:gpio pin analog feature disabled
 *@param[in] pointer to a structure that contain port registers as struct members    
 *@param[in] pin number  
 *@return void       
 *@see  gpio_analog_enable()
 */
void gpio_analog_disable(GPIOA_Type *pGpio,uint8_t pins)
{
pGpio->AMSEL&=~pins;
}
//*******************************************************************




//*******************************************************
//                 Function: gpio_set direction()
//********************************************************

/**
 *\b Description:this function is used to set gpio pin as input or output pin
 *\b PRE-CONDITION: pin should have been set as digital or analog pin and clock to gpio module should have been enabled\br
 *\b POST-CONDITION: corresponding pin is set as input or output pin
 *@param[in]  pointer to a structure that contain port registers as struct members    
 *@param[in]  pin number 
 *@param[in] gpio pin direction whther input or output this values is set from the #define in the .h file  
 *@return    void  
 *@see           
 */

void gpio_set_direction(GPIOA_Type *pGpio,uint8_t pins,uint8_t direction)
{
	if(direction==gpio_pin_dir_in)
	{
		pGpio->DIR&=~pins;
	}
else if(direction==gpio_pin_dir_out)
{
pGpio->DIR|=pins;
}
}
//*******************************************************************************





//*******************************************************
//                 Function: gpio_get_direction()
//********************************************************

/**
 *\b Description:this function is used to get gpio pin direction (input or output pin).
 *\b PRE-CONDITION: pin should have been set as digital or analog pin with configured direction\br
 *\b POST-CONDITION: drection of pin is returned
 *@param[in]  pointer to a structure that contain port registers as struct members    
 *@param[in]  pin number 
 *@param[in] gpio pin direction whther input or output this values is set from the #define in the .h file  
 *@return    void           
 */
uint8_t gpio_get_direction(GPIOA_Type *pGpio,uint8_t pins)
{
return (pGpio->DIR&pins);
}
//*******************************************************************




//*******************************************************
//                 Function: gpio_resistor_enable()
//********************************************************

/**
 *\b Description:this function is used to set the type of resistor an input pin will use (pull up,pull down,open drain).
 *\b PRE-CONDITION:the pin should already been set as gpio input pin \br
 *\b POST-CONDITION:resistor for the corresponding pin is set
 *@param[in]  pointer to a structure that contain port registers as struct members    
 *@param[in]  pin number   
 *@param[in] type of resistance (pull up,pull down,open drain)  
 *@return     void  
 */
void gpio_resistor_enable (GPIOA_Type *pGpio,uint8_t pins,uint8_t resistor)
{

	switch(resistor)
	{
		case gpio_pin_pullUp:
			pGpio->PUR|=pins;
			break;
	
		case gpio_pin_pullDown:
			pGpio->PDR|=pins;
			break;
	
	case gpio_pin_openDrain:
		pGpio->ODR|=pins;
			break;
	
	default:
		break;
	
	}
	
}
//*******************************************************************************************




//*******************************************************
//                 Function: gpio_resistor_disable()
//********************************************************

/**
 *\b Description:this function is used to disable the type of resistor an input pin is already using (pull up,pull down,open drain).
 *\b PRE-CONDITION:the pin should already has set a resistor to it \br
 *\b POST-CONDITION: resistor is disabled to the corresponding pin
 *@param[in]  pointer to a structure that contain port registers as struct members    
 *@param[in]  pin number   
 *@param[in] type of resistance (pull up,pull down,open drain)  
 *@return     void  
 */
void gpio_resistor_disable(GPIOA_Type *pGpio,uint8_t pins,uint8_t resistor)
{
	switch(resistor)
	{
		case gpio_pin_pullUp:
			pGpio->PUR&=~pins;
			break;
	
		case gpio_pin_pullDown:
			pGpio->PDR&=~pins;
			break;
	
	case gpio_pin_openDrain:
		pGpio->ODR&=~pins;
			break;
	
	default:
		break;
	
	}}
//****************************************************************************************************
	
	
	
	
//*******************************************************
//                 Function: Gpio_Init()
//********************************************************

/**
 *\b Description:this function is used to set the current that the gpio pin could drve (2ma,4ma,8ma)
 *\b PRE-CONDITION:pin should already been configured as gpio pin \br
 *\b POST-CONDITION:drive current is set to the coressponding gpio pin
 *@param[in]  pointer to a structure that contain port registers as struct members    
 *@param[in]  pin number 
 *@param[in] drive current value (2ma,4ma,8ma)	
 *@return void        
 */
void gpio_set_currentDrive (GPIOA_Type *pGpio,uint8_t pins,uint8_t driveCurrent)
{

switch(driveCurrent)
{
	case gpio_pin_2mA:
		pGpio->DR2R|=pins;
		break;

	case gpio_pin_4mA:
		pGpio->DR4R|=pins;
		break;

	case gpio_pin_8mA:
		pGpio->DR8R|=pins;
		break;

	default:
		break;
}

}
//****************************************************************************************




//*******************************************************
//                 Function: gpio_alternateFunction()
//********************************************************

/**
 *\b Description:this function is used to set alternate function that a pin will be assigned to  
 *\b PRE-CONDITION:clock of gpio module should have been enabled and mode of pin should be selected as alternate mode not gpio \br
 *\b POST-CONDITION:a pin is configured for a specific function like uart,ADC,etc,,,
 *@param[in]  pointer to a structure that contain port registers as struct members    
 *@param[in]  pin number 
 *@param[in] value of the function that you want pin to be in this values could be found in tiva c datahseet 
 *@return       
 *@see          
 */
void gpio_set_alternateFunction(GPIOA_Type *pGpio,uint8_t pin,uint8_t functionValue)
{
pGpio->PCTL&=~(0b1111<<(pin*4));
pGpio->PCTL|=(functionValue<<(pin*4));	
}
//***************************************************************************




//*******************************************************
//                 Function: gpio_write()
//********************************************************

/**
 *\b Description:this function is used to write data to a gpio pin 
 *\b PRE-CONDITION:pin should already been configured as gpio digital pin\br
 *\b POST-CONDITION:pin is configured to output a high ot low voltage level
 *@param[in]  pointer to a structure that contain port registers as struct members    
 *@param[in]  pin number 
 *@param[in]  voltage level you want to output on the corresponding gpio pin (high or low)
 *@return     void  
 */
void gpio_write(GPIOA_Type *pGpio,uint8_t pins,uint8_t gpioData)
{
if(gpioData==gpio_pin_data_high)
{
pGpio->DATA|=pins;
}
else if(gpioData==gpio_pin_data_low)
{
pGpio->DATA&=~pins;	
}
}
//***********************************************************************************


//*******************************************************
//                 Function: gpio_read()
//********************************************************

/**
 *\b Description:this function is used to get input voltage level on a gpio pin 
 *\b PRE-CONDITION:pin should has been set as an input gpio pin\br
 *\b POST-CONDITION:voltage level on the corresponding pin is read
 *@param[in]  pointer to a structure that contain port registers as struct members    
 *@param[in]  pin number 
 *@return     voltage level on that pin (high,low) 
 */
uint8_t gpio_read(GPIOA_Type *pGpio,uint8_t pins)
{
return (pGpio->DATA&pins);
}
//******************************************************************************





//*******************************************************
//                 Function: Gpio_Init()
//********************************************************

/**
 *\b Description:this functon is used to enable interrupt on a certain pin 
 *\b PRE-CONDITION:the pin should be set as gpio pin \br
 *\b POST-CONDITION:the pin is able to sense signal level and generate interrupts corresponding to the signal
 *@param[in]  pointer to a structure that contain port registers as struct members    
 *@param[in]  pin number 
 *@param[in]  type of signal we want pin to sense and generate interrupt to it could be level,both edges or falling or rising edge of a signal   
 *@param[in]  the number of the interrupt in the vector table you could find that number in the startup code
 *@param[in]   priority you want to set to the interrupt handler
 *@return      void 
 */
void gpio_interrupt_enable (GPIOA_Type *pGpio,uint8_t pins,uint8_t interruptMode,uint8_t irqNumber,uint8_t priority)
{
__disable_irq();
pGpio->IM&=~pins;
switch (interruptMode)
{
	case gpio_INT_level:
	pGpio->IS|=pins;
	pGpio->IM|=pins;
	NVIC_SetPriority(irqNumber,priority);
	NVIC_EnableIRQ(irqNumber);
	break;

	case gpio_INT_bothEdges:
	pGpio->IS&=~pins;
  pGpio->IBE|=pins;
  pGpio->IM|=pins;
	NVIC_SetPriority(irqNumber,priority);
	NVIC_EnableIRQ(irqNumber);
	break;

	case gpio_INT_fallingEdge:
  pGpio->IS&=~pins;
  pGpio->IBE&=~pins;
	pGpio->IEV&=~pins;
	pGpio->ICR|=pins;
  pGpio->IM|=pins;
	NVIC_SetPriority(irqNumber,priority);
	NVIC_EnableIRQ(irqNumber);
	break;

	case gpio_INT_risingEdge:
	pGpio->IS&=~pins;
  pGpio->IBE&=~pins;
	pGpio->IEV|=pins;
	pGpio->ICR|=pins;
  pGpio->IM|=pins;
	NVIC_SetPriority(irqNumber,priority);
	NVIC_EnableIRQ(irqNumber);	
	break;

	default:
	break;
}
__enable_irq();
}


//*******************************************************
//                 Function: Gpio_Init()
//********************************************************

void gpio_interrupt_mask (GPIOA_Type *pGpio,uint8_t pins)
{
pGpio->IM&=~pins;
}


//*******************************************************
//                 Function: Gpio_Init()
//********************************************************

uint8_t gpio_interrupt_status (GPIOA_Type *pGpio,uint8_t pins)
{

	return (pGpio->RIS&pins);

}


//*******************************************************
//                 Function: Gpio_Init()
//********************************************************

void gpio_specialFunction_unlock(GPIOA_Type *pGpio)
{
pGpio->LOCK=gpio_lock_kKey;
pGpio->CR=0x01;
}




//*******************************************************
//                 Function: Gpio_Init()
//********************************************************

void gpio_interrupt_flag_clear(GPIOA_Type *pGpio,uint8_t pins)
{
pGpio->ICR|=pins;
}


