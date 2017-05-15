/////////////////////////////////////
//  Generated Initialization File  //
/////////////////////////////////////

#include <C8051F330.H>

// Peripheral specific initialization functions,
// Called from the Init_Device() function


void PCA_Init()
{
    PCA0MD    &= ~0x40;
    PCA0MD    = 0x00;
}

void Timer_Init()
{
    TCON      = 0x50;
    TMOD      = 0x22;
#if 1 //origin ok
    CKCON     = 0x04;
    TH0       = 0xAD;
    TH1       = 0x96;
    
   // TMR2CN    = 0x04;
   // TMR2RLL   = 0xFE;
   // TMR2RLH   = 0xFE;
#else
    CKCON     = 0x0C;
    TH0       = 0xF6;
    TH1       = 0x60;
    TMR2CN    = 0x04;
    TMR2RLL   = 0xB0;
    TMR2RLH   = 0x61;
#endif
}

void UART_Init()
{
    SCON0     = 0x10;
}

void SMBus_Init()
{
    SMB0CF    = 0x80;
}

void DAC_Init()
{
    IDA0CN    = 0xF2;
}

void Voltage_Reference_Init()
{
    REF0CN    = 0x08;
}

void Port_IO_Init()
{
    // P0.0  -  Skipped,     Open-Drain, Digital
    // P0.1  -  Skipped,     Open-Drain, Digital
    // P0.2  -  SDA (SMBus), Open-Drain, Digital
    // P0.3  -  SCL (SMBus), Open-Drain, Digital
    // P0.4  -  TX0 (UART0), Open-Drain, Digital
    // P0.5  -  RX0 (UART0), Open-Drain, Digital
    // P0.6  -  Unassigned,  Open-Drain, Digital
    // P0.7  -  Unassigned,  Push-Pull,  Digital

    // P1.0  -  Unassigned,  Open-Drain, Digital
    // P1.1  -  Unassigned,  Open-Drain, Digital
    // P1.2  -  Unassigned,  Open-Drain, Digital
    // P1.3  -  Unassigned,  Open-Drain, Digital
    // P1.4  -  Unassigned,  Open-Drain, Digital
    // P1.5  -  Unassigned,  Open-Drain, Digital
    // P1.6  -  Unassigned,  Open-Drain, Digital
    // P1.7  -  Unassigned,  Open-Drain, Digital

    P0MDOUT   = 0x80;
    P0SKIP    = 0x03;
    XBR0      = 0x05;
    XBR1      = 0xC0;
}

void Oscillator_Init()
{
#if 1
    OSCICN    = 0x83;
#else
    OSCICN    = 0x80;
#endif
}

void Interrupts_Init()
{
#if 1
    IE        = 0x10;
    EIE1      = 0x01;
	EIP1      = 0x01;
#else
    IE        = 0x30;
    EIE1      = 0x01;
    EIP1      = 0x01;
#endif
}

// Initialization function for device,
// Call Init_Device() from your main program
void Init_Device(void)
{
    PCA_Init();
    Timer_Init();
    UART_Init();
    SMBus_Init();
    DAC_Init();
    Voltage_Reference_Init();
    Port_IO_Init();
    Oscillator_Init();
    Interrupts_Init();
}
