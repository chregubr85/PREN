/*
 * Ablauf.c
 *
 * Created: 27.03.2014 15:47:12
 *  Author: Christian
 */ 

#include "PWM_TC.h"
#include "uart_buff.h"
#include "Ablauf.h"


/* Initialposition Anfahren */
uint32_t initialPosition(void){
	
	bool startPositionOK = false;
	
	//Enable Interrupts
	pio_enable_interrupt(PIOA, INIT_R1);	//R1
	pio_enable_interrupt(PIOA, INIT_R2);	//R2
	pio_enable_interrupt(PIOA, INIT_Z);	//Z
	
	numberOfSteps(r1, MAXSTEPS, FULLSTEP, COUNTERCLOCKWISE);				
	numberOfSteps(r2, MAXSTEPS, FULLSTEP, COUNTERCLOCKWISE);				
	numberOfSteps(zAchse, MAXSTEPS, FULLSTEP, COUNTERCLOCKWISE);
	
	while(active[0] ||  active[1] || active[2])
	{
	delay_ms(50);	
	}
	
	startPositionOK = startPosition();
	
	if(startPositionOK){
		return UART_INIT_OK;
	}
}


/* Grundposition Anfahren (Startfeld) */
bool startPosition(void){
	
	if(!pio_get_pin_value(INIT_R1))
	{
		pio_enable_interrupt(PIOA, INIT_R1);
		numberOfSteps(r1, MAXSTEPS, FULLSTEP, COUNTERCLOCKWISE);	
	}
	if(!pio_get_pin_value(INIT_R2))
	{
		pio_enable_interrupt(PIOA, INIT_R2);
		numberOfSteps(r2, MAXSTEPS, FULLSTEP, COUNTERCLOCKWISE);
	}	
	
	numberOfSteps(zAchse, STARTPOSITION_Z, FULLSTEP, CLOCKWISE);	
	
	while(active[0] ||  active[1] || active[2])
	{
		delay_ms(50);
	}
	
	return true;
	
}

/* Spielfeld abfahren für Kinect */
bool gotoPositonKinect(void){
	
	//Enable Interrupts
	pio_enable_interrupt(PIOA, INIT_Z);	//Z
	

	numberOfSteps(zAchse, KINECTPOSITION, FULLSTEP, CLOCKWISE);
	
	while(active[0])
	{
		delay_ms(50);
	}
	
		return true;
	
}

void getCube(uint32_t steps_r1, uint32_t steps_r2, uint32_t steps_z){
	
	if(steps_r1 < encode[1])
	{
		numberOfSteps(r1, steps_r1, FULLSTEP, COUNTERCLOCKWISE);
	}
	else
	numberOfSteps(r1, steps_r1, FULLSTEP, CLOCKWISE);
	
	if(steps_r2 < encode[2])
	{
		numberOfSteps(r2, steps_r2, FULLSTEP, COUNTERCLOCKWISE);
	}
	else
	numberOfSteps(r2, steps_r2, FULLSTEP, CLOCKWISE);	
	
	if(steps_z < encode[0])
	{
		numberOfSteps(zAchse, steps_z, FULLSTEP, COUNTERCLOCKWISE);
	}
	else
	numberOfSteps(zAchse, steps_z, FULLSTEP, CLOCKWISE);
	
	while(active[0] || active[1] || active[2])
	{
		delay_ms(50);
	}
	
	//Servo in Mittelstellung
	pwm_channel_update_duty(PWM, &pwm_pin_7, 30);
	
	//Kran senken
	pio_set_pin_high(ZYLINDER_ZACHSE);
	delay_ms(500);
	
	//Servo Klemmen
	pwm_channel_update_duty(PWM, &pwm_pin_7, 42);	
	
	//Kran heben
	pio_set_pin_low(ZYLINDER_ZACHSE);
	delay_ms(500);
}


bool placeTower(void){
		if(PLACE_TOWER_R1 < encode[1])
		{
			numberOfSteps(r1, PLACE_TOWER_R1, FULLSTEP, COUNTERCLOCKWISE);
		}
		else
		numberOfSteps(r1, PLACE_TOWER_R1, FULLSTEP, CLOCKWISE);
		
		if(PLACE_TOWER_R2 < encode[2])
		{
			numberOfSteps(r2, PLACE_TOWER_R2, FULLSTEP, COUNTERCLOCKWISE);
		}
		else
		numberOfSteps(r2, PLACE_TOWER_R2, FULLSTEP, CLOCKWISE);
		
		if(PLACE_TOWER_Z < encode[0])
		{
			numberOfSteps(zAchse, PLACE_TOWER_Z, FULLSTEP, COUNTERCLOCKWISE);
		}
		else
		numberOfSteps(zAchse, PLACE_TOWER_Z, FULLSTEP, CLOCKWISE);
		
		while(active[0] || active[1] || active[2])
		{
			delay_ms(50);
		}
		
		//Kran senken
		pio_set_pin_high(ZYLINDER_ZACHSE);
		delay_ms(500);		
	
		//Stack öffnen
		//TODO
			
		//Kran heben
		pio_set_pin_low(ZYLINDER_ZACHSE);
		delay_ms(500);	
		
		return startPosition();
}



void ISR_INIT(uint32_t id, uint32_t mask){
	/*R1*/
	if (ID_PIOA == id && INIT_R1 == mask)
	{
		tc_stop(TC2, 1);
		active[1] = false;
		pio_configure(PIOC, PIO_INPUT, PIO_PC28, PIO_DEFAULT);
		encode[1] = 0;
		pio_disable_interrupt(PIOA, INIT_R1);
	}
	/*R2*/
	if (ID_PIOA == id && INIT_R2 == mask)
	{
		tc_stop(TC2, 0);
		active[2] = false;
		pio_configure(PIOC, PIO_INPUT, PIO_PC25, PIO_DEFAULT);
		encode[1] = 0;
		pio_disable_interrupt(PIOA, INIT_R2);
	}
	/*ZAchse*/
	if (ID_PIOA == id && INIT_Z == mask)
	{
		tc_stop(TC0, 0);
		active[0]=false;
		pio_configure(PIOB, PIO_INPUT, PIO_PB25, PIO_DEFAULT);
		encode[0] = 0;
		pio_disable_interrupt(PIOA, INIT_Z);
	}	
	
}