/**
 * \file
 *
 * \brief Arduino Due/X board init.
 *
 * Copyright (c) 2011 - 2013 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#include "compiler.h"
#include "board.h"
#include "conf_board.h"
#include "gpio.h"
#include "ioport.h"
#include "PWM.h"
#include "Stepper_Driver.h"


void board_init(void)
{
	/*Pin Deffinieren für Stepper1 Z-Achse*/
		pio_configure(PIOA, PIO_INPUT, PIO_PA15, PIO_DEFAULT);		//ALERT
		pio_configure(PIOD, PIO_OUTPUT_0, PIO_PD0, PIO_DEFAULT);	//M1
		pio_configure(PIOD, PIO_OUTPUT_0, PIO_PD1, PIO_DEFAULT);	//M2
		pio_configure(PIOD, PIO_OUTPUT_0, PIO_PD2, PIO_DEFAULT);	//M3
		pio_configure(PIOD, PIO_OUTPUT_0, PIO_PD3, PIO_DEFAULT);	//ENABLE
		pio_configure(PIOD, PIO_OUTPUT_0, PIO_PD6, PIO_DEFAULT);	//CW/CCW
		pio_configure(PIOD, PIO_OUTPUT_0, PIO_PD9, PIO_DEFAULT);	//RESET
		pio_configure(PIOC, PIO_INPUT, PIO_PC15, PIO_PULLUP);		//Encoder Channel A
		pio_configure(PIOC, PIO_INPUT, PIO_PC14, PIO_PULLUP);		//Encoder Channel B
		
		//PWM2 TIOA0
		pwm2.Timercounter = TC0;
		pwm2.channel = 0;
		pwm2.id = ID_TC0;
		pwm2.pin_id = PIO_PB25_IDX;
		pwm2.mux = IOPORT_MODE_MUX_B;
		
	/*Struct füllen*/
	zAchse.ALERT	= PIO_PA15;
	zAchse.M1		= PIO_PD0_IDX;
	zAchse.M2		= PIO_PD1_IDX;
	zAchse.M3		= PIO_PD2_IDX;
	zAchse.ENBLE	= PIO_PD3_IDX;
	zAchse.RESET	= PIO_PD9_IDX;
	zAchse.CW_CCW	= PIO_PD6_IDX;
	zAchse.pwm		= pwm2;
	zAchse.ENC_A	= PIO_PC15_IDX;
	zAchse.ENC_B	= PIO_PC14_IDX;

	
	

	
	/*Pin Deffinieren für Stepper1 R1*/
		pio_configure(PIOD, PIO_INPUT, PIO_PD10, PIO_DEFAULT);		//ALERT
		pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC1, PIO_DEFAULT);	//M1
		pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC2, PIO_DEFAULT);	//M2
		pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC3, PIO_DEFAULT);	//M3
		pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC4, PIO_DEFAULT);	//ENABLE
		pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC5, PIO_DEFAULT);	//CW/CCW
		pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC6, PIO_DEFAULT);	//RESET
		pio_configure(PIOC, PIO_INPUT, PIO_PC13, PIO_PULLUP);		//Encoder Channel A
		pio_configure(PIOC, PIO_INPUT, PIO_PC12, PIO_PULLUP);		//Encoder Channel B		
		
		//PWM3 TIOA7
		pwm3.Timercounter = TC2;
		pwm3.channel = 1;
		pwm3.id = ID_TC7;
		pwm3.pin_id = PIO_PC28_IDX;
		pwm3.mux = IOPORT_MODE_MUX_B;
		
	/*Struct füllen*/
	r1.ALERT	= PIO_PD10;
	r1.M1		= PIO_PC1_IDX;
	r1.M2		= PIO_PC2_IDX;
	r1.M3		= PIO_PC3_IDX;
	r1.ENBLE	= PIO_PC4_IDX;
	r1.CW_CCW	= PIO_PC5_IDX;
	r1.RESET	= PIO_PC6_IDX;
	r1.pwm		= pwm3;
	r1.ENC_A	= PIO_PC13_IDX;
	r1.ENC_B	= PIO_PC12_IDX;
	
	
	/*Pin Deffinieren für Stepper1 R2*/
			pio_configure(PIOC, PIO_INPUT, PIO_PC8, PIO_DEFAULT);		//ALERT
			pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC9, PIO_DEFAULT);	//M1
			pio_configure(PIOA, PIO_OUTPUT_0, PIO_PA19, PIO_DEFAULT);	//M2
			pio_configure(PIOA, PIO_OUTPUT_0, PIO_PA20, PIO_DEFAULT);	//M3
			pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC19, PIO_DEFAULT);	//ENABLE
			pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC18, PIO_DEFAULT);	//CW/CCW
			pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC17, PIO_DEFAULT);	//RESET
			pio_configure(PIOB, PIO_INPUT, PIO_PC21, PIO_PULLUP);		//Encoder Channel A
			pio_configure(PIOB, PIO_INPUT, PIO_PC14, PIO_PULLUP);		//Encoder Channel B			
			
			//PWM5 TIOA6
			pwm5.Timercounter=TC2;
			pwm5.channel = 0;
			pwm5.id = ID_TC6;
			pwm5.pin_id = PIO_PC25_IDX;
			pwm5.mux = IOPORT_MODE_MUX_B;
			
		/*Struct füllen*/
	r2.ALERT	= PIO_PC8;
	r2.M1		= PIO_PC9_IDX;
	r2.M2		= PIO_PA19_IDX;
	r2.M3		= PIO_PA20_IDX;
	r2.ENBLE	= PIO_PC19_IDX;
	r2.CW_CCW	= PIO_PC18_IDX;
	r2.RESET	= PIO_PC17_IDX;
	r2.pwm		= pwm5;
	r2.ENC_A	= PIO_PB21_IDX;
	r2.ENC_B	= PIO_PB14_IDX;
	
	
	//PWM11 TIOA8
	pwm11.Timercounter = TC2;
	pwm11.channel = 2;
	pwm11.id = ID_TC8;
	pwm11.pin_id =PIO_PD7_IDX;
	pwm11.mux = IOPORT_MODE_MUX_B;
	
	/*SERVO*/
	//Clock Settings
	pmc_enable_periph_clk(ID_PWM);
	
	pwm_clock_t clock_setting = {
		.ul_clka = 20000, //50Hz
		.ul_clkb = 0,
		.ul_mck = sysclk_get_cpu_hz()
	};
	pwm_init(PWM, &clock_setting);
	
	pwm_pin_7.alignment		= PWM_ALIGN_LEFT;
	pwm_pin_7.polarity		= PWM_LOW;
	pwm_pin_7.ul_prescaler	= PWM_CMR_CPRE_CLKA;
	pwm_pin_7.ul_period		= 100;
	pwm_pin_7.ul_duty		= 30;
	pwm_pin_7.channel		= 6;
		
	pwm_channel_init(PWM, &pwm_pin_7);
	pwm_channel_enable(PWM, 6);
	
	//Interrupts Enable für Alert  #TODO
