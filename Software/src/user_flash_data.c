#include "F330_FlashPrimitives.h"
#include "user_flash_data.h"
#include "si5351cb.h"
#include "iDAC.h"
#include "uart.h"

#define BASE_ADDRESS_0 0x01a00L
#define FLASH_WRITE_FLAG_ADD 0x01affL
#define WRITE_DONE 0x55

#define OUTPUT_120Mhz 1

void userDataWrite(unsigned char register_num,unsigned char value)
{
	unsigned char buff[21] = {0};
	unsigned char count = 0;	 
    unsigned char flag = 1;

	for(count = 0; count < 21; count++)
	{
		buff[count] =  FLASH_ByteRead(count + BASE_ADDRESS_0);
	}

	switch(register_num)
	{
		case 0x03:
			buff[0] = value;
			break;
		case 0x10:
			buff[1] = value;
			break;
		case 0x1a:
			buff[2] = value;
			break;
		case 0x1b:
			buff[3] = value;
			break;
		case 0x1c:
			buff[4] = value;
			break;
		case 0x1d:
			buff[5] = value;
            flag = 0;
			break;
		case 0x1e:
			buff[6] = value;
			break;
		case 0x1f:
			buff[7] = value;
			break;
		case 0x20:
			buff[8] = value;
			break;
		case 0x21:
			buff[9] = value;
			break;
		case 0x2a:
			buff[10] = value;
			break;
		case 0x2b:
			buff[11] = value;
			break;
		case 0x2c:
			buff[12] = value;
			break;
		case 0x2d:
			buff[13] = value;
			break;
		case 0x2e:
			buff[14] = value;
			break;
		case 0x2f:
			buff[15] = value;
			break;
		case 0x30:
			buff[16] = value;
			break;
		case 0x31:
			buff[17] = value;
			break;
        case 0x0f:
            buff[18] = value;
            flag = 0;
            break;
		case 0xfd:
			buff[19] = value;
			break;
		case 0xfe:
			buff[20] = value;
			break;
		default:
            flag = 0;
			break;
	}
    
    if(flag == 1)
    {
        FLASH_PageErase(BASE_ADDRESS_0);

        for(count = 0; count < 21; count++)
        {
            FLASH_ByteWrite(count + BASE_ADDRESS_0, buff[count]);
        }
        FLASH_ByteWrite(FLASH_WRITE_FLAG_ADD, WRITE_DONE);
    }
}


unsigned char userDataRead(unsigned char register_num)
{
	unsigned char ret;
	
	switch(register_num)
	{
		case 0x03:
			ret = FLASH_ByteRead(BASE_ADDRESS_0 + 0);
			break;
		case 0x10:
			ret = FLASH_ByteRead(BASE_ADDRESS_0 + 1);
			break;
		case 0x1a:
			ret = FLASH_ByteRead(BASE_ADDRESS_0 + 2);
			break;
		case 0x1b:
			ret = FLASH_ByteRead(BASE_ADDRESS_0 + 3);
			break;
		case 0x1c:
			ret = FLASH_ByteRead(BASE_ADDRESS_0 + 4);
			break;
		case 0x1d:
			ret = FLASH_ByteRead(BASE_ADDRESS_0 + 5);
			break;
		case 0x1e:
			ret = FLASH_ByteRead(BASE_ADDRESS_0 + 6);
			break;
		case 0x1f:
			ret = FLASH_ByteRead(BASE_ADDRESS_0 + 7);
			break;
		case 0x20:
			ret = FLASH_ByteRead(BASE_ADDRESS_0 + 8);
			break;
		case 0x21:
			ret = FLASH_ByteRead(BASE_ADDRESS_0 + 9);
			break;
		case 0x2a:
			ret = FLASH_ByteRead(BASE_ADDRESS_0 + 10);
			break;
		case 0x2b:
			ret = FLASH_ByteRead(BASE_ADDRESS_0 + 11);
			break;
		case 0x2c:
			ret = FLASH_ByteRead(BASE_ADDRESS_0 + 12);
			break;
		case 0x2d:
			ret = FLASH_ByteRead(BASE_ADDRESS_0 + 13);
			break;
		case 0x2e:
			ret = FLASH_ByteRead(BASE_ADDRESS_0 + 14);
			break;
		case 0x2f:
			ret = FLASH_ByteRead(BASE_ADDRESS_0 + 15);
			break;
		case 0x30:
			ret = FLASH_ByteRead(BASE_ADDRESS_0 + 16);
			break;
		case 0x31:
			ret = FLASH_ByteRead(BASE_ADDRESS_0 + 17);
			break;
        case 0x0f:
            ret = FLASH_ByteRead(BASE_ADDRESS_0 + 18);
            break;
		case 0xfd:
			ret = FLASH_ByteRead(BASE_ADDRESS_0 + 19);
			break;
		case 0xfe:
			ret = FLASH_ByteRead(BASE_ADDRESS_0 + 20);
			break;
		default:
			break;
	}
	
	return ret;
}



