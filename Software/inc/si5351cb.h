#ifndef __SI5351CB_H_
#define __SI5351CB_H_
#define SI5351_SUCCESS  0
#define SI5351_FAIL    -1

extern signed char si5351_write(unsigned char register_address,unsigned char value);
extern signed char si5351_read(unsigned char register_address,unsigned char *pvalue);

extern signed char is_si5351_init_done(void);
extern void si5351_switch_clk_source(void);
#endif 
