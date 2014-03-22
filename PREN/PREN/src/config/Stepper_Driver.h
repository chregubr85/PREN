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


typedef struct {
	uint32_t ALERT;
	uint32_t RESET;
	uint32_t M1;
	uint32_t M2;
	uint32_t M3;
	uint32_t ENBLE;
	t_PinPwm pwm;
	uint32_t CW_CCW;
	uint32_t ENC_A;
	uint32_t ENC_B;
	}t_Stepper;

t_Stepper zAchse;
t_Stepper r1;
t_Stepper r2;

void setPinPIOC_high(uint32_t pin);
void setPinPIOC_low(uint32_t pin);

#endif /* STEPPER_DRIVER_H_ */