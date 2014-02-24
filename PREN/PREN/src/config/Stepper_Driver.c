/*
 * Stepper_Driver.c
 *
 * Created: 09.02.2014 20:43:14
 *  Author: Christian
 */ 

#include <asf.h>
#include "Stepper_Driver.h"

/*Setzt ein Pin von PIOC auf High*/	
void setPinPIOC_high(uint32_t pin){
	PIOC->PIO_SODR = zAchse.M1;
}

/*Setzt ein Pin von PIOC auf Low*/
void setPinPIOC_low(uint32_t pin){
	PIOC->PIO_CODR = zAchse.M1;
}