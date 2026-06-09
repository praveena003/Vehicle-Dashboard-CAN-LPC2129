#include <LPC21xx.H>
#include "delay.h"

#define LCD_D (0xff<<0)
#define RS (1<<8)
#define E  (1<<9)

void LCD_INIT(void);
void LCD_COMMAND(unsigned char);
void LCD_DATA(unsigned char);
void LCD_INTEGER(int);
void LCD_STR(unsigned char *);
void LCD_SCROLL(unsigned char *);
void LCD_FLOAT(float);

void LCD_INIT(void)
{
	IODIR0 |= LCD_D|RS|E;

	LCD_COMMAND(0x01);
	LCD_COMMAND(0x02);
	LCD_COMMAND(0x0C);
	LCD_COMMAND(0x38);
}

void LCD_COMMAND(unsigned char cmd)
{
	IOCLR0 = LCD_D;
	IOSET0 = cmd;
	IOCLR0 = RS;
	IOSET0 = E;
	delay_millisecond(2);
	IOCLR0 = E;
}

void LCD_DATA(unsigned char d)
{
	IOCLR0 = LCD_D;
	IOSET0 = d;
	IOSET0 = RS;
	IOSET0 = E;
	delay_millisecond(2);
	IOCLR0 = E;
}

void LCD_INTEGER(int n)
{
	unsigned char num[20];
	int i=0;

	if(n==0)
	{
		LCD_DATA('0');
		return;
	}

	if(n<0)
	{
		LCD_DATA('-');
		n=-n;
	}

	while(n>0)
	{
		num[i++]=n%10;
		n=n/10;
	}

	for(i=i-1;i>=0;i--)
	{
		LCD_DATA(num[i]+48);
	}
}

void LCD_FLOAT(float n)
{
	int temp=(int)n;
	LCD_INTEGER(temp);
	LCD_DATA('.');
	LCD_INTEGER((n-temp)*100);
}

void LCD_STR(unsigned char *d)
{
	while(*d!='\0')
	{
		LCD_DATA(*d++);
	}
}

void LCD_SCROLL(unsigned char *s)
{
	int len=0,i,j;

	while(s[len]!='\0')
		len++;

	for(i=0;i<=len;i++)
	{
		LCD_COMMAND(0x80);

		for(j=0;j<16;j++)
		{
			if((i+j)<len)
				LCD_DATA(s[i+j]);
			else
				LCD_DATA(' ');
		}

		delay_millisecond(300);
	}
}
