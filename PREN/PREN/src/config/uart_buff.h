#ifndef UART_BUFF_H
#define UART_BUFF_H

#include <asf.h>

/** Size of the circular receive buffer, must be power of 2 */
#ifndef UART_RX_BUFFER_SIZE
#define UART_RX_BUFFER_SIZE 32
#endif
/** Size of the circular transmit buffer, must be power of 2 */
#ifndef UART_TX_BUFFER_SIZE
#define UART_TX_BUFFER_SIZE 32
#endif


/* 
** high byte error return code of uart_getc()
*/
#define UART_FRAME_ERROR      0x1000              /* Framing Error by UART       */
#define UART_OVERRUN_ERROR    0x0800              /* Overrun condition by UART   */
#define UART_PARITY_ERROR     0x0400              /* Parity Error by UART        */ 
#define UART_BUFFER_OVERFLOW  0x0200              /* receive ringbuffer overflow */
#define UART_NO_DATA          0x0100              /* no receive data available   */
#define UART_ERROR			  0xffffffff		  /* Allgemeiner Fehler			 */
#define UART_INIT_OK		  0xffff0000		  /* Initialisiert  			 */
#define UART_OK_32			  0xeeeeeeee		  /* OK				 			 */

/*
** usual Values to send
*/

#define UART_OK				 0x20				  /* Daten empfangen			  */
#define UART_NOK			 0x21				  /* Daten nicht empfangen		  */
#define UART_INIT			 0x22				  /* Initialisieren abgeschlossen */
#define UART_EMPTY			 0x00				  /* leeres Byte	    		  */

/*
** function prototypes
*/

/**
   @brief   Initialize UART and set baudrate 
   @param   baudrate Specify baudrate using macro UART_BAUD_SELECT()
   @return  none
*/
extern void uart_buff_init(void);

extern unsigned int uart_getc(void);

extern void uart_putc(unsigned char data);

extern void uart_puts(const char *s );

uint32_t get_input_value(void);

uint32_t uart_get_data(void);

void uart_send(uint32_t data);

#endif // UART_H 

