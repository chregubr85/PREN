/*
 * PWM.h
 *
 * Created: 08.02.2014 21:27:20
 *  Author: Christian
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <asf.h>
/* Define Drivermode */
#define FULLSTEP 1
#define HALFSTEP 2
#define VIERTELSTEP 4
#define ACHTELSTEP 8
#define SECHZEHNTELSTEP 16
#define CLOCKWISE true
#define COUNTERCLOCKWISE false




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

typedef struct {
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

pwm_channel_t pwm_pin_7; //Servo

/* Array mit den Encoderwerten für Z-Achse, R1 und R2
   Encode = 0 in Initialposition */
int32_t encode[3];	

bool active[3];		//Aktivitätsüberwachung der drei Achsen (false = nicht aktiv)




int getValueRCforFreq(int freq);
int getPrescaler(int freq);
int getFreqeunz(t_PinPwm pin);
void timer_init(t_PinPwm pin, int freq);
void gotoPosition(t_Stepper axis, int encValue);
int setStepperMode(t_Stepper pin, uint32_t multistep);
#endif /* PWM_H_ */