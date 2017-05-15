#include <C8051F330.H>
#include "si5351cb.h"
#include "smbus.h"
#include <uart.h>
#include "user_flash_data.h"

#define SI5351_ADDRESS 0xC0
sbit Si5351_INT = P0^7;

signed char si5351_write(unsigned char register_address,unsigned char value)
{
	unsigned char buff[2] = {0};
	unsigned char address = SI5351_ADDRESS;
	signed char ret = SI5351_FAIL;
	buff[0] = register_address;
	buff[1] = value;
	ret = smbusWrite(address,buff,2);
	return ret;
}

signed char si5351_read(unsigned char register_address,unsigned char *pvalue)
{
	unsigned char si5351_address = SI5351_ADDRESS;
	unsigned char tmp = register_address;
	unsigned char value = 0;
	signed char ret = SI5351_FAIL;
	ret = smbusWrite(si5351_address,&tmp,1);
	if(ret == SI5351_SUCCESS)
	{
		ret = smbusRead(si5351_address,&value);
		if(ret == SI5351_SUCCESS)
		{
			*pvalue = value;
			return SI5351_SUCCESS;
		}
	}
	return SI5351_FAIL;
}


void si5351_switch_clk_source(void)
{
    signed char ret = -1;
    unsigned char value_register0 = 0x00;
    unsigned char value_register15 = 0x0f;
    unsigned char value_register29 = 0x1d;
    
    ret = si5351_read(0x00,&value_register0);
    if(ret == SI5351_SUCCESS)
    {
        if(value_register0 & 0x10)  // no CLKIN
        {            
            ret = si5351_read(0x1d,&value_register29);
            if(ret == SI5351_SUCCESS && value_register29 != 0x0d)
            {
                ret = si5351_write(0x1d,0x0d);
                userDataWrite(0x1d,0x0d);
            }
            
            ret = si5351_read(0x0f,&value_register15);
            if(ret == SI5351_SUCCESS && value_register15 != 0x00)
            {
                ret = si5351_write(0x0f,0x00);
                userDataWrite(0x0f,0x00);
            }
        }
        else  //CLKIN is ok 
        {
            
            ret = si5351_read(0x1d,&value_register29);
            if(ret == SI5351_SUCCESS && value_register29 != 0x22)
            {
                ret = si5351_write(0x1d,0x22);
                userDataWrite(0x1d,0x22);
            }
            
            ret = si5351_read(0x0f,&value_register15);
            if(ret == SI5351_SUCCESS && value_register15 != 0x04)
            {
                ret = si5351_write(0x0f,0x04);
                userDataWrite(0x0f,0x04);
            }
            
        }
    }
}

static void si5351_clean_interrupt_status(void)
{
    si5351_write(0x01,0x00);
}

signed char is_si5351_init_done(void)
{
    signed char ret = -1;
    unsigned char value_register0 = 0x00;
    Si5351_INT = 1;
    Uart0Write(0xA2);
	while(Si5351_INT == 1);
    ret = si5351_read(0x00,&value_register0);
    Uart0Write(0xA1);
    Uart0Write(ret);
    if(ret == SI5351_SUCCESS)
    {
        Uart0Write(value_register0);
        if(!(value_register0 & 0x80))
        {
            si5351_clean_interrupt_status();
            return SI5351_SUCCESS;
        }
    }
    return SI5351_FAIL;
}