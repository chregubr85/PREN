/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */


#include "PWM_TC.h"
#include "uart_buff.h"
#include "encoder.h"
#include "Ablauf.h"

//Variablen
unsigned int key = 0;
uint32_t cubePositions [3][6] ;
bool startPositionOk = false;
uint32_t init_ok = 0;
bool uartOK = false;



static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.paritytype = CONF_UART_PARITY,
		.baudrate = CONF_UART_BAUDRATE,
		.stopbits = false
	};
	
	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
	
	uart_disable_interrupt(CONSOLE_UART, 0xffffffff);
	uart_enable_interrupt(CONSOLE_UART, 0x00000101);
	NVIC_EnableIRQ(UART_IRQn);
	
	uart_enable_tx(CONSOLE_UART);
	
	uart_buff_init();
}




int main (void)
{
	
	uint32_t steps;
	uint32_t encValue = 0;
	uint32_t countNrCubes = 0;
	
	configure_console();
	board_init();
	sysclk_init();
	
/*
	puts("--i Zylinder Z aus\r--o Zyliner Z ein\r\r--k Zylinder Stack aus\r--l Zylinder Stack ein\r\r--a Z CCW\r--s Z CW\r\r--d R1 CCW\r--f R1 CW\r\r--g R2 CCW\r--h R2 CW\r\r--x Servo öffnen\r--c Servo in Mittelstellung\r\r--n Initial Position anfahren\r--m Start Position anfahren\r\r--y Alle Motoren aus\r");


	while(true){
		
		// Timer für Encoder stoppen wenn keine Achse aktiv ist //
	if(active[0] == false && active[1] == false && active[2] == false){
		tc_stop(TC0, 1);
	}

	key = uart_getc();
	 	if (key & UART_NO_DATA )
	        {
	             //no data available from UART
			}
	 	else
 		{			
		switch(key){
			case 'i':
			pio_set_pin_low(ZYLINDER_ZACHSE);
			break;
			
			case 'o':
			pio_set_pin_high(ZYLINDER_ZACHSE);
			break;

			case 'k':
			pio_set_pin_low(ZYLINDER_STACK);
			break;
			
			case 'l':
			pio_set_pin_high(ZYLINDER_STACK);
			break;
					
			case 'a':	//Z CCW			
			printf("Anzahl Schritte: \r");
			steps = get_input_value();
			encValue = encode[0] + (5*steps);
			printf("\rFahre %d Schritte.\r", encValue);
			gotoPosition(zAchse, encValue);
			break;	
			
			case 's':	//z CW
			printf("Anzahl Schritte: \r");
			steps = get_input_value();
			encValue = encode[0] - (5*steps);
			printf("\rFahre %d Schritte.\r", encValue);
			gotoPosition(zAchse, encValue);
			break;			
		
			case 'd':	//R1 CCW
			printf("Anzahl Schritte: \r");
			steps = get_input_value();
			encValue = encode[1] - (4*steps);
			printf("\rFahre %d Schritte.\r", encValue);
			gotoPosition(r1, encValue);
			break;
		
			case 'f':	//R1 CW
			printf("Anzahl Schritte: \r");
			steps = get_input_value();
			encValue = encode[1] + (4*steps);
			printf("\rFahre %d Schritte.\r", encValue);
			gotoPosition(r1, encValue);
			break;
			
			case 'g':	//R2 CCW
			printf("Anzahl Schritte: \r");
			steps = get_input_value();
			encValue = encode[2] + (5*steps);
			printf("\rFahre %d Schritte.\r", encValue);
			gotoPosition(r2, encValue);
			break;
			
			case 'h':	//R2 CW
			printf("Anzahl Schritte: \r");
			steps = get_input_value();
			encValue = encode[2] - (5*steps);
			printf("\rFahre %d Schritte.\r", encValue);
			gotoPosition(r2, encValue);
			break;		
			
			case 'x':
			pwm_channel_update_duty(PWM, &pwm_pin_7, 42);
			break;
			
			case 'c':
			pwm_channel_update_duty(PWM, &pwm_pin_7, 36);
			break;	
			
			case 'q':
			printf("/DEBUG MESSAGE FROM ARDUINO TO PC\r");	
			break;
			
			case 'y':
			//Z
			pio_set_pin_low(zAchse.RESET);
			active[0]=false;
			NVIC_DisableIRQ(TC0_IRQn);
			printf("Encoder Z: %d ", encode[0]);
			//R1
			pio_set_pin_low(r1.RESET);
			active[1] = false;
			NVIC_DisableIRQ(TC7_IRQn);
			printf("Encoder R1: %d ", encode[1]);
			//R2
			pio_set_pin_low(r2.RESET);
			active[2] = false;
			NVIC_DisableIRQ(TC6_IRQn);
			printf("Encoder R2: %d\r", encode[2]);
		
			break;
			
			case 'n':
			initialPosition();
			break;
			
			case 'm':
			startPosition();
			break;
			
			case',':
			printf("Encode Z: %d\r", encode[0]);
			printf("Encoder R1: %d\r", encode[1]);
			printf("Encoder R2: %d\r", encode[2]);
			break;
			
			case'.':
			getCube(800, 800, 7800);
			break;
			
			case'-':
			placeTower();
			break;
			
			default:
			printf("%d is not used! \r\n", key);
			}
		 }
*/




	while(true){

	key = uart_getc();	
	if (key & UART_NO_DATA )
        {
            //no data available from UART 
		}
	else
	{
		switch(key){

			case 0x00: //Hello
				uart_get_data();			// cleaer the buffer
				uart_send(UART_OK_32);
			break;
			
			case 0x01: // Initialisieren  
				uart_get_data();			// cleaer the buffer
				
				init_ok = initialPosition(); 
				if(init_ok){
					init_ok = startPosition();
				}
				delay_s(1);
				uart_send(UART_INIT_OK);
			break;
			
			case 0x02: // Würfel 1 R1  
				delay_ms(100);
				cubePositions[0][0] =	4*uart_get_data();
				uart_send(cubePositions[0][0]);
				
			break;
			
			case 0x03: // Würfel 1 R2  
				delay_ms(100);
				cubePositions[1][0] =	5*uart_get_data();
				uart_send(cubePositions[1][0]);	
				
			break;
			
			case 0x04: // Würfel 1 Phi  
				delay_ms(100);
				cubePositions[2][0] =	5*uart_get_data();
				countNrCubes++;
				uart_send(cubePositions[2][0]);		
					
			break;
			
			case 0x05: // Würfel 2 R1  
				delay_ms(100);
				cubePositions[0][1] =	4*uart_get_data();
				uart_send(cubePositions[0][1]);			
			break;
			
			case 0x06: // Würfel 2 R2  
				delay_ms(100);
				cubePositions[1][1] =	5*uart_get_data();
				uart_send(cubePositions[1][1]);			
			break;
			
			case 0x07: // Würfel 2 Phi 
				delay_ms(100); 
				cubePositions[2][1] =	5*uart_get_data();
				countNrCubes++;
				uart_send(cubePositions[2][1]);			
			break;
			
			case 0x08: // Würfel 3 R1  
				delay_ms(100);
				cubePositions[0][2] =	4*uart_get_data();
				uart_send(cubePositions[0][2]);			
			break;
			
			case 0x09: // Würfel 3 R2  
				delay_ms(100);
				cubePositions[1][2] =	5*uart_get_data();
				uart_send(cubePositions[1][2]);			
			break;
			
			case 0x0A: // Würfel 3 Phi
				delay_ms(100);  
				cubePositions[2][2] =	5*uart_get_data();
				countNrCubes++;
				uart_send(cubePositions[2][2]);			
			break;
			
			case 0x0B: // Würfel 4 R1  
				delay_ms(100);
				cubePositions[0][3] =	4*uart_get_data();
				uart_send(cubePositions[0][3]);			
			break;
			
			case 0x0C: // Würfel 4 R2  
				delay_ms(100);
				cubePositions[1][3] =	5*uart_get_data();
				uart_send(cubePositions[1][3]);			
			break;
			
			case 0x0D: // Würfel 4 Phi  
				delay_ms(100);
				cubePositions[2][3] =	5*uart_get_data();
				countNrCubes++;
				uart_send(cubePositions[2][3]);			
			break;
			
			case 0x0E: // Würfel 5 R1  
				delay_ms(100);
				cubePositions[0][4] =	4*uart_get_data();
				uart_send(cubePositions[0][4]);			
			break;
			
			case 0x0F: // Würfel 5 R2  
				delay_ms(100);
				cubePositions[1][4] =	5*uart_get_data();
				uart_send(cubePositions[1][4]);			
			break;
			
			case 0x10: // Würfel 5 Phi  
				delay_ms(100);
				cubePositions[2][4] =	5*uart_get_data();
				countNrCubes++;
				uart_send(cubePositions[2][4]);			
			break;
			
			case 0x11: // Würfel 6 R1  
				delay_ms(100);
				cubePositions[0][5] =	4*uart_get_data();
				uart_send(cubePositions[0][5]);			
			break;
			
			case 0x12: // Würfel 6 R2  
				delay_ms(100);
				cubePositions[1][5] =	5*uart_get_data();
				uart_send(cubePositions[1][5]);			
			break;
			
			case 0x13: // Würfel 6 Phy  
				delay_ms(100);
				cubePositions[2][5] =	5*uart_get_data();
				countNrCubes++;
				uart_send(cubePositions[2][5]);			
			break;
			
			case 0x14: // Alle Daten erhalten 
				 
				//Würfel 1
				if(countNrCubes > 0){
					getCube(cubePositions[0][0], cubePositions[1][0], cubePositions[2][0]);
						while(active[0] || active[1] || active[2])
						{
							delay_ms(50);
						}
				}
				
				//Würfel 2
				if(countNrCubes > 1){
					getCube(cubePositions[0][1], cubePositions[1][1], cubePositions[2][1]);		
					while(active[0] || active[1] || active[2])
						{
							delay_ms(50);
						}		
				}
	
									
				if(countNrCubes > 2){	
					//Würfel 3
					getCube(cubePositions[0][2], cubePositions[1][2], cubePositions[2][2]);	
						while(active[0] || active[1] || active[2])
						{
							delay_ms(50);
						}		
						}
					
				if(countNrCubes > 3){	
					//Würfel 4
					getCube(cubePositions[0][3], cubePositions[1][3], cubePositions[2][3]);	
						while(active[0] || active[1] || active[2])
						{
							delay_ms(50);
						}			
					}
					
				if(cubePositions > 4){	
					//Würfel 5
					getCube(cubePositions[0][4], cubePositions[1][4], cubePositions[2][4]);	
						while(active[0] || active[1] || active[2])
						{
							delay_ms(50);
						}			
					}
					
				if(countNrCubes > 5){	
					//Würfel 6
					getCube(cubePositions[0][5], cubePositions[1][5], cubePositions[2][5]);				
						while(active[0] || active[1] || active[2])
						{
							delay_ms(50);
						}
				}
		
				startPositionOk = placeTower();
				
				if(startPositionOk){
					uart_send(UART_OK_32);
				}
				else
					uart_send(UART_ERROR);
			break;
			
			case 0x1e://Spielfeld abfahren Kinect
				uartOK = gotoPositonKinect();
				if(uartOK){
					uart_send(UART_OK_32);
				}
				else
					uart_send(UART_ERROR);
			break;
			
			case 0x1f: //Turm stellen erzwingen 
				while(active[0] == true || active[1] == true || active[2] == true){
					delay_ms(500);
				}
				placeTower();
			break;		
			
			case',':
			printf("Encode Z: %d\r", encode[0]);
			printf("Encoder R1: %d\r", encode[1]);
			printf("Encoder R2: %d\r", encode[2]);
			break;	
			
			case 'x':
			pwm_channel_update_duty(PWM, &pwm_pin_7, 42);
			break;
			
			case 'c':
			pwm_channel_update_duty(PWM, &pwm_pin_7, 36);
			break;		
			
			case 'i':
			pio_set_pin_low(ZYLINDER_ZACHSE);
			break;
			
			case 'o':
			pio_set_pin_high(ZYLINDER_ZACHSE);
			break;
			
			case 'a':	//Z CCW			
			printf("Anzahl Schritte: \r");
			steps = get_input_value();
			encValue = encode[0] + (5*steps);
			printf("\rFahre %d Schritte.\r", encValue);
			gotoPosition(zAchse, encValue);
			break;	
			
			case 's':	//z CW
			printf("Anzahl Schritte: \r");
			steps = get_input_value();
			encValue = encode[0] - (5*steps);
			printf("\rFahre %d Schritte.\r", encValue);
			gotoPosition(zAchse, encValue);
			break;						

			case 'g':	//R2 CCW
			printf("Anzahl Schritte: \r");
			steps = get_input_value();
			encValue = encode[2] + (5*steps);
			printf("\rFahre %d Schritte.\r", encValue);
			gotoPosition(r2, encValue);
			break;
			
			case 'h':	//R2 CW
			printf("Anzahl Schritte: \r");
			steps = get_input_value();
			encValue = encode[2] - (5*steps);
			printf("\rFahre %d Schritte.\r", encValue);
			gotoPosition(r2, encValue);
			break;				

			default:
				printf("/ERROR COMMAND\r");
				}
			}
		
	}
}