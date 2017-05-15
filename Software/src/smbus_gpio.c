#include <C8051F330.H>
#include "uart.h"

#define DELAYTIME 50
#define DELAY for(NOP = 0; NOP < DELAYTIME; NOP++)
unsigned char NOP;
sbit SCL = 0x83;
sbit SDA = 0x82;

void TwiStart(void)
{
	SCL = 1;
	SDA = 1;
	DELAY;
	SDA = 0;
	DELAY;
	SCL = 0;
	DELAY;
}
void TwiStop(void)
{
	SDA = 0;
	SCL = 1;
	DELAY;
	SDA = 1;
	DELAY;
}
void TwiSendAck(void)
{
	SDA = 0;
	SCL = 0;
	DELAY;
	SCL = 1;
	DELAY;
}

unsigned char TwiReadAck(void)
{
	unsigned char ret;
	SCL = 0;
	SDA = 1;
	ret = SDA;
	DELAY;
	SCL = 1;
	DELAY;	
	return ret;
}

unsigned char TwiWriteByte(unsigned char value)
{
	char i;
	unsigned char ret;
	for(i = 7; i >= 0; i--)
	{
		SCL = 0;
		DELAY;
		SDA = (value & (1 << i));
        DELAY;
		SCL = 1;
		DELAY;
	}
	ret = TwiReadAck();
	return ret;
}

unsigned char TwiReadByte(void)
{
	unsigned char ret;
	char i;
	unsigned char tmp;

	for(i = 7; i >= 0; i--)
	{
		SCL = 0;
		DELAY;
		SDA = 1;
		tmp = SDA;
		ret = (ret << 1) | tmp;
		SCL = 1;
		DELAY;
	}
	TwiSendAck();
	return ret;
}

/* write to SMBus */
signed char smbusWrite(unsigned char slave_address,unsigned char *pbuff,unsigned char len)
{
	unsigned char i;
	unsigned char tmp;
	signed char ret;
	TwiStart();
	tmp = (slave_address) & 0xFE;
	ret = TwiWriteByte(tmp);
	if(ret == 0)
	{
	   for(i = 0; i < len; i++)
		{
			ret = TwiWriteByte(pbuff[i]);
			if(ret != 0)
			{
				break;
			}
		}
	}	
	TwiStop();
	return ret;
}

signed char smbusRead(unsigned char slave_address, unsigned char *pbuff, unsigned char len)
{
	unsigned char i;
	unsigned char tmp;
	unsigned char ret;
	TwiStart();
	tmp = (slave_address) | 1;
	ret = TwiWriteByte(tmp);
	for(i = 0; i < len; i++)
	{
		pbuff[i] = TwiReadByte();
	}
	TwiStop();
	return ret; 
}
