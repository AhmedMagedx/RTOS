#include <avr/io.h>
#include "STD_TYPES.h"

#define       FCLK_SYSTEM     8000000UL



void          usart_init(unsigned short int baudrate);
void          usart_putc(unsigned char data);
unsigned char usart_getc(void);
void          usart_puts(char *str);
ERROR_t usart_getc_NonBlocking(u8 * pData );