void resetUserDataFromFlash(void)
{
    unsigned char count = 0;
	unsigned char tmp = 0;
    unsigned char first_time_flag = 0;
	unsigned char pidc[2] = {0};
    unsigned char buff[21] = {0};
		
	signed char ret = -1;
    unsigned char value_register0 = 0x00;
    unsigned char value_register15 = 0x0f;
    
    first_time_flag = FLASH_ByteRead(FLASH_WRITE_FLAG_ADD); 
    if(first_time_flag == WRITE_DONE)
    {
        for(count = 0; count < 21; count++)
        {
            buff[count] =  FLASH_ByteRead(count + BASE_ADDRESS_0);
        }
    }
    else
    {
        //24Mhz -> 120MHz 
        buff[0] = 0xfe;  //register 0x03
        buff[1] = 0x4f;  //register 0x10
        buff[2] = 0x00;  //register 0x1a
        buff[3] = 0x01;  //register 0x1b
        buff[4] = 0x00;  //register 0x1c
        buff[5] = 0x0d;  //register 0x1d
        buff[6] = 0x00;  //register 0x1e
        buff[7] = 0x00;  //register 0x1f
        buff[8] = 0x00;  //register 0x20
        buff[9] = 0x00;  //register 0x21
        buff[10] = 0x00;  //register 0x2a
        buff[11] = 0x01;  //register 0x2b
        buff[12] = 0x00;  //register 0x2c
        buff[13] = 0x01;  //register 0x2d
        buff[14] = 0x00;  //register 0x2e
        buff[15] = 0x00;  //register 0x2f
        buff[16] = 0x00;  //register 0x30
        buff[17] = 0x00;  //register 0x31
        buff[18] = 0x00;  //register 0x0f
        buff[19] = 0x00;  //register 0xfd
        buff[20] = 0x74;  //register 0xfe
    } 
    /*check clkin source*/
    ret = si5351_read(0x00,&value_register0);
    if(ret == SI5351_SUCCESS)
    {
        if(value_register0 & 0x10)  // no CLKIN
        {   
			//24Mhz -> 120MHz
            if(buff[5] != 0x0d)
            {
                buff[5] = 0x0d;  //register 0x1d
                buff[18] = 0x00;
            }
			
        }
        else  //CLKIN is ok 
        {
            //10Mhz -> 120MHz 
            if(buff[5] != 0x22)
            {
                buff[5] = 0x22;  //register 0x1d
                buff[18] = 0x04;
            }
			
        }
    }
	/*chedk clkin source end*/	
    
    if(first_time_flag != WRITE_DONE)
    {
        FLASH_PageErase(BASE_ADDRESS_0);

        for(count = 0; count < 21; count++)
        {
            if(count != 5 && count != 18)
            {
                FLASH_ByteWrite(count + BASE_ADDRESS_0, buff[count]);
            }                
        }
        FLASH_ByteWrite(FLASH_WRITE_FLAG_ADD, WRITE_DONE);
    }
    
    for(count = 0; count < 19; count++)
    {
        tmp = buff[count];
        switch(count)
        {
			case 0:
				si5351_write(0x03,tmp);
				break;
			case 1:
				si5351_write(0x10,tmp);
				break;
			case 2:
				si5351_write(0x1a,tmp);
				break;
			case 3:
				si5351_write(0x1b,tmp);
				break;
			case 4:
				si5351_write(0x1c,tmp);
				break;
			case 5:
				si5351_write(0x1d,tmp);
				break;
			case 6:
				si5351_write(0x1e,tmp);
				break;
			case 7:
				si5351_write(0x1f,tmp);
				break;
			case 8:
				si5351_write(0x20,tmp);
				break;
			case 9:
				si5351_write(0x21,tmp);
				break;
			case 10:
				si5351_write(0x2a,tmp);
				break;
			case 11:
				si5351_write(0x2b,tmp);
				break;
			case 12:
				si5351_write(0x2c,tmp);
				break;
			case 13:
				si5351_write(0x2d,tmp);
				break;
			case 14:
				si5351_write(0x2e,tmp);
				break;
			case 15:
				si5351_write(0x2f,tmp);
				break;
			case 16:
				si5351_write(0x30,tmp);
				break;
			case 17:
				si5351_write(0x31,tmp);
				break;
      case 18:
				si5351_write(0x0f,tmp);
				break;
			default:
				break;
        }
    }

    pidc[0] = buff[19];
    pidc[1]	= buff[20];

    idacUpdate(pidc);
}