/*	pio_handler_set(PIOA, ID_PIOA, PIO_PA15, PIO_IT_RISE_EDGE, PIOD_ISR);
	pio_enable_interrupt(PIOA, PIO_PA15);
	NVIC_EnableIRQ(PIOA_IRQn);
	
	pio_handler_set(PIOD, ID_PIOD, PIO_PD10, PIO_IT_RISE_EDGE, PIOD_ISR);
	pio_enable_interrupt(PIOD, PIO_PD10);
	NVIC_EnableIRQ(PIOD_IRQn);
	
	pio_handler_set(PIOC, ID_PIOC, PIO_PC8, PIO_IT_RISE_EDGE, PIOD_ISR);
	pio_enable_interrupt(PIOD, PIO_PC8);
	NVIC_EnableIRQ(PIOC_IRQn);
	*/
	//TC1 mit 0.01s Interrupt
	sysclk_enable_peripheral_clock(ID_TC1);
	tc_init(TC0, 1,TC_CMR_TCCLKS_TIMER_CLOCK3
	| TC_CMR_WAVE /* Waveform mode */
	| TC_CMR_WAVSEL_UP_RC
	);
	tc_write_rc(TC0, 1, 26250);
	/*Interrupt enable*/
	TC0->TC_CHANNEL[1].TC_IER = TC_IER_CPCS;
	TC0->TC_CHANNEL[1].TC_IER =~ TC_IDR_CPCS;
	NVIC_EnableIRQ(TC1_IRQn);
	
	
	/*PWM 7,8,9*/
	pio_configure_pin(PIO_PC21_IDX, PIO_PERIPH_B | PIO_DEFAULT);
	pio_configure_pin(PIO_PC22_IDX, PIO_PERIPH_B | PIO_DEFAULT);
	pio_configure_pin(PIO_PC23_IDX, PIO_PERIPH_B | PIO_DEFAULT);
	
	/*ARDUINO DEFINED*/
	
	
