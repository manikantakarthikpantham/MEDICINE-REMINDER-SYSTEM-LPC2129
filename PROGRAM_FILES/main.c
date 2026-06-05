/*
-----------------------------------------------------------------------
Project Name : USER-CONFIGURABLE MEDICINE REMINDER SYSTEM
Controller   : LPC2129 (ARM7)
-----------------------------------------------------------------------

Pin Configuration
-----------------------------------------------------------------------

LCD Interface (16x2 LCD)
------------------------
Data Bus  : P0.8  – P0.15   -> LCD Data Pins (D0–D7)
RS        : P0.16           -> Register Select
RW        : P0.17           -> Read/Write control
EN        : P0.18           -> Enable signal

Keypad Matrix (4x4)
-------------------
Rows     : P1.16 – P1.19    -> Keypad Row Lines
Columns  : P1.20 – P1.23    -> Keypad Column Lines

External Interrupts
-------------------
EINT0 : P0.1
Used to stop the buzzer when a medicine reminder alarm occurs.

EINT1 : P0.3
Used to enter edit mode for modifying RTC time and medicine reminder
timings.

Buzzer
------
P1.24 -> Buzzer output used for medicine alert indication.

-----------------------------------------------------------------------
Software Modules
-----------------------------------------------------------------------

RTC Module
Maintains real-time clock using LPC2129 internal RTC.

checkMedicineTime()
Compares RTC current time with stored medicine reminder times.
If time matches, buzzer is activated.

editTime()
Allows the user to modify RTC time and medicine reminder times
using keypad input when edit mode is triggered.

-----------------------------------------------------------------------
Medicine Reminder Slots
-----------------------------------------------------------------------

Medicine 1 -> m1Hour, m1Min, m1Sec
Medicine 2 -> m2Hour, m2Min, m2Sec
Medicine 3 -> m3Hour, m3Min, m3Sec

The system supports three configurable medicine reminders.
-----------------------------------------------------------------------
*/

#include <LPC21xx.H>
#include "buttonControl.h"
#include "type.h"
#include "delay.h"
#include "lcd.h"
#include "RTC.h"
#include "interupt.h"
#include "kpm.h"
#include "medicine.h"
#include "displayWarnings.h"

/* Variables to store current RTC time and date */
s32 hour,min,sec,date,month,year,day;

/* Variables to store medicine reminder times */
s32 m1Hour=2,m1Min=0,m1Sec=0;
s32 m2Hour=0,m2Min=0,m2Sec=0;
s32 m3Hour=0,m3Min=0,m3Sec=0;

/* Interrupt flags used for event handling */
volatile u32 flag1=0;
volatile u32 flag2=0;

int main()
{
	/* Initialize LCD module */
	InitLCD();

	/* Initialize Real Time Clock */
	InitRTC();

	/* Initialize Keypad Matrix */
	InitKPM();

	/* Initialize External Interrupts */
	InitINTERUPT();
	//DisplayInfo();
	
	/* Set initial RTC time (Hour, Minute, Second) */
	SetRTCTimeInfo(1,59,58);

	/* Set initial RTC date (Date, Day, Month, Year) */
	SetRTCDateInfo(8,0,3,2026);
	
	//DisplayInfo();

	while(1)
	{
		/* Get current time from RTC */
		GetRTCTimeInfo(&hour,&min,&sec);

		/* Display current time on LCD */
		DisplayRTCTime(hour,min,sec);

		/* Get current date from RTC */
		GetRTCDateInfo(&date,&day,&month,&year);

		/* Display current date on LCD */
		DisplayRTCDate(date,day,month,year);

		/* Compare RTC time with medicine reminder times */
		checkMedicineTime(m1Min,m1Hour,m1Sec,
						  m2Min,m2Hour,m2Sec,
						  m3Min,m3Hour,m3Sec,
						  hour,min,sec);

		/* Edit RTC time or medicine reminder times */
		editTime(&m1Min,&m1Hour,&m2Min,&m2Hour,&m3Min,&m3Hour);
	}
}
//DisplayInfo();
//DisplayRun();
//setMedicineTime(&m1Min,&m1Hour,&m2Min,&m2Hour,&m3Min,&m3Hour);
