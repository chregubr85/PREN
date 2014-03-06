/*
 * Terminal.c
 *
 * Created: 27.02.2014 14:09:19
 *  Author: Christian
 */ 

#include "stdio_serial.h"

static uint32_t get_input_value(uint32_t ul_lower_limit,
uint32_t ul_upper_limit)
{
	uint32_t i = 0, length = 0, value = 0;
	uint8_t uc_key, str_temp[5] = { 0 };

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
			}
		}
	}

	if (value > ul_upper_limit || value < ul_lower_limit) {
		puts("\n\r-F- Input value is invalid!");
		return false;
	}

	return value;
}