/*
 * PWM.c
 *
 * Created: 08.02.2014 21:26:48
 *  Author: Christian
 */ 

#include "PWM_TC.h"
#include "encoder.h"
#include "Ablauf.h"




uint32_t globalEncValueZ = 0;
uint32_t globalEncValueR1 = 0;
uint32_t globalEncValueR2 = 0;
uint32_t captured = 0;


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

/*Gibt den Prescaler (MCK/2, MCK/8, MCK/32 oder MCK/128) für die gewünschte
Frequenz zurück*/
int getPrescaler(int freq){
		
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

/*
* pin: Stepper Axis
* CW: Direction, true = Clockwise
* multistep: 1 = Full-Step, 2 = Half-Step, 4 = 1/4 Step, 8 = 1/8 Step, 16 = 1/16 Step
*/
int setStepperMode( t_Stepper pin, uint32_t multistep )
{
	
	int multi = 0;

	
	switch(multistep){
		case 1:
		pio_set_pin_low(pin.M1);
		pio_set_pin_low(pin.M2);
		pio_set_pin_high(pin.M3);
		return multi = 2;
		
		case 2:
		pio_set_pin_low(pin.M1);
		pio_set_pin_high(pin.M2);
		pio_set_pin_low(pin.M3);	
		return multi = 2*2;
		
		case 4:
		pio_set_pin_high(pin.M1);
		pio_set_pin_low(pin.M2);
		pio_set_pin_low(pin.M3);
		return multi = 2*4;
		
		case 8:
		pio_set_pin_high(pin.M1);
		pio_set_pin_low(pin.M2);
		pio_set_pin_high(pin.M3);
		return multi = 2*8;
		
		case 16:
		pio_set_pin_high(pin.M1);
		pio_set_pin_high(pin.M2);
		pio_set_pin_low(pin.M3);
		return multi = 2*16;
		
		default: //Standby
		pio_set_pin_low(pin.M1);
		pio_set_pin_low(pin.M2);
		pio_set_pin_low(pin.M3);
	}
}


void timer_init( t_PinPwm pin, int freq )
{
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
	/*Wert für RC schreiben*/					
		tc_write_rc(pin.Timercounter, pin.channel, getValueRCforFreq(freq));
		tc_start(pin.Timercounter, pin.channel);	
}




void gotoPosition( t_Stepper axis, int encValue )
{
	
	encode_init(axis.pwm);
	
	/*Interrupt PWM Z-Achse*/
	if(axis.pwm.Timercounter == TC0 && axis.pwm.channel == 0){
		
		if(encode[0] > encValue){				// Direction Clockwise
			pio_set_pin_low(axis.CW_CCW);
		}
		else{									// Direction Counterclockwise
			pio_set_pin_high(axis.CW_CCW);
		}

		pio_set_pin_high(axis.RESET);
		NVIC_EnableIRQ(TC0_IRQn);
	}
		
	/*Interrupt PWM R1*/
	if(axis.pwm.Timercounter == TC2 && axis.pwm.channel == 1){
		
		if(encode[1] < encValue){				// Direction Clockwise
			pio_set_pin_low(axis.CW_CCW);
		}
		else{									// Direction Counterclockwise
			pio_set_pin_high(axis.CW_CCW);
		}
		
		pio_set_pin_high(axis.RESET);
		NVIC_EnableIRQ(TC7_IRQn);
	}
	
	/*Interrupt PWM R2*/
	if(axis.pwm.Timercounter == TC2 && axis.pwm.channel == 0){
		
		if(encode[2] > encValue){				// Direction Clockwise
			pio_set_pin_low(axis.CW_CCW);
		}
		else{									// Direction Counterclockwise
			pio_set_pin_high(axis.CW_CCW);
		}
		
		pio_set_pin_high(axis.RESET);
		NVIC_EnableIRQ(TC6_IRQn);
	}
}


/*ISR PWM2 Z-ACHSE*/
void TC0_Handler(){
	TC0->TC_CHANNEL[0].TC_SR;


	if(encode [0] >= globalEncValueZ || encode[0] >= MAXVALUE_ENC_Z){
		pio_set_pin_low(zAchse.RESET);
		active[0]=false;
		NVIC_DisableIRQ(TC0_IRQn);
		printf("Encoder z: %d\r", encode[0]);
	}
}



/*ISR PWM3 R1*/
void TC7_Handler(){
	TC2->TC_CHANNEL[1].TC_SR;


	if(encode[1] >= globalEncValueR1 || encode[1] >= MAXVALUE_ENC_R1){
		pio_set_pin_low(r1.RESET);
		active[1] = false;
		NVIC_DisableIRQ(TC7_IRQn);
		printf("CLK INTER: Encoder r1: %d\r", encode[1]);
	}
}

/*ISR PWM5 R2*/
void TC6_Handler(){
	TC2->TC_CHANNEL[0].TC_SR;


	
	if(encode[2] >= globalEncValueR2 || encode[2] >= MAXVALUE_ENC_R2){
		pio_set_pin_low(r2.RESET);
		active[2] = false;
		NVIC_DisableIRQ(TC6_IRQn);
		printf("CLK INTER: Encoder r2: %d\r", encode[2]);
	}
}

/*ISR PWM11*/
void TC8_Handler(){
	TC2->TC_CHANNEL[2].TC_SR;
		encode[0] += encode_zAchse_read4();
		encode[1] += encode_r1_read4();
		encode[2] += encode_r2_read4();
}
