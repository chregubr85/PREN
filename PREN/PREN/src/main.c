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

pwm_channel_t pwm_pin_8;
pwm_channel_t pwm_pin_9;
pwm_channel_t pwm_pin_7;


static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.paritytype = CONF_UART_PARITY,
		.baudrate = CONF_UART_BAUDRATE
	};
	
	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
	
}

int main (void)
{
	board_init();
	sysclk_init();
	configure_console();

//PWM11 Capture mode
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


	timer_init((zAchse.pwm),40000);
	numberOfSteps(zAchse.pwm, 2000000);
//tc_start(TC0, 1);
}
