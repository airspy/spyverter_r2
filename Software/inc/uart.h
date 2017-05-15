#ifndef __UART
#define __UART

extern unsigned char Uart0Available(void);
extern char Uart0Read(void);
extern void Uart0Write(char c);

#endif