#include<lpc21xx.h>
#include "kpm_defines.h"
#include "type.h"
#include "lcd.h"
#include "delay.h"
#include "interupt.h"
#include<stdlib.h>

// Keypad lookup table (maps row & column to key value)
u8 kpmLUT[4][4]={{'7','8','9','/'},
								 {'4','5','6','*'},
								 {'1','2','3','-'},
								 {'C','0','=','+'}};

// Initialize keypad rows as output
void InitKPM()
{
	IODIR1 |= 0XF<<ROW0;
}

// Check if any key is pressed (column detection)
u32 colScan()
{
	if(((IOPIN1>>COL0)&0XF)<15)
		return 0;   // Key pressed
	else
		return 1;   // No key pressed
}

// Identify which row is active
u32 rowCheck()
{
	u32 rno;

	for(rno=0; rno<4; rno++)
	{
		IOPIN1 = (IOPIN1 & ~(0xF << ROW0)) | ((~(1 << rno) << ROW0) & (0xF << ROW0));

		if(colScan()==0)
			break;   // Row found
	}

	IOCLR1 = 0XF<<ROW0;
	return rno;
}

// Identify which column is active
u32 colCheck()
{
	int cno;

	for(cno=0; cno<4; cno++)
	{
		if(((IOPIN1>>(COL0+cno))&1)==0)
			break;   // Column found
	}

	return cno;
}

// Scan keypad and return pressed key
u32 keyScan()
{
	u32 rno,cno,keyVal;

	while(colScan());        // Wait until key press

	rno = rowCheck();        // Detect row
	cno = colCheck();        // Detect column

	keyVal = kpmLUT[rno][cno]; // Get key value

	while(!colScan());       // Wait until key release

	return keyVal;
}

// Read single digit number from keypad
u32 ReadNUM()
{
	u8 key = keyScan();

	if(key>='0' && key<='9')
		return (key-48);   // Convert ASCII to integer

	return 0;
}

// Read multi-digit number with display and validation
u32 ReadNUMWithDisp(u32 *k)
{
	u32 sum = 0;
	s32 c = 0;
	u8 key;
	u32 flag = 0;

	*k = 0;   // k = input status flag

	/*
	 * k meaning:
	 * k = 0 ? valid input
	 * k = 1 ? no input entered
	 * k = 2 ? wrong key pressed (not '=' after input)
	 */

	while(1)
	{
		key = keyScan();

		if(key>='0' && key<='9')   // If digit pressed
		{
			CmdLCD(0xc0+c);
			CharLCD(key);

			sum = sum*10 + (key-48); // Build number
			c++;
		}
		else if(key == '=' && c)   // End of input
		{
			flag = 1;
			break;
		}
		else if(key == 'C' && c)   // Backspace operation
		{
			sum /= 10;
			CmdLCD(0xc0+c-1);
			CharLCD(' ');

			if(c == 1)
				CmdLCD(0xc0);

			--c;
		}
		else   // Invalid or empty input
		{
			if(!flag && c)
			{
				*k = 2;   // Wrong input method
				return 0;
			}
			*k = 1;       // No input entered
			return 0;
		}
	}
	return sum;   // Return valid number
}
