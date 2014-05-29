/*
 * Ablauf.c
 *
 * Created: 27.03.2014 15:47:12
 *  Author: Christian
 */ 

#include "PWM_TC.h"
#include "uart_buff.h"
#include "Ablauf.h"


uint32_t countKinect = 0;

/* Initialposition Anfahren */
uint32_t initialPosition(void){
	
	if(!pio_get_pin_value(INIT_R1_IDX)){
		gotoPosition(r1, -MAXVALUE_ENC_R1);	
	}
	
	if(!pio_get_pin_value(INIT_Z_IDX)){
	gotoPosition(zAchse, -MAXVALUE_ENC_Z);
	}
	delay_ms(50);
	while(active[1]){
		delay_ms(50);
	}
	if(!pio_get_pin_value(INIT_R2_IDX)){
		gotoPosition(r2, -MAXVALUE_ENC_R2);	
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
		gotoPosition(r1, 0);	
	}
	
	while(active[1]){
		delay_ms(50);
	}
	if(!pio_get_pin_value(INIT_R2_IDX & pio_get_pin_value(INIT_R1_IDX))){
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
	
	
	if(countKinect == 0 || countKinect ==3){
		gotoPosition(zAchse,KINECTPOSITION_90);
		while(active[0]) {
			delay_ms(50);
		}
			
		countKinect++;
		return true;
	}
	
	else if(countKinect == 1 || countKinect ==4){
		gotoPosition(zAchse, KINECTPOSITION_135);
		while(active[0]) {
			delay_ms(50);
		}
		
		countKinect++;
		return true;
	}	
	
	else if(countKinect == 2 || countKinect ==5){
		gotoPosition(zAchse, KINECTPOSITION_180);
		while(active[0]) {
			delay_ms(50);
		}
			if(countKinect == 2){
				countKinect++;
				return true;	
			}
			else{
				countKinect = 0;
				return true;
			}
	}
	
}

void getCube(uint32_t steps_r1, uint32_t steps_r2, uint32_t steps_z){
	
	gotoPosition(r1, steps_r1);
	
	
	gotoPosition(r2, steps_r2);	
	
	//gotoPosition(zAchse, steps_z);  TODO UNCOMMENT

	
	while(active[0] || active[1] || active[2])
	{
		delay_ms(50);
	}
	delay_s(1);			//wait after position reached
	
	//Servo in Mittelstellung
	pwm_channel_update_duty(PWM, &pwm_pin_7, 42);
	delay_ms(500);		//wait for opening Servo
	
	//Kran senken
	pio_set_pin_high(ZYLINDER_ZACHSE);
	delay_s(2);			//wait for getting down
		
	//Servo Klemmen
	pwm_channel_update_duty(PWM, &pwm_pin_7, 36);
	delay_ms(500);		//wait for closing Servo
	
	//Kran heben
	pio_set_pin_low(ZYLINDER_ZACHSE);
}


bool placeTower(void){

		gotoPosition(r1, PLACE_TOWER_R1);
		
		while(active[1]){
			delay_ms(500);
		}
		gotoPosition(r2, PLACE_TOWER_R2);
		
		gotoPosition(zAchse, PLACE_TOWER_Z);
		
		while(active[0] || active[1] || active[2])
		{
			delay_ms(50);
		}
		
		//Kran senken
		pio_set_pin_high(ZYLINDER_ZACHSE);
		delay_s(2);		
	
		//Stack öffnen
		pio_set_pin_high(ZYLINDER_STACK);
		delay_ms(500);
		
			
		//Kran heben
		pio_set_pin_low(ZYLINDER_ZACHSE);
		delay_ms(500);	
		/*gotoPosition(r1, TOWER_PLACED);
		gotoPosition(r2, TOWER_PLACED);
		
		while(active[1] || active[2]){
			delay_ms(50);
		}	
		
		return startPosition();*/
		return;
}



void ISR_INIT(uint32_t id, uint32_t mask){
	/*R1*/
	if (ID_PIOA == id && INIT_R1 == mask)
	{
		pio_set_pin_low(r1.RESET);
		active[1] = false;
		encode[1] = 0;
	}
	/*R2*/
	if (ID_PIOA == id && INIT_R2 == mask)
	{
		pio_set_pin_low(r2.RESET);
		active[2] = false;
		encode[2] = 0;
	}
	/*ZAchse*/
	if (ID_PIOA == id && INIT_Z == mask)
	{
		pio_set_pin_low(zAchse.RESET);
		active[0]=false;
		encode[0] = 0;
	}
	
}