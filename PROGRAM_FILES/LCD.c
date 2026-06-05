/* LCD.c*/
#include "lcdDefinesh.h"
#include "delay.h"
#include "type.h"
#include <lpc21xx.h>

// Sends raw data/command to LCD data pins
void WriteLCD(u8 data)
{
	IOCLR0 = 1<<RW;   // Set write mode
	IOPIN0 = (IOPIN0 & ~(0XFF<<LCD)) | (data<<LCD); // Place data on LCD data lines
	IOSET0 = 1<<EN;   // Enable pulse HIGH
	delay_us(1);
	IOCLR0 = 1<<EN;   // Enable pulse LOW
	delay_ms(2);      // Wait for LCD to process
}

// Sends command to LCD (RS = 0)
void CmdLCD(u8 cmd)
{
	IOCLR0 = 1<<RS;   // Select command register
	WriteLCD(cmd);
}

// Sends character data to LCD (RS = 1)
void CharLCD(u8 ch)
{
	IOSET0 = 1<<RS;   // Select data register
	WriteLCD(ch);
}

// Initializes LCD in 8-bit mode with required settings
void InitLCD()
{
	IODIR0 |= 0XFF<<LCD; // Configure data pins as output
	IODIR0 |= 1<<RW;     // Configure RW pin as output
	IODIR0 |= 1<<RS;     // Configure RS pin as output
	IODIR0 |= 1<<EN;     // Configure EN pin as output
	
	delay_ms(15);        // Initial power-on delay

	CmdLCD(_8BIT_LINE1); // 001DL NF00
	delay_ms(5);
	CmdLCD(_8BIT_LINE1);
	delay_us(100);
	CmdLCD(_8BIT_LINE1);

	CmdLCD(_8BIT_LINE2); // Set 2-line mode            
	CmdLCD(DISP_ON);     // 0000 1DCB ? Display ON
	CmdLCD(CLEAR_LCD);   // 0000 0001 ? Clear display
	CmdLCD(SHIFT_CUR_RIGHT); // 0000 01I/DS ? Cursor move right

//	CmdLCD(0x10);
}

// Displays a string on LCD
void StrLCD(u8 *str)
{
	while(*str)
	{
		CharLCD(*str++);   // Send each character
	}
}

// Displays integer number on LCD
void IntLCD(u32 n)
{
	u8 num[10];
	s32 i = 0;
	if(n == 0)
		CharLCD('0');
	while(n)   // Convert number to digits (reverse order)
	{
		num[i] = (n%10) + 48;
		n /= 10;
		i++;
	}
	for(--i; i >= 0; --i)   // Display digits in correct order
	{
		CharLCD(num[i]);
	}
}

// Loads custom characters into CGRAM
void BuildCGRAM(u8 *ptr,u32 n)
{
	u32 i;
	CmdLCD(0x40);   // Set CGRAM address
	for(i=0; i<n; i++)
	{
		CharLCD(ptr[i]);   // Write custom pattern data
	}
}
