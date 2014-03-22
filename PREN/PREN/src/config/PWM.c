/*
 * PWM.c
 *
 * Created: 08.02.2014 21:26:48
 *  Author: Christian
 */ 

#include "PWM.h"
#include <sysclk.h>
#include <asf.h>
#include <stdarg.h>
#include "Stepper_Driver.h"

#define STEPS_RAMP 1000

uint32_t count_z = 0;
uint32_t count_r1 = 0;
uint32_t count_r2 = 0;
uint32_t g_steps_z = 0;
uint32_t g_steps_r1 = 0;
uint32_t g_steps_r2 = 0;
uint32_t captured = 0;


/*Gibt den Wert f�r f�r das RC Register zur�ck, -> wie "weit" soll
der Timer laufen.*/
int getValueRCforFreq(int freq){
		int timerCLK = 0;
					
			if(freq>=640)
			{
				timerCLK = sysclk_get_peripheral_hz()/2;
			}
			if(freq>=160 && freq<640){
				timerCLK = sysclk_get_peripheral_hz()/8;
			}
			if(freq>=40 && freq<160){
				timerCLK = sysclk_get_peripheral_hz()/32;
			}
			if(freq<40){
				timerCLK = sysclk_get_peripheral_hz()/128;
			}
	return (timerCLK/freq);	
}

/*Gibt den Prescaler (MCK/2, MCK/8, MCK/32 oder MCK/128) f�r die gew�nschte
Frequenz zur�ck*/
int getPrescaler(int freq){
	int prescale = 0;
	
		if(freq>=640)
		{
			return TC_CMR_TCCLKS_TIMER_CLOCK1;
		}
		if(freq>=160 && freq<640){
			return TC_CMR_TCCLKS_TIMER_CLOCK2;
		}
		if(freq>=40 && freq<160){
			return TC_CMR_TCCLKS_TIMER_CLOCK3;;
		}
		else{
			return TC_CMR_TCCLKS_TIMER_CLOCK4;
		}
}

int getFreqeunz(t_PinPwm pin){
	if(pin.prescale == 0){
		return (sysclk_get_peripheral_hz()/2)/tc_read_rc(pin.Timercounter, pin.channel);
	}
	if(pin.prescale == 1){
		return (sysclk_get_peripheral_hz()/8)/tc_read_rc(pin.Timercounter, pin.channel);
	}
	if(pin.prescale == 2){
		return (sysclk_get_peripheral_hz()/32)/tc_read_rc(pin.Timercounter, pin.channel);
	}
	if(pin.prescale == 3){
		return (sysclk_get_peripheral_hz()/128)/tc_read_rc(pin.Timercounter, pin.channel);
	}
}




void timer_init( t_PinPwm pin, int freq )
{
	freq = 2*freq;
	ioport_set_pin_mode(pin.pin_id, pin.mux);
	ioport_disable_pin(pin.pin_id);
	
	sysclk_enable_peripheral_clock(pin.id);
	pin.prescale = getPrescaler(freq);
	
	tc_init(pin.Timercounter, pin.channel,
	pin.prescale
	| TC_CMR_WAVE /* Waveform mode */
	| TC_CMR_WAVSEL_UPDOWN /*Center Allaigned*/
	| TC_CMR_ACPA_CLEAR /*Set bei RA */				
	| TC_CMR_ACPC_SET /* Clear bei RC */
	| TC_CMR_CPCTRG /* Trigger bei RC */
	);
	/*Interrupt enable*/
		pin.Timercounter->TC_CHANNEL[pin.channel].TC_IER = TC_IER_CPCS;
		pin.Timercounter->TC_CHANNEL[pin.channel].TC_IER =~ TC_IDR_CPCS;
	/*Wert f�r RC schreiben*/					
		tc_write_rc(pin.Timercounter, pin.channel, getValueRCforFreq(freq));	
}

void numberOfSteps(t_PinPwm pwm, int steps){
	
	
	
	/*Interrupt PWM Z-Achse*/
	if(pwm.Timercounter == TC0 && pwm.channel == 0){
		g_steps_z = steps;
		NVIC_EnableIRQ(TC0_IRQn);
		tc_start(pwm.Timercounter, pwm.channel);;
	}
	/*Interrupt PWM R1*/
	if(pwm.Timercounter == TC2 && pwm.channel == 1){
		g_steps_r1 = steps;
		NVIC_EnableIRQ(TC7_IRQn);
		tc_start(pwm.Timercounter, pwm.channel);
	}
	/*Interrupt PWM R2*/
	if(pwm.Timercounter == TC2 && pwm.channel == 0){
		g_steps_r2 = steps;
		NVIC_EnableIRQ(TC6_IRQn);
		tc_start(pwm.Timercounter, pwm.channel);
	}
}


/*ISR PWM2 Z-ACHSE*/
void TC0_Handler(){
	TC0->TC_CHANNEL[0].TC_SR;
	count_z++;
	if(count_z == g_steps_z){
		tc_stop(TC0,0);
		count_z = 0;
	}
}



/*ISR PWM3*/
void TC7_Handler(){
	TC2->TC_CHANNEL[1].TC_SR;
	count_r1++;
	if(count_r1 == g_steps_r1){
		tc_stop(TC2,1);
		count_r1 = 0;
	}
}

/*ISR PWM5*/
void TC6_Handler(){
	TC2->TC_CHANNEL[0].TC_SR;
	count_r2++;
	if(count_r2 == g_steps_r2){
		tc_stop(TC2,0);
		count_r2 = 0;
	}
}

/*ISR PWM11*/
void TC8_Handler(){
	TC2->TC_CHANNEL[2].TC_SR;
	captured++;
}

/*ISR PIOD*/
void PIOD_ISR(uint32_t id, uint32_t mask)
{
	if (ID_PIOD == id && PIO_PD0 == mask){
	}	
	if (ID_PIOD == id && PIO_PD1 == mask){
	}	
	if (ID_PIOD == id && PIO_PD2 == mask){
	}
}