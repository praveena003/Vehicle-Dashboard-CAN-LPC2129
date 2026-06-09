/* can.h */

#include <LPC21xx.H>

typedef struct CAN2
{
	unsigned int id;
	unsigned int rtr;
	unsigned int dlc;
	unsigned int byteA;
	unsigned int byteB;
}CAN2_MSG;

/* function prototypes */
void can2_init(void);
void can2_tx(CAN2_MSG m1);
void can2_rx(CAN2_MSG *m1);

/* ---------------- CAN INIT ---------------- */
void can2_init(void)
{
	PINSEL1 |= 0x00014000;     /* P0.23 = RD2 , P0.24 = TD2 */

	VPBDIV = 1;               /* PCLK = CCLK */

	C2MOD = 0x01;            /* reset mode */
	AFMR  = 0x02;            /* accept all */

	C2BTR = 0x001C001D;      /* 125 kbps */

	C2MOD = 0x00;            /* normal mode */
}

/* ---------------- CAN TX ---------------- */
void can2_tx(CAN2_MSG m1)
{
	C2TID1 = m1.id;

	C2TFI1 = (m1.dlc << 16);

	if(m1.rtr == 0)
	{
		C2TFI1 &= ~(1<<30);   /* data frame */
		C2TDA1 = m1.byteA;
		C2TDB1 = m1.byteB;
	}
	else
	{
		C2TFI1 |= (1<<30);    /* remote frame */
	}

	C2CMR = (1<<0) | (1<<5);

	while((C2GSR & (1<<3)) == 0);
}

/* ---------------- CAN RX ---------------- */
void can2_rx(CAN2_MSG *m1)
{
	while((C2GSR & 1) == 0);

	m1->id  = C2RID;
	m1->dlc = (C2RFS >> 16) & 0x0F;
	m1->rtr = (C2RFS >> 30) & 0x01;

	if(m1->rtr == 0)
	{
		m1->byteA = C2RDA;
		m1->byteB = C2RDB;
	}

	C2CMR = (1<<2);          /* release buffer */
}
