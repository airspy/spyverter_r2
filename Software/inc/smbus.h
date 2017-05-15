#ifndef _SMBUS_H_
#define _SMBUS_H_
/**************************************************************************************************
 * function : smbusWrite
 * parameter: 
 *     slave_address  -->  7-bit, Right-align
 *     pbuff          -->  data buff to be sent
 *     len            -->  data number to be sent
 * return :
 *     0   -->   success
 *    -1   -->   fail
 *************************************************************************************************/
extern signed char smbusWrite(unsigned char slave_address,unsigned char *pbuff,unsigned char len);

/**************************************************************************************************
 * function : smbusRead
 * parameter: 
 *     slave_address  -->  7-bit, Right-align
 *     pvalue         -->  point of 1-byte buff to store the read data.
 * return :
 *     0   -->   success
 *    -1   -->   fail
 *************************************************************************************************/
extern signed char smbusRead(unsigned char slave_address, unsigned char *pvalue);

#endif