#ifndef CONF_BOARD_KEEP_WATCHDOG_AT_INIT
	/* Disable the watchdog */
	WDT->WDT_MR = WDT_MR_WDDIS;
#endif

	/* GPIO has been deprecated, the old code just keeps it for compatibility.
	 * In new designs IOPORT is used instead.
	 * Here IOPORT must be initialized for others to use before setting up IO.
	 */
	ioport_init();
	
	/* Configure Push Button pins */
	gpio_configure_pin(GPIO_PUSH_BUTTON_1, GPIO_PUSH_BUTTON_1_FLAGS);
	//gpio_configure_pin(GPIO_PUSH_BUTTON_2, GPIO_PUSH_BUTTON_2_FLAGS);


#ifdef CONF_BOARD_UART_CONSOLE
	/* Configure UART pins */
	gpio_configure_group(PINS_UART_PIO, PINS_UART, PINS_UART_FLAGS);
#endif

	/* Configure ADC example pins */
#ifdef CONF_BOARD_ADC
	/* TC TIOA configuration */
	gpio_configure_pin(PIN_TC0_TIOA0,PIN_TC0_TIOA0_FLAGS);

	/* ADC Trigger configuration */
	gpio_configure_pin(PINS_ADC_TRIG, PINS_ADC_TRIG_FLAG);

	/* PWMH0 configuration */
	gpio_configure_pin(PIN_PWMC_PWMH0_TRIG, PIN_PWMC_PWMH0_TRIG_FLAG);
#endif

#ifdef CONF_BOARD_PWM_LED0
	/* Configure PWM LED0 pin */
	gpio_configure_pin(PIN_PWM_LED0_GPIO, PIN_PWM_LED0_FLAGS);
#endif


#ifdef CONF_BOARD_PWM_LED2
	/* Configure PWM LED2 pin */
	gpio_configure_pin(PIN_PWM_LED2_GPIO, PIN_PWM_LED2_FLAGS);
#endif

	/* Configure SPI0 pins */
#ifdef CONF_BOARD_SPI0
	gpio_configure_pin(SPI0_MISO_GPIO, SPI0_MISO_FLAGS);
	gpio_configure_pin(SPI0_MOSI_GPIO, SPI0_MOSI_FLAGS);
	gpio_configure_pin(SPI0_SPCK_GPIO, SPI0_SPCK_FLAGS);

	/**
	 * For NPCS 1, 2, and 3, different PINs can be used to access the same
	 * NPCS line.
	 * Depending on the application requirements, the default PIN may not be
	 * available.
	 * Hence a different PIN should be selected using the
	 * CONF_BOARD_SPI_NPCS_GPIO and
	 * CONF_BOARD_SPI_NPCS_FLAGS macros.
	 */

#   ifdef CONF_BOARD_SPI0_NPCS0
		gpio_configure_pin(SPI0_NPCS0_GPIO, SPI0_NPCS0_FLAGS);
#   endif

#   ifdef CONF_BOARD_SPI0_NPCS1
	        gpio_configure_pin(SPI0_NPCS1_PA29_GPIO,SPI0_NPCS1_PA29_FLAGS);
