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
	
	configure_console();
	board_init();
	sysclk_init();
	
	
	puts("--i Zylinder Z aus\r--o Zyliner Z ein\r\r--k Zylinder Stack aus\r--l Zylinder Stack ein\r\r--a Z CCW\r--s Z CW\r\r--d R1 CCW\r--f R1 CW\r\r--g R2 CCW\r--h R2 CW\r\r--x Servo �ffnen\r--c Servo in Mittelstellung\r ");


	while(true){
		
		// Timer f�r Encoder stoppen wenn keine Achse aktiv ist //
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
			encValue = encode[0] + (-5*steps);
			printf("\rFahre %d Schritte.\r", encValue);
			gotoPosition(zAchse, encValue);
			break;	
			
			case 's':	//z CW
			printf("Anzahl Schritte: \r");
			steps = get_input_value();
			encValue = encode[0] - (-5*steps);
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
			
			case 'u':
			//printf("Encode maxvalue Z: %d\r", encode[0]);
			printf("Encode maxvalue R1: %d\r", encode[1]);
			//printf("Encode maxvalue R2: %d\r", encode[2]);
			break;
			
			case 'y':
			pio_set_pin_low(zAchse.RESET);
			pio_set_pin_low(r1.RESET);
			pio_set_pin_low(r2.RESET);
			break;
			
			case 'p':
			initialPosition();
			break;
			
			case 'm':
			encode_init(zAchse.pwm);
			while(true){
				encode[0]+=encode_zAchse_read4();
				printf("Encode Z: %d\r", encode[0]);
			}
			break;
			
			default:
			printf("%d is not used! \r\n", key);
			}
		 }






/*

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
				if(startPosition()){
					uart_send(init_ok);
				}
			break;
			
			case 0x02: // W�rfel 1 R1  
				cubePositions[0][0] =	uart_get_data();
				uart_send(cubePositions[0][0]);

			break;
			
			case 0x03: // W�rfel 1 R2  
				cubePositions[1][0] =	uart_get_data();
				uart_send(cubePositions[1][0]);			
			break;
			
			case 0x04: // W�rfel 1 Phi  
				cubePositions[2][0] =	uart_get_data();
				uart_send(cubePositions[2][0]);			
			break;
			
			case 0x05: // W�rfel 2 R1  
				cubePositions[0][1] =	uart_get_data();
				uart_send(cubePositions[0][1]);			
			break;
			
			case 0x06: // W�rfel 2 R2  
				cubePositions[1][1] =	uart_get_data();
				uart_send(cubePositions[1][1]);			
			break;
			
			case 0x07: // W�rfel 2 Phi  
				cubePositions[2][1] =	uart_get_data();
				uart_send(cubePositions[2][1]);			
			break;
			
			case 0x08: // W�rfel 3 R1  
				cubePositions[0][2] =	uart_get_data();
				uart_send(cubePositions[0][2]);			
			break;
			
			case 0x09: // W�rfel 3 R2  
				cubePositions[1][2] =	uart_get_data();
				uart_send(cubePositions[1][2]);			
			break;
			
			case 0x0A: // W�rfel 3 Phi  
				cubePositions[2][2] =	uart_get_data();
				uart_send(cubePositions[2][2]);			
			break;
			
			case 0x0B: // W�rfel 4 R1  
				cubePositions[0][3] =	uart_get_data();
				uart_send(cubePositions[0][3]);			
			break;
			
			case 0x0C: // W�rfel 4 R2  
				cubePositions[1][3] =	uart_get_data();
				uart_send(cubePositions[1][3]);			
			break;
			
			case 0x0D: // W�rfel 4 Phi  
				cubePositions[2][3] =	uart_get_data();
				uart_send(cubePositions[2][3]);			
			break;
			
			case 0x0E: // W�rfel 5 R1  
				cubePositions[0][4] =	uart_get_data();
				uart_send(cubePositions[0][4]);			
			break;
			
			case 0x0F: // W�rfel 5 R2  
				cubePositions[1][4] =	uart_get_data();
				uart_send(cubePositions[1][4]);			
			break;
			
			case 0x10: // W�rfel 5 Phi  
				cubePositions[2][4] =	uart_get_data();
				uart_send(cubePositions[2][4]);			
			break;
			
			case 0x11: // W�rfel 6 R1  
				cubePositions[0][5] =	uart_get_data();
				uart_send(cubePositions[0][5]);			
			break;
			
			case 0x12: // W�rfel 6 R2  
				cubePositions[1][5] =	uart_get_data();
				uart_send(cubePositions[1][5]);			
			break;
			
			case 0x13: // W�rfel 6 Phy  
				cubePositions[2][5] =	uart_get_data();
				uart_send(cubePositions[2][5]);			
			break;
			
			case 0x14: // Alle Daten erhalten 
			 	
				//W�rfel 1
				getCube(cubePositions[0][0], cubePositions[1][0], cubePositions[2][0]);
				
				//W�rfel 2
				getCube(cubePositions[0][0], cubePositions[1][0], cubePositions[2][0]);		
						
				//W�rfel 3
				getCube(cubePositions[0][0], cubePositions[1][0], cubePositions[2][0]);	
						
				//W�rfel 4
				getCube(cubePositions[0][0], cubePositions[1][0], cubePositions[2][0]);	
							
				//W�rfel 5
				getCube(cubePositions[0][0], cubePositions[1][0], cubePositions[2][0]);	
							
				//W�rfel 6
				getCube(cubePositions[0][0], cubePositions[1][0], cubePositions[2][0]);				
				
				startPositionOk = placeTower();
				
				if(startPositionOk){
					uart_send(UART_INIT_OK);
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

			default:
				printf("ERROR\r");
			}

		}*/
	}
}