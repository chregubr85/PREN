/*
 * Stepper_Driver.h
 *
 * Created: 09.02.2014 20:43:02
 *  Author: Christian
 */ 


#ifndef STEPPER_DRIVER_H_
#define STEPPER_DRIVER_H_

#include <asf.h>
#include "PWM_TC.h"




t_Stepper zAchse;
t_Stepper r1;
t_Stepper r2;

int32_t encode[3];	// Array mit den Encoderwerten für Z-Achse, R1 und R2
bool active[3];		//Aktivitätsüberwachung der drei Achsen (false = nicht aktiv)

void setPinPIOC_high(uint32_t pin);
void setPinPIOC_low(uint32_t pin);

#endif /* STEPPER_DRIVER_H_ */