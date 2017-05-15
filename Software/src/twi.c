#include <C8051F330.H>

#include "hw_config.h"

#define TWI_BUFFER_SIZE 16
#define DELAYTIME 255
#define DELAY for(NOP = 0; NOP < DELAYTIME; NOP++)
unsigned char NOP;
sbit SDA=P0^2;
sbit SCL=P0^3;

xdata unsigned char SLA;
xdata unsigned char SLAW;
xdata unsigned char SLAR;
xdata unsigned char NUMBYT;
xdata unsigned char MTD[TWI_BUFFER_SIZE];
xdata unsigned char MRD[TWI_BUFFER_SIZE];
bit NACK;

void TwiStart(void)
{
	SDA = 1;
	SCL = 1;
	DELAY;
	SDA = 0;
	DELAY;
	SCL = 0;
}
void TwiStop(void)
{
	SDA = 0;
	SCL = 1;
	DELAY;
	SDA = 1;
	DELAY;
	SCL = 0;
}
void TwiAck(void)
{
	SDA = 0;
	SCL = 1;
	DELAY;
	SDA = 1;
	SCL = 0;
}
void TwiNotAck(void)
{
	SDA = 1;
	SCL = 1;
	DELAY;
	SDA = 0;
	SCL = 0;
}
