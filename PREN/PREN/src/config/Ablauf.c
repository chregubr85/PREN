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
	
	if(!pio_get_pin_value(INIT_R1_IDX)){
		gotoPosition(r1, 0);	
	}
	
	if(!pio_get_pin_value(INIT_Z_IDX)){
	gotoPosition(zAchse, 0);
	}
	delay_ms(50);
	while(active[1]){
		delay_ms(50);
	}
	if(!pio_get_pin_value(INIT_R2_IDX)){
		gotoPosition(r2, 0);	
	}
	
	while(active[0] || active[1] ||active[2])
	{
	delay_ms(50);	
	}
	
	delay_s(1);
	
	return UART_INIT_OK;
		
}


/* Grundposition Anfahren (Startfeld) */
bool startPosition(void){
	
	if(!pio_get_pin_value(INIT_R1_IDX))
	{
		pio_enable_interrupt(PIOA, INIT_R1_IDX);
		gotoPosition(r1, 0);	
	}
	if(!pio_get_pin_value(INIT_R2_IDX) & pio_get_pin_value(INIT_R1_IDX))
	{
		pio_enable_interrupt(PIOA, INIT_R2_IDX);
		gotoPosition(r2, 0);
	}	
	
	gotoPosition(zAchse, STARTPOSITION_Z);	
	
	while(active[0] ||  active[1] || active[2])
	{
		delay_ms(50);
	}
	
	return true;
	
}

/* Spielfeld abfahren für Kinect */
bool gotoPositonKinect(void){
	
	initialPosition();
	

	gotoPosition(zAchse, (encode[0]+KINECTPOSITION));
	
	while(active[0])
	{
		delay_ms(50);
	}
	
	return true;
	
}

void getCube(uint32_t steps_r1, uint32_t steps_r2, uint32_t steps_z){
	
	if(steps_r1 < encode[1])
	{
		gotoPosition(r1, steps_r1);
	}
	else
	gotoPosition(r1, steps_r1);
	
	if(steps_r2 < encode[2])
	{
		gotoPosition(r2, steps_r2);
	}
	else
	gotoPosition(r2, steps_r2);	
	
	if(steps_z < encode[0])
	{
		gotoPosition(zAchse, steps_z);
	}
	else
	gotoPosition(zAchse, steps_z);
	
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
			gotoPosition(r1, PLACE_TOWER_R1);
		}
		else
		gotoPosition(r1, PLACE_TOWER_R1);
		
		if(PLACE_TOWER_R2 < encode[2])
		{
			gotoPosition(r2, PLACE_TOWER_R2);
		}
		else
		gotoPosition(r2, PLACE_TOWER_R2);
		
		if(PLACE_TOWER_Z < encode[0])
		{
			gotoPosition(zAchse, PLACE_TOWER_Z);
		}
		else
		gotoPosition(zAchse, PLACE_TOWER_Z);
		
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
		gotoPosition(r1, TOWER_PLACED);
		gotoPosition(r2, TOWER_PLACED);
		
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
		active[1] = false;
		encode[1] = 0;
		
		printf("INIT INTER: Encoder r1: %d\r", encode[1]);
	}
	/*R2*/
	if (ID_PIOA == id && INIT_R2 == mask)
	{
		printf("R2\r");
		pio_set_pin_low(r2.RESET);
		active[2] = false;
		encode[2] = 0;
		
		printf("INIT INTER: Encoder r2: %d\r", encode[2]);
	}
	/*ZAchse*/
	if (ID_PIOA == id && INIT_Z == mask)
	{
		printf("Z\r");
		pio_set_pin_low(zAchse.RESET);
		active[0]=false;
		encode[0] = 0;
	}
	
}