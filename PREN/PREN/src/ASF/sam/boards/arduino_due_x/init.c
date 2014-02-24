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



	
	/*Pin Deffinieren f�r Stepper1 Z-Achse
	- PIO_PD0 = PIN25		PIO_PC4  = PIN36
	- PIO_PC1 = PIN33		PIO_PC5  = PIN37
	- PIO_PC2 = PIN34		PIO_PB25 = PWM2
	- PIO_PC3 = PIN35
	*/
		pio_configure(PIOD, PIO_INPUT, PIO_PD0, PIO_DEFAULT);
		pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC1, PIO_DEFAULT);
		pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC2, PIO_DEFAULT);
		pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC3, PIO_DEFAULT);
		pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC4, PIO_DEFAULT);
		pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC5, PIO_DEFAULT);
		//PWM2 TIOA0
		pwm2.Timercounter = TC0;
		pwm2.channel = 0;
		pwm2.id = ID_TC0;
		pwm2.pin_id = PIO_PB25_IDX;
		pwm2.mux = IOPORT_MODE_MUX_B;
		
	/*Struct f�llen*/
	zAchse.ALERT = PIO_PD0;
	zAchse.M1 = PIO_PC1;
	zAchse.M2 = PIO_PC2;
	zAchse.M3 = PIO_PC3;
	zAchse.ENBLE = PIO_PC4;
	zAchse.pwm = pwm2;
	zAchse.DirectionOfRotation = PIO_PC5;
	

	
	/*Pin Deffinieren f�r Stepper1 R1
	- PIO_PD1 = PIN26		PIO_PC9  = PIN41
	- PIO_PC6 = PIN38		PIO_PC14 = PIN49
	- PIO_PC7 = PIN39		PIO_PC25 = PWM3
	- PIO_PC8 = PIN40
	*/
		pio_configure(PIOD, PIO_INPUT, PIO_PD1, PIO_DEFAULT);
		pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC6, PIO_DEFAULT);
		pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC7, PIO_DEFAULT);
		pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC8, PIO_DEFAULT);
		pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC9, PIO_DEFAULT);
		pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC14, PIO_DEFAULT);
		//PWM3 TIOA7
		pwm3.Timercounter = TC2;
		pwm3.channel = 1;
		pwm3.id = ID_TC7;
		pwm3.pin_id = PIO_PC28_IDX;
		pwm3.mux = IOPORT_MODE_MUX_B;
		
	/*Struct f�llen*/
	r1.ALERT = PIO_PD1;
	r1.M1 = PIO_PC6;
	r1.M2 = PIO_PC7;
	r1.M3 = PIO_PC8;
	r1.ENBLE = PIO_PC9;
	r1.pwm = pwm3;
	r1.DirectionOfRotation = PIO_PC14;
	
	
	/*Pin Deffinieren f�r Stepper1 R2
	- PIO_PD2  = PIN27		PIO_PC15 = PIN48
	- PIO_PC12 = PIN51		PIO_PC16 = PIN47
	- PIO_PC13 = PIN50		PIO_PC26 = PWM5
	- PIO_PC14 = PIN49
	*/
			pio_configure(PIOD, PIO_INPUT, PIO_PD2, PIO_DEFAULT);
			pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC12, PIO_DEFAULT);
			pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC13, PIO_DEFAULT);
			pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC14, PIO_DEFAULT);
			pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC15, PIO_DEFAULT);
			pio_configure(PIOC, PIO_OUTPUT_0, PIO_PC16, PIO_DEFAULT);
			//PWM5 TIOA6
			pwm5.Timercounter=TC2;
			pwm5.channel = 0;
			pwm5.id = ID_TC6;
			pwm5.pin_id = PIO_PC25_IDX;
			pwm5.mux = IOPORT_MODE_MUX_B;
		/*Struct f�llen*/
	r1.ALERT = PIO_PD2;
	r1.M1 = PIO_PC12;
	r1.M2 = PIO_PC13;
	r1.M3 = PIO_PC14;
	r1.ENBLE = PIO_PC15;
	r1.pwm = pwm5;
	r1.DirectionOfRotation = PIO_PC16;
	
	//PWM11 TIOA8
	pwm11.Timercounter = TC2;
	pwm11.channel = 2;
	pwm11.id = ID_TC8;
	pwm11.pin_id =PIO_PD7_IDX;
	pwm11.mux = IOPORT_MODE_MUX_B;
	
	//Interrupts Enable f�r PIOD 0-2 (Alert)
	pio_handler_set(PIOD, ID_PIOD, PIO_PD0, PIO_IT_RISE_EDGE, PIOD_ISR);
	pio_enable_interrupt(PIOD, PIO_PD0);
	pio_handler_set(PIOD, ID_PIOD, PIO_PD1, PIO_IT_RISE_EDGE, PIOD_ISR);
	pio_enable_interrupt(PIOD, PIO_PD1);
	pio_handler_set(PIOD, ID_PIOD, PIO_PD2, PIO_IT_RISE_EDGE, PIOD_ISR);
	pio_enable_interrupt(PIOD, PIO_PD2);
	NVIC_EnableIRQ(PIOD_IRQn);
	
	
#ifndef CONF_BOARD_KEEP_WATCHDOG_AT_INIT
	/* Disable the watchdog */
	WDT->WDT_MR = WDT_MR_WDDIS;
#endif

	/* GPIO has been deprecated, the old code just keeps it for compatibility.
	 * In new designs IOPORT is used instead.
	 * Here IOPORT must be initialized for others to use before setting up IO.
	 */
	ioport_init();
	
	
	pio_configure_pin(PIO_PC21_IDX, PIO_PERIPH_B | PIO_DEFAULT);
	pio_configure_pin(PIO_PC22_IDX, PIO_PERIPH_B | PIO_DEFAULT);
	pio_configure_pin(PIO_PC23_IDX, PIO_PERIPH_B | PIO_DEFAULT);
	
	
	
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
