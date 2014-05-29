/*
 * encoder.c
 *
 * Created: 17.03.2014 21:52:18
 *  Author: Christian
 */ 

#include "PWM_TC.h"
#include "Ablauf.h"

int32_t enc = 0;

volatile int8_t enc_delta_z, enc_delta_r1, enc_delta_r2;          // -128 ... 127
static int8_t last_z, last_r1, last_r2;


void encode_init( t_PinPwm pwm )
{
	int8_t new_z, new_r1, new_r2 = 0;
	
	if(pwm.pin_id == PIO_PB25_IDX) //Aufruf von ZAchse
	{
		active[0] = true;
	}
	if(pwm.pin_id == PIO_PC28_IDX) //Aufruf von R1
	{
		active[1] = true;
	}
	if(pwm.pin_id == PIO_PC25_IDX) //Aufruf von R2
	{
		active[2] = true;
	}		
	
	//ZAchse:
	if( pio_get_pin_value(zAchse.ENC_B) )
	new_z = 3;
	if( pio_get_pin_value(zAchse.ENC_A) )
	new_z ^= 1;							 // convert gray to binary
	last_z = new_z;						 // power on state
	enc_delta_z = 0;
	
	//R1
	if( pio_get_pin_value(r1.ENC_A) )
	new_r1 = 3;
	if( pio_get_pin_value(r1.ENC_B) )
	new_r1 ^= 1;					   // convert gray to binary
	last_r1 = new_r1;                   // power on state
	enc_delta_r1 = 0;
	
	//R2
	if( pio_get_pin_value(r2.ENC_A) )
	new_r1 = 3;
	if( pio_get_pin_value(r2.ENC_B) )
	new_r2 ^= 1;					   // convert gray to binary
	last_r2 = new_r2;                   // power on state
	enc_delta_r2 = 0;	
	
	tc_start(TC0, 1);
}


TC1_Handler(void)
{
	TC0->TC_CHANNEL[1].TC_SR;
		

		int8_t new_z = 0, new_r1 = 0, new_r2 = 0,  diff_z, diff_r1, diff_r2;


		//ZAchse
		if( pio_get_pin_value(zAchse.ENC_B) ){
		new_z = 3;
		}
		if( pio_get_pin_value(zAchse.ENC_A) ){
		new_z ^= 1;								// convert gray to binary 
		}
		diff_z = last_z - new_z;				// difference last - new
		if( diff_z & 1 ){						// bit 0 = value (1)
			last_z = new_z;						// store new as next last
			enc_delta_z += (diff_z & 2) - 1;	// bit 1 = direction (+/-)
		}
		
		//R1
		if( pio_get_pin_value(r1.ENC_A) ){
			new_r1 = 3;
		}
		if( pio_get_pin_value(r1.ENC_B) ){
			new_r1 ^= 1;							// convert gray to binary
		}
		diff_r1 = last_r1 - new_r1;					// difference last - new
		if( diff_r1 & 1 ){							// bit 0 = value (1)
			last_r1 = new_r1;						// store new as next last
			enc_delta_r1 += (diff_r1 & 2) - 1;		// bit 1 = direction (+/-)
		}		
		
		//R2
		if( pio_get_pin_value(r2.ENC_A) ){
			new_r2 = 3;
		}
		if( pio_get_pin_value(r2.ENC_B) ){
			new_r2 ^= 1;							// convert gray to binary
		}
		diff_r2 = last_r2 - new_r2;					// difference last - new
		if( diff_r2 & 1 ){							// bit 0 = value (1)
			last_r2 = new_r2;						// store new as next last
			enc_delta_r2 += (diff_r2 & 2) - 1;		// bit 1 = direction (+/-)
		}		
		
		
		
		//
		// Stop Steppers
		//
		if(encode [0] == globalEncValueZ || encode[0] >= MAXVALUE_ENC_Z){
			pio_set_pin_low(zAchse.RESET);
			active[0]=false;
		}
		
		if(encode[1] == globalEncValueR1 || encode[1] >= MAXVALUE_ENC_R1){
			pio_set_pin_low(r1.RESET);
			active[1] = false;
		}
	
		if(encode[2] == globalEncValueR2 || encode[2] >= MAXVALUE_ENC_R2){
			pio_set_pin_low(r2.RESET);
			active[2] = false;
		}
		
			
}


int8_t encode_zAchse_read4(void)					// read four step encoders
{
	int8_t val;

	TC0->TC_CHANNEL[1].TC_IDR = TC_IER_CPCS;
	TC0->TC_CHANNEL[1].TC_IDR =~ TC_IDR_CPCS;
	
	//ZAchse
	val = enc_delta_z;
	enc_delta_z = val & 3;
	
	TC0->TC_CHANNEL[1].TC_IER = TC_IER_CPCS;
	TC0->TC_CHANNEL[1].TC_IER =~ TC_IDR_CPCS;
	
	return val >> 2;

}

int8_t encode_r1_read4(void)						// read four step encoders
{
	int8_t val;
	
	TC0->TC_CHANNEL[1].TC_IDR = TC_IER_CPCS;
	TC0->TC_CHANNEL[1].TC_IDR =~ TC_IDR_CPCS;
	
	//R1
	val = enc_delta_r1;
	enc_delta_r1 = val & 3;
	
	TC0->TC_CHANNEL[1].TC_IER = TC_IER_CPCS;
	TC0->TC_CHANNEL[1].TC_IER =~ TC_IDR_CPCS;

	return val >> 2;
}

int8_t encode_r2_read4(void)						// read four step encoders
{
	int8_t val;
	
	TC0->TC_CHANNEL[1].TC_IDR = TC_IER_CPCS;
	TC0->TC_CHANNEL[1].TC_IDR =~ TC_IDR_CPCS;
	
	//R2
	val = enc_delta_r2;
	enc_delta_r2 = val & 3;
	
	TC0->TC_CHANNEL[1].TC_IER = TC_IER_CPCS;
	TC0->TC_CHANNEL[1].TC_IER =~ TC_IDR_CPCS;

	return val >> 2;
}

