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

//Variablen
unsigned int key = 0;
uint32_t cubePositions [3][6] ;



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
	unsigned int temp;
	uint32_t freq_temp = 2000;
	
	
	configure_console();
	board_init();
	sysclk_init();
	

//PWM11 Capture mode
/*
	ioport_set_pin_mode(ID_TC8, IOPORT_MODE_MUX_B);
	ioport_disable_pin(ID_TC8);

	sysclk_enable_peripheral_clock(ID_TC8);
	tc_init(TC2, 2, TC_CMR_TCCLKS_TIMER_CLOCK1
	|TC_CMR_LDRA_RISING
	|TC_CMR_LDRB_FALLING
	|TC_CMR_ABETRG
	|TC_CMR_ETRGEDG_FALLING);

	TC2->TC_CHANNEL[2].TC_IER = TC_IER_CPCS;
	TC2->TC_CHANNEL[2].TC_IER =~ TC_IDR_CPCS;
	NVIC_EnableIRQ(TC8_IRQn);
	tc_start(TC2,2);
	//
	*/
// 


	
		puts("--z z Achse \r--r r1 \r--t r2 \r--s Servo klemmen\r--l Servo �ffnen\r--m Servo in Mittelstellung\r");


	while(true){
		//test Achse aktiv
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

			numberOfSteps(r1, 2000, 1, true);
			
			numberOfSteps(r2, 5000, 1, true);		
			
			numberOfSteps(zAchse, 3000, 1, true);	

		


			break;
		
			case 'z':	
			printf("Anzahl Schritte: \r");
			steps = get_input_value();
			printf("\rFahre %d Schritte.\r", steps);
			numberOfSteps(zAchse, 200, 1, true);
			break;
		
			case 'r':
			printf("Anzahl Schritte: \r");
			steps = get_input_value();
			printf("\rFahre %d Schritte.\r", steps);
			numberOfSteps(r1, steps, 1, true);
			break;
		
			case 't':		
			printf("Anzahl Schritte: \r");
			steps = get_input_value();
			printf("\rFahre %d Schritte.\r", steps);
			numberOfSteps(r2, steps, 1, true);
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









/*
	key = uart_getc();	
	if (key & UART_NO_DATA )
        {
            / *no data available from UART* /
		}
	else
	{
		switch(key){
			printf("KEY: %d\r", key);
			case 0x00:
				uart_send((uint32_t) UART_EMPTY);
			break;
			
			case 0x01: / * Initialisieren * /
				/ * TODO Initialisieren uint32_t INIT_OK = init(void) * /
			//	uart_send(INIT_OK);
			break;
			
			case 0x02: / * W�rfel 1 R1 * /
				cubePositions[0][0] =	uart_get_data();
				uart_send(cubePositions[0][0]);

			break;
			
			case 0x03: / * W�rfel 1 R2 * /
				cubePositions[1][0] =	uart_get_data();
				uart_send(cubePositions[1][0]);			
			break;
			
			case 0x04: / * W�rfel 1 Phi * /
				cubePositions[2][0] =	uart_get_data();
				uart_send(cubePositions[2][0]);			
			break;
			
			case 0x05: / * W�rfel 2 R1 * /
				cubePositions[0][1] =	uart_get_data();
				uart_send(cubePositions[0][1]);			
			break;
			
			case 0x06: / * W�rfel 2 R2 * /
				cubePositions[1][1] =	uart_get_data();
				uart_send(cubePositions[1][1]);			
			break;
			
			case 0x07: / * W�rfel 2 Phi * /
				cubePositions[2][1] =	uart_get_data();
				uart_send(cubePositions[2][1]);			
			break;
			
			case 0x08: / * W�rfel 3 R1 * /
				cubePositions[0][2] =	uart_get_data();
				uart_send(cubePositions[0][2]);			
			break;
			
			case 0x09: / * W�rfel 3 R2 * /
				cubePositions[1][2] =	uart_get_data();
				uart_send(cubePositions[1][2]);			
			break;
			
			case 0x0A: / * W�rfel 3 Phi * /
				cubePositions[2][2] =	uart_get_data();
				uart_send(cubePositions[2][2]);			
			break;
			
			case 0x0B: / * W�rfel 4 R1 * /
				cubePositions[0][3] =	uart_get_data();
				uart_send(cubePositions[0][3]);			
			break;
			
			case 0x0C: / * W�rfel 4 R2 * /
				cubePositions[1][3] =	uart_get_data();
				uart_send(cubePositions[1][3]);			
			break;
			
			case 0x0D: / * W�rfel 4 Phi * /
				cubePositions[2][3] =	uart_get_data();
				uart_send(cubePositions[2][3]);			
			break;
			
			case 0x0E: / * W�rfel 5 R1 * /
				cubePositions[0][4] =	uart_get_data();
				uart_send(cubePositions[0][4]);			
			break;
			
			case 0x0F: / * W�rfel 5 R2 * /
				cubePositions[1][4] =	uart_get_data();
				uart_send(cubePositions[1][4]);			
			break;
			
			case 0x10: / * W�rfel 5 Phi * /
				cubePositions[2][4] =	uart_get_data();
				uart_send(cubePositions[2][4]);			
			break;
			
			case 0x11: / * W�rfel 6 R1 * /
				cubePositions[0][5] =	uart_get_data();
				uart_send(cubePositions[0][5]);			
			break;
			
			case 0x12: / * W�rfel 6 R2 * /
				cubePositions[1][5] =	uart_get_data();
				uart_send(cubePositions[1][5]);			
			break;
			
			case 0x13: / * W�rfel 6 Phy * /
				cubePositions[2][5] =	uart_get_data();
				uart_send(cubePositions[2][5]);			
			break;
			
			case 0x14: / * Alle Daten erhalten* /
			

			      for (i=0;i<3;i++)
			      {
				      printf("\r\r");
				      
				      for(j=0;j<6;j++)
				      {
					      printf("%d**",(*(*(cubePositions+i)+j)));
				      }
			      }
			      printf("\r");

			
			
				/ * TODO W�rfel einsammeln, Turm stellen* /
			break;
			
			case 0x1f: / *Turm stellen erzwingen* /
			/ * TODO Turm stellen * /
			break;			

			default:
				printf("ERROR\r");
			}

		}
*/
	}
}