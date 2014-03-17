/*
 * PWM.h
 *
 * Created: 08.02.2014 21:27:20
 *  Author: Christian
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <asf.h>

typedef struct {
		Tc  *Timercounter;
		uint32_t channel;
		uint32_t id;
		ioport_pin_t pin_id;
		ioport_mode_t mux;
		uint32_t prescale;
	}t_PinPwm;
	
t_PinPwm pwm2;	
t_PinPwm pwm3;
t_PinPwm pwm5;
t_PinPwm pwm11;
pwm_channel_t pwm_pin_7; //Servo


int getValueRCforFreq(int freq);
int getPrescaler(int freq);
int getFreqeunz(t_PinPwm pin);
void timer_init(t_PinPwm pin, int freq);
void ramp_up(t_PinPwm pin);
void numberOfSteps(t_PinPwm pwm, int steps);
void PIOD_ISR(uint32_t id, uint32_t mask);
#endif /* PWM_H_ */