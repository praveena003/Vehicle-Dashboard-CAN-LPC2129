```c
/* ---------- transmit.c ---------- */
/* Vehicle Dashboard Node */

#include <LPC21xx.H>
#include "display.h"
#include "can.h"

#define SW1 10        /* Temperature Switch */
#define SW2 11        /* Fuel Switch */

#define M1 16         /* Motor Control */
#define M2 17
#define LED 19        /* Fuel Warning LED (Active Low) */

int main()
{
    CAN2_MSG m1, m2;
    unsigned int v;

    LCD_INIT();
    can2_init();

    /* Motor and LED as Output */
    IODIR0 |= (1 << M1) | (1 << M2) | (1 << LED);

    /* Initial State */
    IOCLR0 = (1 << M1);
    IOCLR0 = (1 << M2);
    IOSET0 = (1 << LED);

    LCD_COMMAND(0x80);
    LCD_STR("VEHICLE");

    LCD_COMMAND(0xC0);
    LCD_STR("DASHBOARD");

    delay_second(2);

    LCD_COMMAND(0x01);

    while (1)
    {
        /* ---------- Temperature Node ---------- */

        if (((IOPIN0 >> SW1) & 1) == 0)
        {
            delay_millisecond(200);

            m1.id    = 0x101;
            m1.rtr   = 1;          /* Remote Frame */
            m1.dlc   = 1;
            m1.byteA = 0;
            m1.byteB = 0;

            can2_tx(m1);
            can2_rx(&m2);

            if ((m2.id == 0x101) && (m2.rtr == 0))
            {
                LCD_COMMAND(0x01);

                LCD_COMMAND(0x80);
                LCD_STR("TEMP:");

                LCD_INTEGER(m2.byteA);

                LCD_DATA(0xDF);
                LCD_DATA('C');

                if (m2.byteA > 35)
                {
                    /* Motor ON */
                    IOSET0 = (1 << M1);
                    IOCLR0 = (1 << M2);
                }
                else
                {
                    /* Motor OFF */
                    IOCLR0 = (1 << M1);
                    IOCLR0 = (1 << M2);
                }
            }

            while (((IOPIN0 >> SW1) & 1) == 0);
        }

        /* ---------- Fuel Node ---------- */

        if (((IOPIN0 >> SW2) & 1) == 0)
        {
            delay_millisecond(200);

            m1.id    = 0x102;
            m1.rtr   = 1;          /* Remote Frame */
            m1.dlc   = 1;
            m1.byteA = 0;
            m1.byteB = 0;

            can2_tx(m1);
            can2_rx(&m2);

            if ((m2.id == 0x102) && (m2.rtr == 0))
            {
                v = m2.byteA;

                LCD_COMMAND(0x01);

                LCD_COMMAND(0x80);

                if (v < 10)
                {
                    LCD_STR("FUEL:LOW");
                    IOCLR0 = (1 << LED);    /* LED ON */
                }
                else if (v < 25)
                {
                    LCD_STR("FUEL:MEDIUM");
                    IOSET0 = (1 << LED);    /* LED OFF */
                }
                else
                {
                    LCD_STR("FUEL:HIGH");
                    IOSET0 = (1 << LED);    /* LED OFF */
                }

                LCD_COMMAND(0xC0);
                LCD_STR("VOLT:");

                LCD_INTEGER(v / 10);
                LCD_DATA('.');
                LCD_INTEGER(v % 10);
                LCD_DATA('V');
            }

            while (((IOPIN0 >> SW2) & 1) == 0);
        }
    }
}
```
