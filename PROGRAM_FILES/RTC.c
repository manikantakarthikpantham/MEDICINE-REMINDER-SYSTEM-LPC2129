#include "type.h"
#include <lpc214x.h>
#include "lcdDefinesh.h"
#include "lcd.h"
#include "displayWarnings.h"
#include "kpm.h"

// System clock and peripheral clock Macros
#define FOSC 12000000              // Main oscillator frequency (12 MHz)
#define CCLK (5*FOSC)              // CPU clock (PLL multiplied)
#define PCLK (CCLK/4)              // Peripheral clock

// RTC Prescaler Calculation Macros
// RTC requires 32.768 kHz clock for 1-second increment.
// PREINT and PREFRAC divide PCLK to generate this frequency

#define PREINT_VAL (int)((PCLK / 32768) - 1)              // Integer part of division
#define PREFRAC_VAL (PCLK - ((PREINT_VAL + 1) * 32768))   // Fractional remainder

u8 str[][4] = {"SUN","MON","TUE","WED","THU","FRI","SAT"}; // Day names

// Initialize RTC
void InitRTC()
{
	CCR = 1<<1;        // Reset RTC counter
	PREINT = 456;      // Set integer prescaler value
	PREFRAC = 25024;   // Set fractional prescaler value
	CCR = 1<<0;        // Enable RTC
}

// Get current time from RTC registers
void GetRTCTimeInfo(s32 *hour, s32 *min, s32 *sec)
{
	*sec  = SEC;       // Read seconds
	*min  = MIN;       // Read minutes
	*hour = HOUR;      // Read hours
}

// Display time on LCD in HH:MM:SS format
void DisplayRTCTime(s32 hour, s32 min, s32 sec)
{
	CmdLCD(GOTO_LINE1_POS0); // Move cursor to line 1

	CharLCD(hour/10 + 48);   // Tens digit of hour
	CharLCD(hour%10 + 48);   // Units digit of hour
	CharLCD(':');

	CharLCD(min/10 + 48);    // Tens digit of minute
	CharLCD(min%10 + 48);    // Units digit of minute
	CharLCD(':');

	CharLCD(sec/10 + 48);    // Tens digit of second
	CharLCD(sec%10 + 48);    // Units digit of second
}

// Set RTC time
void SetRTCTimeInfo(s32 hour, s32 min, s32 sec)
{
	HOUR = hour;  // Set hour register
	MIN  = min;   // Set minute register
	SEC  = sec;   // Set second register
}

// Get date information from RTC
void GetRTCDateInfo(s32 *date, s32 *day, s32 *month, s32 *year)
{
	*date  = DOM;    // Day of month
	*day   = DOW;    // Day of week
	*month = MONTH;  // Month
	*year  = YEAR;   // Year
}

// Display date on LCD
void DisplayRTCDate(s32 date, s32 day, s32 month, s32 year)
{
	CmdLCD(0x80 + 10);   // Move cursor to line 1 (position 10)
	StrLCD(str[day]);    // Display day string (SUN, MON, etc.)

	CmdLCD(0xC0);        // Move to second line

	CharLCD(date/10 + 48);   // Date tens
	CharLCD(date%10 + 48);   // Date units
	CharLCD('/');

	CharLCD(month/10 + 48);  // Month tens
	CharLCD(month%10 + 48);  // Month units
	CharLCD('/');

	IntLCD(year);            // Display full year
}

// Set RTC date
void SetRTCDateInfo(s32 date, s32 day, s32 month, s32 year)
{
	DOM   = date;   // Set day of month
	DOW   = day;    // Set day of week
	MONTH = month;  // Set month
	YEAR  = year;   // Set year
}

