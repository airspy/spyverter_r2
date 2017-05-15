#include <C8051F330.H>
#include "user_cmd.h"
#include "hw_config.h"
#include "smbus.h"
#include "uart.h"
#include "user_flash_data.h"
#include "si5351cb.h"


#define FAIL   (-1)
#define SUCCESS (0) 

volatile unsigned int system_tick = 0;
sbit c2d = P2^0;

void delay(void)
{
    unsigned char i,j;
    for(i = 200; i > 0; i--)
    {
        for(j = 200; j > 0; j--)
        {
            ;
        }
    }
}

unsigned char setup(void)
{
    unsigned char buff[2] = {0x01,0x55};
    Init_Device();
	EA =1;
    while(is_si5351_init_done() == FAIL);
	resetUserDataFromFlash();
    
    /*set port2.0 -> gpio input*/
    //P2MDOUT |= 0x01;
    if(c2d == 0)
    {
        delay();
        if(c2d == 0)
        {
            Uart0Write('W');
            Uart0Write('a');
            Uart0Write('k');
            Uart0Write('e');
            Uart0Write('M');
            Uart0Write('o');
            Uart0Write('d');
            Uart0Write('e');
            return 1;
        }
    }
    //P2MDOUT &= 0xFE;

    Uart0Write('S');
    Uart0Write('t');
    Uart0Write('o');
    Uart0Write('p');
    Uart0Write('M');
    Uart0Write('o');
    Uart0Write('d');
    Uart0Write('e');
    
    PCON |= 0x02;
    PCON = PCON;
    return 0;
}

void loop()
{
    signed char ret = FAIL;
    /*step1 : get the user_cmd by the cmd_start(0x55) & cmd_end(0xAA)*/
    ret = getUserCmd();
    if(ret == CMD_IS_PARSED)
    {		
        /*step2 : excute the user_cmd*/
		ret = excuteCmd();
		if(ret == SUCCESS)
		{
            sendSuccessToUser();
		}
		else
		{
	    	sendFailToUser();
		}
    }
	else if(ret == CMD_IS_RUNNING)	
	{
        sendBusyToUser();
	}
	else if(ret == CMD_IS_WRONG)
	{
        sendFailToUser();
	}
	else
	{
		
	}

#if 0
    if(TF2H == 1)
    {
        TF2H = 0;
        system_tick++;
        if(system_tick == 20000)
        {
            //Uart0Write(0xbb);
            //si5351_switch_clk_source();
            system_tick = 0;
        }
    }
#endif
}						

