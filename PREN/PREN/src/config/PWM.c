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

uint32_t count_z = 0;
uint32_t count_r1 = 0;
uint32_t count_r2 = 0;
uint32_t g_steps_z = 0;
uint32_t g_steps_r1 = 0;
uint32_t g_steps_r2 = 0;
uint32_t count = 0;
uint32_t captured = 0;
bool flag_05_ms;

/*Gibt den Wert für für das RC Register zurück, -> wie "weit" soll
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

/*Gibt den Wert für das RA Register zurück. Das RA Register bestimmt
den Dutycycle (0-100%) des PWM's)*/
int getValueRAforDuty(int duty, int freq){
	
	return ((((100-duty)*getValueRCforFreq(freq))/100));	
}
	
/*Gibt den Prescaler (MCK/2, MCK/8, MCK/32 oder MCK/128) für die gewünschte
Frequenz zurück*/
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

void ramp_up( t_PinPwm pin, int freq )
{
	uint32_t freq_temp = 0;
	tc_stop(pin.Timercounter, pin.channel);
	while(freq_temp < freq){

	tc_init(pin.Timercounter, pin.channel,
	getPrescaler(freq_temp)
	| TC_CMR_WAVE /* Waveform mode */
	| TC_CMR_ACPA_SET /*Set bei RA */
	| TC_CMR_ACPC_CLEAR /* Clear bei RC */
	| TC_CMR_CPCTRG /* Trigger bei RC */
	);
		tc_write_rc(pin.Timercounter, pin.channel, getValueRCforFreq(freq_temp));
		tc_write_ra(pin.Timercounter, pin.channel, getValueRAforDuty(50, freq_temp));
		tc_start(pin.Timercounter, pin.channel);
		if(flag_05_ms){
		freq_temp= freq_temp +1;
		}
	}
}


void timer_init(t_PinPwm pin, int freq, int duty){
	
	ioport_set_pin_mode(pin.pin_id, pin.mux);
	ioport_disable_pin(pin.pin_id);
	
	sysclk_enable_peripheral_clock(pin.id);
	
	tc_init(pin.Timercounter, pin.channel,
	getPrescaler(freq)
	| TC_CMR_WAVE /* Waveform mode */
	| TC_CMR_ACPA_SET /*Set bei RA */
	| TC_CMR_ACPC_CLEAR /* Clear bei RC */
	| TC_CMR_CPCTRG /* Trigger bei RC */
	);
	/*Interrupt enable*/
		pin.Timercounter->TC_CHANNEL[pin.channel].TC_IER = TC_IER_CPCS;
		pin.Timercounter->TC_CHANNEL[pin.channel].TC_IER =~ TC_IDR_CPCS;
	/*Wert für RA und RC schreiben*/					
		tc_write_rc(pin.Timercounter, pin.channel, getValueRCforFreq(freq));
		tc_write_ra(pin.Timercounter, pin.channel, getValueRAforDuty(duty, freq));	
}

void numberOfSteps(t_PinPwm pwm, int steps){
	
	/*Interrupt PWM Z-Achse*/
	if(pwm.Timercounter == TC0 && pwm.channel == 0){
		g_steps_z = steps;
		NVIC_EnableIRQ(TC0_IRQn);
		tc_start(pwm.Timercounter, pwm.channel);
		TC0_Handler();
	}
	/*Interrupt PWM R1*/
	if(pwm.Timercounter == TC2 && pwm.channel == 1){
		g_steps_r1 = steps;
		NVIC_EnableIRQ(TC7_IRQn);
		tc_start(pwm.Timercounter, pwm.channel);
		TC7_Handler();

	}
	/*Interrupt PWM R2*/
	if(pwm.Timercounter == TC2 && pwm.channel == 0){
		g_steps_r2 = steps;
		NVIC_EnableIRQ(TC6_IRQn);
		tc_start(pwm.Timercounter, pwm.channel);
		TC6_Handler();
	}
}
void TC1_Handler(){
	tc_get_status(TC0, 1);
	if(flag_05_ms){
		flag_05_ms = false;
	}
	else{
		flag_05_ms = true;
	}
}

/*ISR PWM2*/
void TC0_Handler(){
	tc_get_status(TC0, 0);
	count_z++;
	if(count_z == g_steps_z){
		tc_stop(TC0,0);
		count_z = 0;
	}
}
/*ISR PWM3*/
void TC7_Handler(){
	tc_get_status(TC2, 1);
	count_r1++;
	if(count_r1 == g_steps_r1){
		tc_stop(TC2,1);
	}
}
/*ISR PWM5*/
void TC6_Handler(){
	tc_get_status(TC2, 0);
	count_r2++;
	if(count_r2 == g_steps_r2){
		tc_stop(TC2,0);
	}
}
/*ISR PWM11*/
void TC8_Handler(){
	tc_get_status(TC2,2);
	captured++;
	printf("captured: %d\r",captured);
}

/*ISR PIOD*/
void PIOD_ISR(uint32_t id, uint32_t mask)
{
	if (ID_PIOD == id && PIO_PD0 == mask)
	
	if (ID_PIOD == id && PIO_PD1 == mask){
	}
	
	if (ID_PIOD == id && PIO_PD2 == mask){
	}
}