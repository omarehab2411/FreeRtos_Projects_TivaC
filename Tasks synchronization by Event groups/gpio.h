///@file gpio.h
/******************************************************************
* @Title : general purpose input output(GPIO)
* @Filename : gpio.h
* @Author : Omar Ehab
* @Origin Date : 27/07/2020
* @Version : 1.0.0
* @Compiler : Keil
* @Target : Tm4c123gh6pm
* @Notes : this file contains prototypes for gpio module functions
********************************************************************/
#ifndef __GPIO__H
#define __GPIO__H

#include <stdint.h>
#include "TM4C123GH6PM.h"  
//**********************************************
//     specil consideration pins
//***********************************************
/*
pa[0:1]--->uart0
pa[5:2]--->ssio                   
pb[3:2]--->i2c0               
pc[3:0]--->jtag/swd
pd[7]---->GPIO
pf[0]----->GPIO
*/
//********************************************************************


//*******************************************************************
//              (1) Macros and enums                                 //
//*******************************************************************

//port 
#define portA (0)
#define portB (1)
#define portC (2)
#define portD (3)
#define portE (4)
#define portF (5)


//pin number
#define pin0 (1<<0)
#define pin1 (1<<1)
#define pin2 (1<<2)
#define pin3 (1<<3)
#define pin4 (1<<4)
#define pin5 (1<<5)
#define pin6 (1<<6)
#define pin7 (1<<7)

#define pin_position_0 (0)
#define pin_position_1 (1)
#define pin_position_2 (2)
#define pin_position_3 (3)
#define pin_position_4 (4)
#define pin_position_5 (5)
#define pin_position_6 (6)
#define pin_position_7 (7)

//Gpio pin data
#define gpio_pin_data_high (1)
#define gpio_pin_data_low  (0)

//Gpio pin direction
#define gpio_pin_dir_out (1)
#define gpio_pin_dir_in  (0)

//gpio pin mode
#define gpio_pin_gpioMode      (0)
#define gpio_pin_alternateMode (1)

//gpio pin driver select 
#define gpio_pin_2mA (0)
#define gpio_pin_4mA (1)
#define gpio_pin_8mA (2)

//gpio pin resistor select
#define gpio_pin_openDrain (0)
#define gpio_pin_pullUp    (1)
#define gpio_pin_pullDown  (2)

//gpio pin digital or analg enable
#define gpio_pin_digital_enable (0)
#define gpio_pin_analog_enable  (1)

// gpio pin interrupt options 
#define gpio_INT_level       (0)
#define gpio_INT_bothEdges   (1)
#define gpio_INT_risingEdge  (2)
#define gpio_INT_fallingEdge (3)


//gpio lock 
#define gpio_lock_kKey  (0x4c4f434b)  


//enable clock to gpio port
#define gpio_clk_enable(port)  (SYSCTL->RCGCGPIO|=(1<<port))  

//*********************************************************************


//************************************************************************
//                      (2) Gpio driver APIS
//************************************************************************
void gpio_set_mode(GPIOA_Type *pGpio,uint8_t pins,uint8_t mode);
uint8_t gpio_get_mode(GPIOA_Type *pGpio,uint8_t pins);

void gpio_digital_enable(GPIOA_Type *pGpio,uint8_t pins);
void gpio_digital_disable(GPIOA_Type *pGpio,uint8_t pins);

void gpio_analog_enable(GPIOA_Type *pGpio,uint8_t pins);
void gpio_analog_disable(GPIOA_Type *pGpio,uint8_t pins);


void gpio_set_direction(GPIOA_Type *pGpio,uint8_t pins,uint8_t direction);
uint8_t gpio_get_direction(GPIOA_Type *pGpio,uint8_t pins);


void gpio_resistor_enable (GPIOA_Type *pGpio,uint8_t pins,uint8_t resistor);
void gpio_resistor_disable(GPIOA_Type *pGpio,uint8_t pins,uint8_t resistor);


void gpio_set_currentDrive (GPIOA_Type *pGpio,uint8_t pins,uint8_t driveCurrent);

void gpio_set_alternateFunction(GPIOA_Type *pGpio,uint8_t pin,uint8_t functionValue);

void gpio_write(GPIOA_Type *pGpio,uint8_t pins,uint8_t gpioData);
uint8_t gpio_read(GPIOA_Type *pGpio,uint8_t pins);


void gpio_interrupt_enable (GPIOA_Type *pGpio,uint8_t pins,uint8_t interruptMode,uint8_t irqNumber,uint8_t priority);
void gpio_interrupt_mask (GPIOA_Type *pGpio,uint8_t pins);
uint8_t gpio_interrupt_status (GPIOA_Type *pGpio,uint8_t pins);
void gpio_interrupt_flag_clear(GPIOA_Type *pGpio,uint8_t pins);

void gpio_specialFunction_unlock(GPIOA_Type *pGpio);
//*************************************************************************

#endif