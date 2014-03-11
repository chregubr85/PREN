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
#include <asf.h>
#include "conf_board.h"
#include "conf_clock.h"
#include "PWM_TC.h"
#include "Stepper_Driver.h"
#include "pwm.h"


pwm_channel_t pwm_pin_9;
pwm_channel_t pwm_pin_7;
uint8_t comand = 0;
uint32_t uart_data = 0;
uint8_t uart_count_bytes = 0;

static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.paritytype = CONF_UART_PARITY,
		.baudrate = CONF_UART_BAUDRATE
	};
	
	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
	
	uart_disable_interrupt(CONSOLE_UART, 0xffffffff);
	uart_enable_interrupt(CONSOLE_UART, 0x00000001);
	NVIC_EnableIRQ(UART_IRQn);
}

static uint32_t get_input_value(uint32_t ul_lower_limit, uint32_t ul_upper_limit)
{
	uint32_t i = 0, length = 0, value = 0, str_temp[5] = { 0 };
	uint8_t uc_key;

	while (1) {
		usart_serial_getchar((Usart *)CONSOLE_UART, &uc_key);

		if (uc_key == '\n' || uc_key == '\r') {
			puts("\r");
			break;
		}

		if ('0' <= uc_key && '9' >= uc_key) {
			printf("%c", uc_key);
			str_temp[i++] = uc_key;

			if (i >= 4) {
				break;
			}
		}
	}

	str_temp[i] = '\0';
	/* Input string length */
	length = i;
	value = 0;

	/* Convert string to integer */
	for (i = 0; i < 4; i++) {
		if (str_temp[i] != '0') {
			switch (length - i - 1) {
				case 0:
				value += (str_temp[i] - '0');
				break;

				case 1:
				value += (str_temp[i] - '0') * 10;
				break;

				case 2:
				value += (str_temp[i] - '0') * 100;
				break;

				case 3:
				value += (str_temp[i] - '0') * 1000;
				break;
				
				case 4:
				value += (str_temp[i] - '0') * 10000;
				break;
				
				case 5:
				value += (str_temp[i] - '0') * 100000;
				break;
				
				case 6:
				value += (str_temp[i] - '0') * 1000000;
				break;
				
				case 7:
				value += (str_temp[i] - '0') * 10000000;
				break;
				
				case 8:
				value += (str_temp[i] - '0') * 100000000;
				break;
				
				case 9:
				value += (str_temp[i] - '0') * 1000000000;
				break;
				
				case 10:
				value += (str_temp[i] - '0') * 10000000000;
				break;
			}
		}
	}

	if (value > ul_upper_limit || value < ul_lower_limit) {
		puts("\n\r-F- Input value is invalid!");
		return false;
	}

	return value;
}

uint32_t read4bytes(void){
	uint32_t data = 0;
	uint8_t temp =0;
	uint8_t i = 0;
	
	if(i == 0){
		uart_read(CONSOLE_UART, &temp);
		uart_disable_rx(CONSOLE_UART);
		data = temp;
		i++;
	}
	return data;
}

