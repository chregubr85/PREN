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

	
	configure_console();
	board_init();
	sysclk_init();
	
	
	//puts("--z z Achse \r--r r1 \r--t r2 \r--s Servo klemmen\r--l Servo öffnen\r--m Servo in Mittelstellung\r");


	while(true){
		
		/* Timer für Encoder stoppen wenn keine Achse aktiv ist */
	if(active[0] == false && active[1] == false && active[2] == false){
		tc_stop(TC0, 1);
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
			case 'i':
	
			while(true){
							numberOfSteps(zAchse, 200, FULLSTEP, CLOCKWISE);
							delay_ms(750);
							
			}
		


			break;
		
			case 'z':	
			printf("Anzahl Schritte: \r");
			steps = get_input_value();
			printf("\rFahre %d Schritte.\r", steps);
			numberOfSteps(zAchse, 200, FULLSTEP, CLOCKWISE);
			break;
		
			case 'r':
			printf("Anzahl Schritte: \r");
			steps = get_input_value();
			printf("\rFahre %d Schritte.\r", steps);
			numberOfSteps(r1, steps, FULLSTEP, CLOCKWISE);
			break;
		
			case 't':		
			printf("Anzahl Schritte: \r");
			steps = get_input_value();
			printf("\rFahre %d Schritte.\r", steps);
			numberOfSteps(r2, steps, FULLSTEP, CLOCKWISE);
			break;
		
			case 's':
			pwm_channel_update_duty(PWM, &pwm_pin_7, 21);
			break;
		
			case 'l':
			pwm_channel_update_duty(PWM, &pwm_pin_7, 42);
			break;
			
			case 'm':
			pwm_channel_update_duty(PWM, &pwm_pin_7, 30);
			break;
		
			default:
			printf("%d is not used! \r\n", key);
			}
		 }



*/




	key = uart_getc();	
	if (key & UART_NO_DATA )
        {
            //no data available from UART 
		}
	else
	{
		switch(key){

			case 0x00: //Hello
				uart_get_data();
				uart_send(UART_OK_32);
			break;
			
			case 0x01: // Initialisieren  
				uart_get_data();
			//	init_ok = initialPosition(); 
				init_ok = UART_INIT_OK;
				uart_send(init_ok);
			break;
			
			case 0x02: // Würfel 1 R1  
				cubePositions[0][0] =	uart_get_data();
				uart_send(cubePositions[0][0]);

			break;
			
			case 0x03: // Würfel 1 R2  
				cubePositions[1][0] =	uart_get_data();
				uart_send(cubePositions[1][0]);			
			break;
			
			case 0x04: // Würfel 1 Phi  
				cubePositions[2][0] =	uart_get_data();
				uart_send(cubePositions[2][0]);			
			break;
			
			case 0x05: // Würfel 2 R1  
				cubePositions[0][1] =	uart_get_data();
				uart_send(cubePositions[0][1]);			
			break;
			
			case 0x06: // Würfel 2 R2  
				cubePositions[1][1] =	uart_get_data();
				uart_send(cubePositions[1][1]);			
			break;
			
			case 0x07: // Würfel 2 Phi  
				cubePositions[2][1] =	uart_get_data();
				uart_send(cubePositions[2][1]);			
			break;
			
			case 0x08: // Würfel 3 R1  
				cubePositions[0][2] =	uart_get_data();
				uart_send(cubePositions[0][2]);			
			break;
			
			case 0x09: // Würfel 3 R2  
				cubePositions[1][2] =	uart_get_data();
				uart_send(cubePositions[1][2]);			
			break;
			
			case 0x0A: // Würfel 3 Phi  
				cubePositions[2][2] =	uart_get_data();
				uart_send(cubePositions[2][2]);			
			break;
			
			case 0x0B: // Würfel 4 R1  
				cubePositions[0][3] =	uart_get_data();
				uart_send(cubePositions[0][3]);			
			break;
			
			case 0x0C: // Würfel 4 R2  
				cubePositions[1][3] =	uart_get_data();
				uart_send(cubePositions[1][3]);			
			break;
			
			case 0x0D: // Würfel 4 Phi  
				cubePositions[2][3] =	uart_get_data();
				uart_send(cubePositions[2][3]);			
			break;
			
			case 0x0E: // Würfel 5 R1  
				cubePositions[0][4] =	uart_get_data();
				uart_send(cubePositions[0][4]);			
			break;
			
			case 0x0F: // Würfel 5 R2  
				cubePositions[1][4] =	uart_get_data();
				uart_send(cubePositions[1][4]);			
			break;
			
			case 0x10: // Würfel 5 Phi  
				cubePositions[2][4] =	uart_get_data();
				uart_send(cubePositions[2][4]);			
			break;
			
			case 0x11: // Würfel 6 R1  
				cubePositions[0][5] =	uart_get_data();
				uart_send(cubePositions[0][5]);			
			break;
			
			case 0x12: // Würfel 6 R2  
				cubePositions[1][5] =	uart_get_data();
				uart_send(cubePositions[1][5]);			
			break;
			
			case 0x13: // Würfel 6 Phy  
				cubePositions[2][5] =	uart_get_data();
				uart_send(cubePositions[2][5]);			
			break;
			
			case 0x14: // Alle Daten erhalten 
			 	
				//Würfel 1
				getCube(cubePositions[0][0], cubePositions[1][0], cubePositions[2][0]);
				
				//Würfel 2
				getCube(cubePositions[0][0], cubePositions[1][0], cubePositions[2][0]);		
						
				//Würfel 3
				getCube(cubePositions[0][0], cubePositions[1][0], cubePositions[2][0]);	
						
				//Würfel 4
				getCube(cubePositions[0][0], cubePositions[1][0], cubePositions[2][0]);	
							
				//Würfel 5
				getCube(cubePositions[0][0], cubePositions[1][0], cubePositions[2][0]);	
							
				//Würfel 6
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

		}
	}
}