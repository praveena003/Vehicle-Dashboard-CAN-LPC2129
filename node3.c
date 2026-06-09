```c
/* ---------- receiver2.c ---------- */
/* Fuel Node */

#include <LPC21xx.H>
#include "can.h"

#define CS        7
#define MSTR_BIT  5
#define Mode_3    0x18
#define SPIF_BIT  7

CAN2_MSG m1, m2;

unsigned char SPI0(unsigned char data);
float Read_ADC_MCP3204(unsigned char ch);

/* ---------- SPI INIT ---------- */

void Init_SPI0(void)
{
    PINSEL0 |= 0x00001500;      /* P0.4-SCK, P0.5-MISO, P0.6-MOSI */

    S0SPCCR = 60;
    S0SPCR  = (1 << MSTR_BIT) | Mode_3;

    IODIR0 |= (1 << CS);
    IOSET0  = (1 << CS);
}

/* ---------- SPI DATA ---------- */

unsigned char SPI0(unsigned char data)
{
    unsigned char stat;

    stat = S0SPSR;

    S0SPDR = data;

    while ((S0SPSR & (1 << SPIF_BIT)) == 0);

    return S0SPDR;
}

/* ---------- ADC READ ---------- */

float Read_ADC_MCP3204(unsigned char ch)
{
    unsigned char h, l;
    unsigned int val = 0;

    IOCLR0 = (1 << CS);

    SPI0(0x06);
    h = SPI0(ch << 6);
    l = SPI0(0x00);

    IOSET0 = (1 << CS);

    val = ((h & 0x0F) << 8) | l;

    return ((val * 3.3) / 4096.0);
}

/* ---------- MAIN ---------- */

int main()
{
    float volt;

    can2_init();
    Init_SPI0();

    while (1)
    {
        /* Read Fuel Sensor Voltage */
        volt = Read_ADC_MCP3204(0);

        /* Wait for CAN Request */
        can2_rx(&m1);

        if ((m1.id == 0x102) && (m1.rtr == 1))
        {
            m2.id    = 0x102;
            m2.rtr   = 0;
            m2.dlc   = 1;

            /* Send Voltage × 10 */
            m2.byteA = (unsigned int)(volt * 10);
            m2.byteB = 0;

            can2_tx(m2);
        }
    }
}
```
