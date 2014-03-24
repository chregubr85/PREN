/*
 * encoder.c
 *
 * Created: 17.03.2014 21:52:18
 *  Author: Christian
 */ 
#include <asf.h>
#include "Stepper_Driver.h"



volatile int8_t enc_delta;          // -128 ... 127
static int8_t last;


void encode_init( void )
{
	int8_t new;
	
	new = 0;
	if( zAchse.ENC_A )
	new = 3;
	if( zAchse.ENC_B )
	new ^= 1;                 // convert gray to binary
	last = new;                   // power on state
	enc_delta = 0;
	tc_start(TC0, 1);
}


TC1_Handler(void)
{
	TC0->TC_CHANNEL[1].TC_SR;
		

		int8_t new, diff;
		
		new = 0;
		if( pio_get_pin_value(zAchse.ENC_A) ){
		new = 3;
		}
		if( pio_get_pin_value(zAchse.ENC_B) ){
		new ^= 1;                   // convert gray to binary 
		}
		diff = last - new;                // difference last - new
		if( diff & 1 ){             // bit 0 = value (1)
			last = new;                 // store new as next last
			enc_delta += (diff & 2) - 1;        // bit 1 = direction (+/-)
		}


}


int8_t encode_read4( void )         // read four step encoders
{
	int8_t val;
	
	TC0->TC_CHANNEL[1].TC_IDR = TC_IER_CPCS;
	TC0->TC_CHANNEL[1].TC_IDR =~ TC_IDR_CPCS;
	val = enc_delta;
	enc_delta = val & 3;
	TC0->TC_CHANNEL[1].TC_IER = TC_IER_CPCS;
	TC0->TC_CHANNEL[1].TC_IER =~ TC_IDR_CPCS;
	return val >> 2;
}



