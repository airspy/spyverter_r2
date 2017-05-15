#include "user_cmd.h"
#include "si5351cb.h"
#include "uart.h"
#include "iDAC.h"
#include "user_flash_data.h"

#define CMD_BIT_SI5351_READ     (1)
#define CMD_BIT_SI5351_WRITE    (2)
#define CMD_BIT_DAC_READ        (3)
#define CMD_BIT_DAC_WRITE       (4)
#define CMD_BIT_FAIL            (5)
#define CMD_BIT_SUCCESS         (6)
#define CMD_BIT_BUSY            (7)
#define CMD_BIT_SI5351_BACK     (8)
#define CMD_BIT_DAC_BACK        (9)

volatile unsigned char command[5]={0};
volatile unsigned char cmd_position = 0;

signed char getUserCmd(void)
{
    unsigned char ch = 0;
 
    if(Uart0Available() > 0)
    {
		ch = Uart0Read();
		switch(cmd_position)
		{
			case 0:
	    		if(ch == 0x55)
	    		{
					command[cmd_position] = ch;
					cmd_position++;
					return CMD_NOT_FINISHED;
	    		}
	    		else
	    		{
					return CMD_IS_WRONG;
	    		}
			case 1:
	    		if(ch == CMD_BIT_SI5351_READ  || 
				   ch == CMD_BIT_SI5351_WRITE ||
				   ch == CMD_BIT_DAC_READ     ||
				   ch == CMD_BIT_DAC_WRITE)
	    		{
					command[cmd_position] = ch;
					cmd_position++;
					return CMD_NOT_FINISHED;
	    		}
	    		else
	    		{
					return CMD_IS_WRONG;
	    		}
			case 2:
			case 3:
	    		command[cmd_position] = ch;
	    		cmd_position++;
	    		return CMD_NOT_FINISHED;
			case 4:
	    		if(ch == 0xAA)
	    		{
					command[cmd_position] = ch;
					cmd_position++;
					return CMD_IS_PARSED;
	    		}
	    		else
	    		{
					return CMD_IS_WRONG;
	    		}
			default:
	    		return CMD_IS_RUNNING;
		}/*switch(cmd_position)*/
    }/*if(Uart0Available() > 0)*/
	else
	{
		return CMD_NOT_FINISHED;
	}
}

signed char excuteCmd(void)
{
    unsigned char value = 0;
	unsigned char tmp = 0;
	unsigned char pidc[2] = {0};
	signed char ret = USER_CMD_FAIL;
    if(cmd_position > 4)
    {
		    if(command[1] == CMD_BIT_SI5351_WRITE)
		    {
			      ret = si5351_write(command[2],command[3]);
			      userDataWrite(command[2],command[3]);
		    }
		    else if(command[1] == CMD_BIT_SI5351_READ)
		    {
                  ret = si5351_read(command[2],&value);
			      if(ret == SI5351_SUCCESS)
			      {
						#if 0
				        tmp = userDataRead(command[2]);
				        if(tmp == value)
				        {
					          Uart0Write(0x55);
					          Uart0Write(CMD_BIT_SI5351_BACK);
					          Uart0Write(command[2]);
					          Uart0Write(value);
					          Uart0Write(0xAA);
				        }
				        else
				        {
					          ret = SI5351_FAIL;
				        }
				        #else
				        Uart0Write(0x55);
                        Uart0Write(CMD_BIT_SI5351_BACK);
                        Uart0Write(command[2]);
				        Uart0Write(value);
				        Uart0Write(0xAA);
				        #endif
			      }
		    }
		else if(command[1] == CMD_BIT_DAC_READ)
		{
			pidc[0] = userDataRead(0xfd);
			pidc[1] = userDataRead(0xfe);
			Uart0Write(0x55);
			Uart0Write(CMD_BIT_SI5351_BACK);
			Uart0Write(pidc[0]);
			Uart0Write(pidc[1]);
			Uart0Write(0xAA);
			ret = SI5351_SUCCESS;
		}
		else if(command[1] == CMD_BIT_DAC_WRITE)
		{
			idacUpdate(&command[2]);
			userDataWrite(0xfd,command[2]);
			userDataWrite(0xfe,command[3]);
			ret = SI5351_SUCCESS;
		}
    }
    return ret;
}

static void cleanCmdBuff(void)
{
	unsigned char count = 0;
	for(count = 0; count < 5; count++)
	{
		command[count] = 0;
	}
	cmd_position = 0;
}

void sendSuccessToUser(void)
{
    Uart0Write(0x55);
    Uart0Write(CMD_BIT_SUCCESS);
    Uart0Write(0x00);
    Uart0Write(0x00);
    Uart0Write(0xAA);
	cleanCmdBuff();
}

void sendBusyToUser(void)
{
    Uart0Write(0x55);
    Uart0Write(CMD_BIT_BUSY);
    Uart0Write(0x00);
    Uart0Write(0x00);
    Uart0Write(0xAA);
}

void sendFailToUser(void)
{
	unsigned char i = 250;
	while(i--);
	if(Uart0Available() == 0)
	{
    	Uart0Write(0x55);
    	Uart0Write(CMD_BIT_FAIL);
    	Uart0Write(0x00);
    	Uart0Write(0x00);
    	Uart0Write(0xAA);
	}
	cleanCmdBuff();
}

