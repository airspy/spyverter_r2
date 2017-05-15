#include <C8051F330.H>

#include "hw_config.h"

#define SERIAL_BUFFER_SIZE 16

struct ring_buffer
{
  char buffer[SERIAL_BUFFER_SIZE];
  volatile unsigned char head;
  volatile unsigned char tail;
};

xdata struct  ring_buffer rx_buffer = { { 0 }, 0, 0};

void UART_RX_ISR(void) interrupt 4 using 0
{
	char c,i;
	EA = 0; // Disable global interrupt
	if(RI0 == 1)
	{
		RI0 = 0;
		c = SBUF0;
		i = (rx_buffer.head + 1) % SERIAL_BUFFER_SIZE;
		if (i != rx_buffer.tail) 
		{
    		rx_buffer.buffer[rx_buffer.head] = c;
    		rx_buffer.head = i;
		}
	}
	EA =1;
	return;
}

unsigned char Uart0Available(void)
{
	 return (unsigned char)(SERIAL_BUFFER_SIZE + rx_buffer.head - rx_buffer.tail) % SERIAL_BUFFER_SIZE;
}

char Uart0Read(void)
{
	char c;
	if (rx_buffer.head == rx_buffer.tail)
	{
		return -1;
	}
	else
	{
		c = rx_buffer.buffer[rx_buffer.tail];
		rx_buffer.tail = (unsigned char)(rx_buffer.tail + 1) % SERIAL_BUFFER_SIZE;
		return c;
	}
}

void Uart0Write(char c)
{
	TI0	= 0;
	SBUF0 = c;	
	while(TI0 == 0);
}