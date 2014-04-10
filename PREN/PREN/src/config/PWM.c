/*
 * PWM.c
 *
 * Created: 08.02.2014 21:26:48
 *  Author: Christian
 */ 

#include "PWM_TC.h"
#include "encoder.h"


int NORM_FREQ_R1_R2 = 300;
int NORM_FREQ_Z		= 700;

uint32_t g_steps_z = 0;
uint32_t g_steps_r1 = 0;
uint32_t g_steps_r2 = 0;
uint32_t captured = 0;
uint32_t count_z = 0;



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

/*
* pin: Stepper Axis
* CW: Direction, true = Clockwise
* multistep: 1 = Full-Step, 2 = Half-Step, 4 = 1/4 Step, 8 = 1/8 Step, 16 = 1/16 Step
*/
int setStepperMode(t_Stepper pin, bool CW, uint32_t multistep){
	
	int multi = 0;
	
	
	if(CW){
		pio_set_pin_low(pin.CW_CCW);	
	}
	else
		pio_set_pin_high(pin.CW_CCW);
		
	
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
}



/*
* axis: Stepper Axis
* steps: # of Steps
* mode: Full-, Half.... Steps
* CW : True = CW
*/
void numberOfSteps( t_Stepper axis, int steps, uint32_t mode, bool CW )
{
	int multi = setStepperMode(axis, CW, mode);
	
	//ZAchse
	if(axis.pwm.pin_id == PIO_PB25_IDX){ 
		encode_init(axis.pwm);
		pio_set_pin_high(axis.RESET);
		pio_set_pin_high(axis.ENBLE);		
		timer_init(axis.pwm, (NORM_FREQ_Z * multi));
	}
	//R1 und R2
	else{
		encode_init(axis.pwm);
		pio_set_pin_high(axis.RESET);
		pio_set_pin_high(axis.ENBLE);		
		timer_init(axis.pwm, (NORM_FREQ_R1_R2*multi));	
	}
		/*Interrupt PWM Z-Achse*/////axis.pwm.Timercounter == TC0 && axis.pwm.channel == 0
		if(axis.pwm.pin_id == PIO_PB25_IDX){
			g_steps_z = 2 * steps;
			encode[0] = 3;
			NVIC_EnableIRQ(TC0_IRQn);
			tc_start(axis.pwm.Timercounter, axis.pwm.channel);
		}
		/*Interrupt PWM R1*/
		if(axis.pwm.Timercounter == TC2 && axis.pwm.channel == 1){
			g_steps_r1 = 2 * steps;
			encode[1] = 0;
			NVIC_EnableIRQ(TC7_IRQn);
			tc_start(axis.pwm.Timercounter, axis.pwm.channel);
		}
		/*Interrupt PWM R2*/
		if(axis.pwm.Timercounter == TC2 && axis.pwm.channel == 0){
			g_steps_r2 = 2 * steps;
			encode[2]=0;
			NVIC_EnableIRQ(TC6_IRQn);
			tc_start(r2.pwm.Timercounter, r2.pwm.channel);

		}
	
}


/*ISR PWM2 Z-ACHSE*/
void TC0_Handler(){
	TC0->TC_CHANNEL[0].TC_SR;
	encode[0] += encode_zAchse_read4();
	count_z++;
	
	
	if(encode [0] == g_steps_z){
		tc_stop(TC0, 0);
		active[0]=false;
		pio_configure(PIOB, PIO_INPUT, PIO_PB25, PIO_DEFAULT);
	//	printf("Encoder z: %d\r", encode[0]);
	}
}



/*ISR PWM3 R1*/
void TC7_Handler(){
	TC2->TC_CHANNEL[1].TC_SR;
	encode[1] += encode_r1_read4();
	
	if(Abs(encode[1]) == g_steps_r1){
		tc_stop(TC2, 1);
		active[1] = false;
		pio_configure(PIOC, PIO_INPUT, PIO_PC28, PIO_DEFAULT);
	//	printf("Encoder r1: %d\r", encode[1]);
	}
}

/*ISR PWM5 R2*/
void TC6_Handler(){
	TC2->TC_CHANNEL[0].TC_SR;
	encode[2] += encode_r2_read4();
				printf("Steps: %d\r", g_steps_r2);
	
	if(Abs(encode[2]) == g_steps_r2){
		tc_stop(TC2, 0);
		active[2] = false;
		pio_configure(PIOC, PIO_INPUT, PIO_PC25, PIO_DEFAULT);
	//	printf("Encoder r2: %d\r", encode[2]);
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