#   endif
#endif // #ifdef CONF_BOARD_SPI0

	/* Configure SPI1 pins */
#ifdef CONF_BOARD_SPI1
	gpio_configure_pin(SPI1_MISO_GPIO, SPI1_MISO_FLAGS);
	gpio_configure_pin(SPI1_MOSI_GPIO, SPI1_MOSI_FLAGS);
	gpio_configure_pin(SPI1_SPCK_GPIO, SPI1_SPCK_FLAGS);

#   ifdef CONF_BOARD_SPI1_NPCS0
		gpio_configure_pin(SPI1_NPCS0_GPIO, SPI1_NPCS0_FLAGS);
#   endif

#   ifdef CONF_BOARD_SPI1_NPCS1
		gpio_configure_pin(SPI1_NPCS1_GPIO, SPI1_NPCS1_FLAGS);
#   endif

#   ifdef CONF_BOARD_SPI1_NPCS2
		gpio_configure_pin(SPI1_NPCS2_GPIO, SPI1_NPCS2_FLAGS);
#   endif

#   ifdef CONF_BOARD_SPI1_NPCS3
		gpio_configure_pin(SPI1_NPCS3_GPIO, SPI1_NPCS3_FLAGS);
#   endif
#endif

#ifdef CONF_BOARD_TWI0
	gpio_configure_pin(TWI0_DATA_GPIO, TWI0_DATA_FLAGS);
	gpio_configure_pin(TWI0_CLK_GPIO, TWI0_CLK_FLAGS);
#endif

#ifdef CONF_BOARD_TWI1
	gpio_configure_pin(TWI1_DATA_GPIO, TWI1_DATA_FLAGS);
	gpio_configure_pin(TWI1_CLK_GPIO, TWI1_CLK_FLAGS);
#endif

#ifdef CONF_BOARD_USART_RXD
	/* Configure USART RXD pin */
	gpio_configure_pin(PIN_USART0_RXD_IDX, PIN_USART0_RXD_FLAGS);
#endif

#ifdef CONF_BOARD_USART_TXD
	/* Configure USART TXD pin */
	gpio_configure_pin(PIN_USART0_TXD_IDX, PIN_USART0_TXD_FLAGS);
#endif

#ifdef CONF_BOARD_USB_PORT
	/* Configure USB_ID (UOTGID) pin */
	gpio_configure_pin(USB_ID_GPIO, USB_ID_FLAGS);
	/* Configure USB_VBOF (UOTGVBOF) pin */
	gpio_configure_pin(USB_VBOF_GPIO, USB_VBOF_FLAGS);
#endif

#ifdef CONF_BOARD_MMA7341L
	/* Configure MMA7341L mode set control pin */
	gpio_configure_pin(PIN_MMA7341L_MODE, PIN_MMA7341L_MODE_FLAG);
	/* Configure MMA7341L x,y,z axis output voltage pin */
	gpio_configure_pin(PIN_MMA7341L_X_AXIS, PIN_MMA7341L_X_AXIS_FLAG);
	gpio_configure_pin(PIN_MMA7341L_Y_AXIS, PIN_MMA7341L_Y_AXIS_FLAG);
	gpio_configure_pin(PIN_MMA7341L_Z_AXIS, PIN_MMA7341L_Z_AXIS_FLAG);
#endif

#ifdef CONF_BOARD_ADS7843
	/* Configure Touchscreen SPI pins */
	gpio_configure_pin(BOARD_ADS7843_IRQ_GPIO,BOARD_ADS7843_IRQ_FLAGS);
	gpio_configure_pin(BOARD_ADS7843_BUSY_GPIO, BOARD_ADS7843_BUSY_FLAGS);
	gpio_configure_pin(SPI0_MISO_GPIO, SPI0_MISO_FLAGS);
	gpio_configure_pin(SPI0_MOSI_GPIO, SPI0_MOSI_FLAGS);
	gpio_configure_pin(SPI0_SPCK_GPIO, SPI0_SPCK_FLAGS);
	gpio_configure_pin(SPI0_NPCS0_GPIO, SPI0_NPCS0_FLAGS);
#endif
}
