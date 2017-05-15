
#include <C8051F330.H>
#include "uart.h"

/*********************************
 * SMBUS write/read command 
 ********************************/
#define  SMBUS_WRITE          0x00          
#define  SMBUS_READ           0x01
/*********************************
 * SMBUS busy or idle micro 
 ********************************/
#define SMBUS_IS_BUSY 1
#define SMBUS_IS_IDLE 0
/**************************************
 * SMBUS operation state 
 *************************************/
#define SMB_NOT_START           0
#define SMB_DATA_NOT_FINISHED   1
#define SMB_ALL_DATA_DONE       2
/**************************************
 * SMBUS write operation result
 *************************************/
#define SMB_OPERATION_SUCCESS  0
#define SMB_OPERATION_FAIL    -1
/*********************************
 *Status vector - top 4 bits only
 ********************************/
#define  SMB_MTSTA      0xE0           /* (MT) start transmitted     */
#define  SMB_MTDB       0xC0           /* (MT) data byte transmitted */
#define  SMB_MRDB       0x80           /* (MR) data byte received    */
#define SMB_WRITE_BUF_LEN 16

/******************************************************************************
 *Global VARIABLES
 *****************************************************************************/
/* Global holder for SMBus data     *
 * All receive data is written here */
volatile unsigned char smb_data_in;                                           

/* Global holder for SMBus data. *
 * SMBus slave address           */
volatile unsigned char slave_smbus_address;    
/* Software flag
 * indicate whether write or read operation is running. */                               
volatile unsigned char smb_busy = SMBUS_IS_IDLE;                          
/* Software flag                              
 * indicate the direction of the current transfer */
volatile unsigned char smb_rw = SMBUS_WRITE;    
/* Software flag                              
 * indicate the data stream end */
volatile unsigned char smb_state = SMB_NOT_START; 
/* Software flag                              
 * indicate the operation's result */
volatile signed char operation_result = SMB_OPERATION_FAIL;

struct SMB_WRITE_BUFF
{
	unsigned char buf[SMB_WRITE_BUF_LEN];
	unsigned char positon;
	unsigned char end;
};
xdata struct SMB_WRITE_BUFF write_buff = {{0},0,0};

/*SMBUS interrupt functions *
 *To start a transfer , set SMB0CN_STA to send a "start" outside this function*/
void SMBUS0_ISR(void) interrupt 7 using 1
{
	/* Used by the ISR to flag failed */
	unsigned char  fail = 0;                       
	unsigned char count = 0;

    /* Check for errors */
	if (ARBLOST == 0)                   
	{
		/* Normal operation */
		switch (SMB0CN & 0xF0)           
		{
			/* START condition transmitted */
			case SMB_MTSTA:		
				SMB0DAT = slave_smbus_address;         
				SMB0DAT &= 0xFE;                                                  
				SMB0DAT |= (unsigned char) smb_rw;
    
				STA = 0;           
				smb_state = SMB_DATA_NOT_FINISHED;
				break;

			/* Master Transmitter: Data byte transmitted*/
			case SMB_MTDB:
				if (ACK)    
				{
					if(smb_state == SMB_DATA_NOT_FINISHED)
					{
						if(smb_rw == SMBUS_WRITE)
						{
							SMB0DAT = write_buff.buf[write_buff.positon];
							write_buff.positon++;
							if(write_buff.positon >= write_buff.end)
							{
								smb_state = SMB_ALL_DATA_DONE;
							}
						}
						else
						{
							/*do nothing, wait to receive data*/
						}
					}
					else if(smb_state == SMB_ALL_DATA_DONE)
					{  
						/* Set SMB0CN_STO to terminate transfer */
						STO = 1;      
						/* And free SMBus interface */
						smb_busy = SMBUS_IS_IDLE;       
						smb_state = SMB_NOT_START;
						operation_result = SMB_OPERATION_SUCCESS;
						for(count = 0; count < write_buff.end; count++)
						{
							write_buff.buf[count] = 0;
						}
						write_buff.end = 0;
						write_buff.positon = 0;
					}
					else
					{
						STO = 1;               
						smb_busy = SMBUS_IS_IDLE;
						operation_result = SMB_OPERATION_FAIL;
					}

				}
				else  /* If slave NACK */
				{
					STO = 1;               
					smb_busy = SMBUS_IS_IDLE;
					operation_result = SMB_OPERATION_FAIL;
				}
				break;

			/* Master Receiver: byte received */
			case SMB_MRDB:
				/* Store received byte */
				smb_data_in = SMB0DAT;     
				smb_busy = SMBUS_IS_IDLE;
				operation_result = SMB_OPERATION_SUCCESS;
				ACK = 0;                   
				STO = 1;                   
				break;

			default:
				fail = 1;                  
				break;
		} /* switch(SMB0CN & 0xF0) */
	}
	else
	{
		/* SMB0CN_ARBLOST = 1, error occurred... abort transmission */
		fail = 1;
	} 	/* end SMB0CN_ARBLOST if */

	if (fail)                           
	{
		SMB0CF &= ~0x80;                 
		SMB0CF |= 0x80;
		STA = 0;
		STO = 0;
		ACK = 0;

		smb_busy = SMBUS_IS_IDLE;    
		operation_result = SMB_OPERATION_FAIL;
		fail = 0;
	}

	SI = 0;   /* Clear interrupt flag */                         
}

/* write to SMBus */
signed char smbusWrite(unsigned char slave_address,unsigned char *pbuff,unsigned char len)
{
	unsigned char count = 0;
	operation_result = SMB_OPERATION_FAIL;
	slave_smbus_address = slave_address;
	write_buff.end = len;
	for(count = 0; count < write_buff.end; count++)
	{
		write_buff.buf[count] = pbuff[count];
	}
	write_buff.positon = 0;
	while (smb_busy == SMBUS_IS_BUSY); /* Wait for SMBus to be free.*/
	smb_busy = SMBUS_IS_BUSY;          /* Claim SMBus (set to busy) */
	smb_rw = SMBUS_WRITE;              /* Mark this transfer as a WRITE */
	STA = 1;                    /* Start transfer */
	while(smb_busy == SMBUS_IS_BUSY);
	return operation_result;
}

signed char smbusRead(unsigned char slave_address, unsigned char *pvalue)
{
	slave_smbus_address = slave_address;
	operation_result = SMB_OPERATION_FAIL;
	while (smb_busy == SMBUS_IS_BUSY);  /* Wait for bus to be free.     */
	smb_busy = SMBUS_IS_BUSY;           /* Claim SMBus (set to busy)    */
	smb_rw = SMBUS_READ;                /* Mark this transfer as a READ */
	STA = 1;                     /* Start transfer               */
	while (smb_busy == SMBUS_IS_BUSY);  /* Wait for transfer to complete*/
	*pvalue = smb_data_in;
	return operation_result;
}
