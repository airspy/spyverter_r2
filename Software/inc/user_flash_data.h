#ifndef _USER_FLASH_DATA_H_
#define _USER_FLASH_DATA_H_

#include "F330_FlashPrimitives.h"



extern void userDataWrite(unsigned char register_num,unsigned char value);
extern unsigned char userDataRead(unsigned char register_num);

extern void resetUserDataFromFlash(void);

#endif