int main (void)
{
	
	uint8_t key;
	uint32_t steps;
	
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
/*
	pmc_enable_periph_clk(ID_PWM);
	
	pwm_clock_t clock_setting = {
		.ul_clka = 100000,
		.ul_clkb = 0,
		.ul_mck = sysclk_get_cpu_hz()
	};
	
	pwm_init(PWM, &clock_setting);
	
	pwm_pin_7.alignment		= PWM_ALIGN_LEFT;
	pwm_pin_7.polarity		= PWM_LOW;
	pwm_pin_7.ul_prescaler	= 0b1010;
	pwm_pin_7.ul_period		= 100;
	pwm_pin_7.ul_duty		= 50;
	pwm_pin_7.channel		= 6;
	
	
	pwm_channel_init(PWM, &pwm_pin_7);
	pwm_channel_enable(PWM, 6);

////
	pwm_pin_8.alignment		= PWM_ALIGN_LEFT;
	pwm_pin_8.polarity		= PWM_LOW;
	pwm_pin_8.ul_prescaler	= PWM_CMR_CPRE_CLKA;
	pwm_pin_8.ul_period		= 100;
	pwm_pin_8.ul_duty		= 50;
	pwm_pin_8.channel		= 5;
	
	pwm_channel_init(PWM, &pwm_pin_8);
	pwm_channel_enable(PWM, 5);
	////
	pwm_pin_9.alignment		= PWM_ALIGN_LEFT;
	pwm_pin_9.polarity		= PWM_LOW;
	pwm_pin_9.ul_prescaler	= PWM_CMR_CPRE_CLKA;
	pwm_pin_9.ul_period		= 100;
	pwm_pin_9.ul_duty		= 50;
	pwm_pin_9.channel		= 4;
		
	pwm_channel_init(PWM, &pwm_pin_9);
	pwm_channel_enable(PWM, 4);
//
*/


	timer_init((zAchse.pwm),1000);
	timer_init((r1.pwm),1000);
	timer_init((r2.pwm),1000);
	
	
	//puts("--z z Achse \r--r r1 200'000'000 Schritte\r--t r2 200'000'000 Schritte\r--s Servo klemmen\r--l Servo öffnen\r");
	
	
	while(true){
/*		
	while (uart_read(CONSOLE_UART, &key));	
		
		
	switch(key){
		case 'i':
		pio_set_pin_low(zAchse.ENBLE);
		pio_set_pin_low(zAchse.RESET);
		//setPinPIOC_low(zAchse.ENBLE);
		//setPinPIOC_low(r1.M1);
		break;
		
		case 'z':
		numberOfSteps(zAchse.pwm, 200000);
		delay_ms(500);
		pio_set_pin_high(zAchse.ENBLE);
		//setPinPIOC_high(zAchse.ENBLE);
		delay_ms(1000);
		pio_set_pin_high(zAchse.RESET);
		//setPinPIOC_high(r1.M1);
		break;
		
		case 'r':
		printf("Anzahl Schritte: \r");
		steps = get_input_value(0, 9999);
		printf("\rFahre %d Anzahl Schritte.\r", steps*100);
		numberOfSteps(r1.pwm, steps*100);
		break;
		
		case 't':
		printf("Anzahl Schritte: \r");
		scanf("%d",steps);
		printf("Fahre %d Anzahl Schritte.\r", steps);
		numberOfSteps(r2.pwm, steps);
		break;
		
		case 's':
		pwm_channel_update_duty(PWM, &pwm8, 80);
		break;
		
		case 'l':
		pwm_channel_update_duty(PWM, &pwm8, 20);
		break;
		
		default:
		printf("%d is not used! \r\n", key);
		}
		*/

if(!(uart_get_status(CONSOLE_UART) & UART_SR_RXRDY))
	while (uart_read(CONSOLE_UART, &key));	
	
		switch(key){
			case 'a':
			//printf("read: %d\r", read4bytes());
			printf("comand: %d\r", comand);
			printf("data: %d\r", uart_data);
			printf("count : %d\r", uart_count_bytes);
			break;
			
			case 0x01:
			
			break;
			
			case 0x02:
			
			break;
			
			case 0x03:
			
			break;
			
			case 0x04:
			
			break;
			
			case 0x05:
			
			break;
			
			case 0x06:
			
			break;
			
			case 0x07:
			
			break;
			
			case 0x08:
			
			break;
			
			case 0x09:
			
			break;
			
			case 0x0A:
			
			break;
			
			case 0x0B:
			
			break;
			
			case 0x0C:
			
			break;
			
			case 0x0D:
			
			break;
			
			case 0x0E:
			
			break;
			
			case 0x0F:
			
			break;
			
			case 0x10:
			
			break;
			
			case 0x11:
			
			break;
			
			case 0x12:
			
			break;
			
			case 0x13:
			
			break;
			
			case 0x14:
			
			break;
			
			case 0x15:
			
			break;
			
			default:
			printf("%d is not used! \r\n", key);
		}
		
		
	}
}

void UART_Handler(){
	uint8_t temp = 0;
	
	uart_read(CONSOLE_UART, &temp);
	
	if(uart_count_bytes == 0){
		comand = temp;
		uart_data = 0;
	}
	
	if(uart_count_bytes == 1){
		uart_data = temp;
	}
	
	if(uart_count_bytes == 2){
		uart_data =(uart_data << 8) + temp;
	}
	
	if(uart_count_bytes == 3){
		uart_data =(uart_data << 8) + temp;
	}
	
	if(uart_count_bytes == 4){
		uart_data =(uart_data << 8) + temp;
		uart_disable_interrupt(CONSOLE_UART, 0xffffffff);
		uart_count_bytes = 0;
	}
	uart_count_bytes++;
}