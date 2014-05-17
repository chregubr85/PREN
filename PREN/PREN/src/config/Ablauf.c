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
	pio_enable_interrupt(PIOA, INIT_Z);		//Z
	
	if(!pio_get_pin_value(INIT_R1_IDX)){
		numberOfSteps(r1, MAXSTEPS, COUNTERCLOCKWISE);	
	}
	
	if(!pio_get_pin_value(INIT_Z_IDX)){
	numberOfSteps(zAchse, MAXSTEPS, CLOCKWISE);
	}
	delay_ms(50);
	while(active[1]){
		delay_ms(50);
	}
	if(!pio_get_pin_value(INIT_R2_IDX)){
		numberOfSteps(r2, MAXSTEPS, CLOCKWISE);	
	}
	
	while(active[0] || active[1] ||active[2])
	{
	delay_ms(50);	
	}
	
	delay_s(1);
	
	startPositionOK = startPosition();
	
	if(startPositionOK){
		return UART_INIT_OK;
	}
}


/* Grundposition Anfahren (Startfeld) */
bool startPosition(void){
	
	if(!pio_get_pin_value(INIT_R1_IDX))
	{
		pio_enable_interrupt(PIOA, INIT_R1_IDX);
		numberOfSteps(r1, MAXSTEPS, COUNTERCLOCKWISE);	
	}
	if(!pio_get_pin_value(INIT_R2_IDX) & pio_get_pin_value(INIT_R1_IDX))
	{
		pio_enable_interrupt(PIOA, INIT_R2_IDX);
		numberOfSteps(r2, MAXSTEPS, COUNTERCLOCKWISE);
	}	
	
	numberOfSteps(zAchse, STARTPOSITION_Z, COUNTERCLOCKWISE);	
	
	while(active[0] ||  active[1] || active[2])
	{
		delay_ms(50);
	}
	
	return true;
	
}

/* Spielfeld abfahren für Kinect */
bool gotoPositonKinect(void){
	
	//Enable Interrupts
	pio_enable_interrupt(PIOA, INIT_Z_IDX);	//Z
	

	numberOfSteps(zAchse, KINECTPOSITION, CLOCKWISE);
	
	/*while(active[0])
	{
		delay_ms(50);
	}*/
	
		return true;
	
}

void getCube(uint32_t steps_r1, uint32_t steps_r2, uint32_t steps_z){
	
	if(steps_r1 < encode[1])
	{
		numberOfSteps(r1, steps_r1, COUNTERCLOCKWISE);
	}
	else
	numberOfSteps(r1, steps_r1, CLOCKWISE);
	
	if(steps_r2 < encode[2])
	{
		numberOfSteps(r2, steps_r2, COUNTERCLOCKWISE);
	}
	else
	numberOfSteps(r2, steps_r2, CLOCKWISE);	
	
	if(steps_z < encode[0])
	{
		numberOfSteps(zAchse, steps_z, COUNTERCLOCKWISE);
	}
	else
	numberOfSteps(zAchse, steps_z, CLOCKWISE);
	
	while(active[0] || active[1] || active[2])
	{
		delay_ms(50);
	}
	
	//Servo in Mittelstellung
	pwm_channel_update_duty(PWM, &pwm_pin_7, 36);
	
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
			numberOfSteps(r1, PLACE_TOWER_R1, COUNTERCLOCKWISE);
		}
		else
		numberOfSteps(r1, PLACE_TOWER_R1, CLOCKWISE);
		
		if(PLACE_TOWER_R2 < encode[2])
		{
			numberOfSteps(r2, PLACE_TOWER_R2, COUNTERCLOCKWISE);
		}
		else
		numberOfSteps(r2, PLACE_TOWER_R2, CLOCKWISE);
		
		if(PLACE_TOWER_Z < encode[0])
		{
			numberOfSteps(zAchse, PLACE_TOWER_Z, COUNTERCLOCKWISE);
		}
		else
		numberOfSteps(zAchse, PLACE_TOWER_Z, CLOCKWISE);
		
		while(active[0] || active[1] || active[2])
		{
			delay_ms(50);
		}
		
		//Kran senken
		pio_set_pin_high(ZYLINDER_ZACHSE);
		delay_ms(500);		
	
		//Stack öffnen
		pio_set_pin_high(ZYLINDER_STACK);
		delay_ms(500);
		
			
		//Kran heben
		pio_set_pin_low(ZYLINDER_ZACHSE);
		delay_ms(500);	
		numberOfSteps(r1, TOWER_PLACED, COUNTERCLOCKWISE);
		numberOfSteps(r2, TOWER_PLACED, COUNTERCLOCKWISE);
		
		while(active[1] || active[2]){
			delay_ms(500);
		}	
		
		return startPosition();
}



void ISR_INIT(uint32_t id, uint32_t mask){
	/*R1*/
	if (ID_PIOA == id && INIT_R1 == mask)
	{
		printf("R1\r");
		pio_set_pin_low(r1.RESET);
		NVIC_DisableIRQ(TC7_IRQn);
		active[1] = false;
		encode[1] = 0;
		//pio_disable_interrupt(PIOA, INIT_R1);
		
		printf("INIT INTER: Encoder r1: %d\r", encode[1]);
	}
	/*R2*/
	if (ID_PIOA == id && INIT_R2 == mask)
	{
		printf("R2\r");
		pio_set_pin_low(r2.RESET);
		NVIC_DisableIRQ(TC6_IRQn);
		active[2] = false;
		encode[2] = 0;
		//pio_disable_interrupt(PIOA, INIT_R2);
		
		printf("INIT INTER: Encoder r2: %d\r", encode[2]);
	}
	/*ZAchse*/
	if (ID_PIOA == id && INIT_Z == mask)
	{
		printf("Z\r");
		pio_set_pin_low(zAchse.RESET);
		NVIC_DisableIRQ(TC0_IRQn);
		active[0]=false;
		encode[0] = 0;
	//	pio_disable_interrupt(PIOA, INIT_Z);
	}
	
}