// RTC configuration menu using keypad input
void RTC()
{
	s32 x;          // Stores user input value
	u32 k = 0;      // Status flag from keypad input function

	/*
	 * IMPORTANT ABOUT 'k':
	 * k is a status indicator returned via pointer from ReadNUMWithDisp()
	 *
	 * k == 0 ? Valid input entered correctly
	 * k == 1 ? Empty input (user pressed '=' without entering value)
	 * k == 2 ? Wrong input method (user didn’t follow proper steps)
	 *
	 * This helps us decide which warning message to show
	 */

	CmdLCD(0x01);   // Clear LCD

	while(1)
	{
		// Display menu options
		CmdLCD(0x80);
		StrLCD("1.H 2.M 3.S 4.D");

		CmdLCD(0xC0);
		StrLCD("5.D 6.M 7.Y 8.E");

		x = ReadNUM();   // Read user choice

		switch(x)
		{
			case 1:
				CmdLCD(0x01);

HOURINPUT:
				CmdLCD(0x80);
				StrLCD("HOURS [0-23] ");

				CmdLCD(0xC0);
				x = ReadNUMWithDisp(&k);   // Read input with display and status

				if(x < 0 || x > 23)       // Validate hour range
				{
					DisplayWarning();
					goto HOURINPUT;
				}

				if(k)                     // Handle special input conditions
				{
					if(k == 2)
						DisplayHowtoEnter();   // Wrong input method
					else
						DisplayEmptyString();  // Empty input

					goto HOURINPUT;
				}

				HOUR = x;                // Set hour
				CmdLCD(0x01);
				break;

			case 2:
				CmdLCD(0x01);

MININPUT:
				CmdLCD(0x80);
				StrLCD("MIN [0-59] ");

				CmdLCD(0xC0);
				x = ReadNUMWithDisp(&k);

				if(x < 0 || x > 59)
				{
					DisplayWarning();
					goto MININPUT;
				}

				if(k)
				{
					if(k == 2)
						DisplayHowtoEnter();
					else
						DisplayEmptyString();

					goto MININPUT;
				}

				MIN = x;
				CmdLCD(0x01);
				break;

			case 3:
				CmdLCD(0x01);

SECINPUT:
				CmdLCD(0x80);
				StrLCD("SEC [0-59] ");

				CmdLCD(0xC0);
				x = ReadNUMWithDisp(&k);

				if(x < 0 || x > 59)
				{
					DisplayWarning();
					goto SECINPUT;
				}

				if(k)
				{
					if(k == 2)
						DisplayHowtoEnter();
					else
						DisplayEmptyString();

					goto SECINPUT;
				}

				SEC = x;
				CmdLCD(0x01);
				break;

			case 4:
				CmdLCD(0x01);

DATEINPUT:
				CmdLCD(0x80);
				StrLCD("DATE [1-31] ");

				CmdLCD(0xC0);
				x = ReadNUMWithDisp(&k);

				if(k)
				{
					if(k == 2)
						DisplayHowtoEnter();
					else
						DisplayEmptyString();

					goto DATEINPUT;
				}

				if(x < 1 || x > 31)
				{
					DisplayWarning();
					goto DATEINPUT;
				}

				DOM = x;
				CmdLCD(0x01);
				break;

			case 5:
				CmdLCD(0x01);

DAYINPUT:
				CmdLCD(0x80);
				StrLCD("DAY [1-7] ");

				CmdLCD(0xC0);
				x = ReadNUMWithDisp(&k);

				if(k)
				{
					if(k == 2)
						DisplayHowtoEnter();
					else
						DisplayEmptyString();

					goto DAYINPUT;
				}

				if(x < 1 || x > 7)
				{
					DisplayWarning();
					goto DAYINPUT;
				}

				DOW = x - 1;   // Convert to 0–6 index
				CmdLCD(0x01);
				break;

			case 6:
				CmdLCD(0x01);

MONTHINPUT:
				CmdLCD(0x80);
				StrLCD("MONTH [1-12] ");

				CmdLCD(0xC0);
				x = ReadNUMWithDisp(&k);

				if(k)
				{
					if(k == 2)
						DisplayHowtoEnter();
					else
						DisplayEmptyString();

					goto MONTHINPUT;
				}

				if(x < 1 || x > 12)
				{
					DisplayWarning();
					goto MONTHINPUT;
				}

				MONTH = x;
				CmdLCD(0x01);
				break;

			case 7:
				CmdLCD(0x01);

YEARINPUT:
				CmdLCD(0x80);
				StrLCD("YEAR [2000-2035]");

				CmdLCD(0xC0);
				x = ReadNUMWithDisp(&k);

				if(k)
				{
					if(k == 2)
						DisplayHowtoEnter();
					else
						DisplayEmptyString();

					goto YEARINPUT;
				}

				if(x < 2000 || x > 2035)
				{
					DisplayWarning();
					goto YEARINPUT;
				}

				YEAR = x;
				CmdLCD(0x01);
				break;

			case 8:
				CmdLCD(0x01);
				return;   // Exit RTC menu

			default:
				DisplayWarning(); // Invalid menu choice
				break;
		}
	}
}
