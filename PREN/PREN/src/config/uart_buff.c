#include "uart_buff.h"
#include <asf.h>
#include "PWM_TC.h"
#include "Stepper_Driver.h"
/*
 *  constants and macros
 */

/* size of RX/TX buffers */
#define UART_RX_BUFFER_MASK ( UART_RX_BUFFER_SIZE - 1)
#define UART_TX_BUFFER_MASK ( UART_TX_BUFFER_SIZE - 1)

/*
 *  module global variables
 */
static volatile unsigned char UART_TxBuf[UART_TX_BUFFER_SIZE];
static volatile unsigned char UART_RxBuf[UART_RX_BUFFER_SIZE];
static volatile unsigned char UART_TxHead;
static volatile unsigned char UART_TxTail;
static volatile unsigned char UART_RxHead;
static volatile unsigned char UART_RxTail;
static volatile unsigned char UART_LastRxError;


void UART_Handler(){

	   unsigned char tmphead;
	   unsigned char data;
	   unsigned char usr;
	   unsigned char lastRxError;
	   unsigned char tmptail;
	
	
	if(uart_get_status(CONSOLE_UART) & UART_SR_RXRDY){
		uart_read(CONSOLE_UART, &data);
		
		/* calculate buffer index */
		tmphead = ( UART_RxHead + 1) & UART_RX_BUFFER_MASK;
		
		if ( tmphead == UART_RxTail ) {
			/* error: receive buffer overflow */
			lastRxError = UART_BUFFER_OVERFLOW >> 8;
			}
			else{
			/* store new index */
			UART_RxHead = tmphead;
			/* store received data in buffer */
			UART_RxBuf[tmphead] = data;
			}
		UART_LastRxError |= lastRxError;
	}
	
	if(uart_get_status(CONSOLE_UART) & UART_SR_TXRDY){ 
	   if ( UART_TxHead != UART_TxTail) {
		   /* calculate and store new buffer index */
		   tmptail = (UART_TxTail + 1) & UART_TX_BUFFER_MASK;
		   UART_TxTail = tmptail;
		   /* get one byte from buffer and write it to UART */
		   uart_write(CONSOLE_UART, UART_TxBuf[tmptail]);
		   
		   }else{
		   /* tx buffer empty, disable UDRE interrupt */
		   uart_disable_interrupt(CONSOLE_UART, UART_SR_TXRDY);
	   }
	
		
	}
}



/*************************************************************************
Function: uart_init()
Purpose:  initialize UART and set baudrate
Input:    baudrate using macro UART_BAUD_SELECT()
Returns:  none
**************************************************************************/
void uart_buff_init(void)
{
    UART_TxHead = 0;
    UART_TxTail = 0;
    UART_RxHead = 0;
    UART_RxTail = 0;
    

	
}/* uart_init */


/*************************************************************************
Function: uart_getc()
Purpose:  return byte from ringbuffer  
Returns:  lower byte:  received byte from ringbuffer
          higher byte: last receive error
**************************************************************************/
unsigned int uart_getc(void)
{    
    unsigned char tmptail;
    unsigned char data;

    if ( UART_RxHead == UART_RxTail ) {
        return UART_NO_DATA;   /* no data available */
    }
    
    /* calculate /store buffer index */
    tmptail = (UART_RxTail + 1) & UART_RX_BUFFER_MASK;
    UART_RxTail = tmptail; 
    
    /* get data from receive buffer */
    data = UART_RxBuf[tmptail];
    
    data = (UART_LastRxError << 8) + data;
    UART_LastRxError = 0;
    return data;

}/* uart_getc */


/*************************************************************************
Function: uart_putc()
Purpose:  write byte to ringbuffer for transmitting via UART
Input:    byte to be transmitted
Returns:  none          
**************************************************************************/
void uart_putc(unsigned char data)
{
    unsigned char tmphead;

    
    tmphead  = (UART_TxHead + 1) & UART_TX_BUFFER_MASK;
    
    while ( tmphead == UART_TxTail ){
        ;/* wait for free space in buffer */
    }
    
    UART_TxBuf[tmphead] = data;
    UART_TxHead = tmphead;

    /* enable UDRE interrupt */
	uart_enable_interrupt(CONSOLE_UART, UART_SR_TXRDY);

}/* uart_putc */


/*************************************************************************
Function: uart_puts()
Purpose:  transmit string to UART
Input:    string to be transmitted
Returns:  none          
**************************************************************************/
void uart_puts(const char *s )
{
    while (*s) 
      uart_putc(*s++);

}/* uart_puts */

