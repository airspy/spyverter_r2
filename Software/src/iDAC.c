#include "iDAC.h"

#include <C8051F330.H>

void idacUpdate(unsigned char *pvalue)
{
	unsigned char tmp_h = (pvalue[0] << 6) | (pvalue[1] >> 2);
	unsigned char tmp_l = (pvalue[1] << 6);


	IDA0L = tmp_l;
	IDA0H = tmp_h;
}
