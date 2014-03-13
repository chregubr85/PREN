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


uint32_t get_input_value( void )
{
	uint32_t i = 0, length = 0, value = 0, str_temp[10] = { 0 };
	unsigned int uc_key;

	while (1) {
		uc_key = uart_getc();
	 	if (uc_key & UART_NO_DATA )
	       {
	            //no data available from UART
		}
		else
 	{
		
		if (uc_key == '\n' || uc_key == '\r') {
			uart_putc("\r");
			break;
		}

		if ('0' <= uc_key && '9' >= uc_key) {
			printf("%c", uc_key);
			str_temp[i++] = uc_key;

			if (i >= 10) {
				break;
			}
		}
	}
	}
	str_temp[i] = '\0';
	/* Input string length */
	length = i;
	value = 0;

	/* Convert string to integer */
	for (i = 0; i < 10; i++) {
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

	return value;
}


/*Returns the 4 Data-Bytes*/
uint32_t uart_get_data(void)
{
	uint32_t uart_data = 0;
	unsigned int temp;
	uint8_t count = 0;
	
	while(count<=4)
	{
		temp = uart_getc();
		if(temp & UART_NO_DATA)
		{
			/*no data available from UART*/
			return UART_ERROR;
		}
		else
		{
			if(count <1)
			{
				uart_data = (unsigned char) temp;
				count++;
			}
			else
			{
				uart_data =(uart_data << 8) + (unsigned char) temp;
				count++;
			}
		}
	}
	
	return uart_data;
}


void uart_send(uint32_t data)
{
	if(data == UART_ERROR)
	{
				uart_putc(UART_NOK);
				uart_putc(UART_EMPTY);
				uart_putc(UART_EMPTY);
				uart_putc(UART_EMPTY);
				uart_putc(UART_EMPTY);
	}
	if(data == UART_INIT_OK)
	{
		uart_putc(UART_INIT);
		uart_putc(UART_EMPTY);
		uart_putc(UART_EMPTY);
		uart_putc(UART_EMPTY);
		uart_putc(UART_EMPTY);
	}
	else
	{
				uart_putc(UART_OK);
				uart_putc(UART_EMPTY);
				uart_putc(UART_EMPTY);
				uart_putc(UART_EMPTY);
				uart_putc(UART_EMPTY);
	}
	
}