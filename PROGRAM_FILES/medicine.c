#include<lpc21xx.h>
#include "interuptdefinesh.h"
#include "interupt.h"
#include "lcd.h"
#include "delay.h"
#include "kpm.h"
#include "RTC.h"
#include "displayWarnings.h"
#include "medicine.h"

// Main medicine menu to select which medicine to configure
void Medicine(s32 *m1Min,s32 *m1Hour,s32 *m2Min,s32 *m2Hour,s32 *m3Min,s32 *m3Hour)
{
	s32 x = 0;

	CmdLCD(0x01);

	while(1)
	{
		CmdLCD(0x80);
		StrLCD("1.M1  2.M2  3.M3");

		CmdLCD(0xc0);
		StrLCD("4.EXIT");

		x = ReadNUM();   // Read user option

		switch(x)
		{
			case 1: Medicine_1(m1Hour,m1Min); 				// Configure Medicine 1
							break;   
			case 2: Medicine_2(m2Hour,m2Min); 			  // Configure Medicine 2
							break;
			case 3: Medicine_3(m3Hour,m3Min); 		    // Configure Medicine 3
							break;
			case 4:
							CmdLCD(0x01);
							x = 0;
							return;   // Exit menu
			default:
							DisplayWarning();   // Invalid input
							break;
		}
	}
}

// Menu for Medicine 1 settings
void Medicine_1(s32 *m1Hour,s32 *m1Min)
{
	s32 y = 0;

	CmdLCD(0x01);

	while(1)
	{
		CmdLCD(0x80);
		StrLCD("MEDICINE 1 : ");

		CmdLCD(0xc0);
		StrLCD("1.H 2.M 3.D 4.E");

		y = ReadNUM();   // Read option

		if(y == 1)
			editMedicineHour(m1Hour,m1Min,1);   // Edit hour
		else if(y == 2)
			editMedicineMin(m1Hour,m1Min,1);    // Edit minute
		else if(y == 3)
			DisplayMedicineTime(*m1Hour,*m1Min,1); // Display time
		else if(y == 4)
		{
			CmdLCD(0x01);
			return;   // Exit
		}
		else
			DisplayWarning();
	}
}

// Menu for Medicine 2 settings
void Medicine_2(s32 *m2Hour,s32 *m2Min)
{
	s32 y = 0;

	CmdLCD(0x01);

	while(1)
	{
		CmdLCD(0x80);
		StrLCD("MEDICINE 2 : ");

		CmdLCD(0xc0);
		StrLCD("1.H 2.M 3.D 4.E");

		y = ReadNUM();

		if(y == 1)
			editMedicineHour(m2Hour,m2Min,2);
		else if(y == 2)
			editMedicineMin(m2Hour,m2Min,2);
		else if(y == 3)
			DisplayMedicineTime(*m2Hour,*m2Min,2);
		else if(y == 4)
		{
			CmdLCD(0x01);
			return;
		}
		else
			DisplayWarning();
	}
}

// Menu for Medicine 3 settings
void Medicine_3(s32 *m3Hour,s32 *m3Min)
{
	s32 y = 0;

	CmdLCD(0x01);

	while(1)
	{
		CmdLCD(0x80);
		StrLCD("MEDICINE 3 : ");

		CmdLCD(0xc0);
		StrLCD("1.H 2.M 3.D 4.E");

		y = ReadNUM();

		if(y == 1)
			editMedicineHour(m3Hour,m3Min,3);
		else if(y == 2)
			editMedicineMin(m3Hour,m3Min,3);
		else if(y == 3)
			DisplayMedicineTime(*m3Hour,*m3Min,3);
		else if(y == 4)
		{
			CmdLCD(0x01);
			return;
		}
		else
			DisplayWarning();
	}
}

// Edit medicine hour with validation
void editMedicineHour(s32 *hour,s32 *min,s32 n)
{
	s32 y = 0;
	u32 k = 0;   // Input status flag: 0=valid, 1=empty input, 2=wrong entry method

HOURINPUT:
	CmdLCD(0x01);

	CharLCD('M');
	IntLCD(n);
	StrLCD(".HOUR [0-23] ");

	y = ReadNUMWithDisp(&k);   // Read number and update status in k

	if(y < 0 || y > 23)
	{
		DisplayWarning();
		goto HOURINPUT;
	}

	if(k)   // Handle input errors using status flag
	{
		if(k == 2)
			DisplayHowtoEnter();   // Wrong input format
		else
			DisplayEmptyString();  // No input entered

		goto HOURINPUT;
	}

	*hour = y;   // Store valid hour

	CmdLCD(0x01);
	StrLCD("MEDICINE");
	IntLCD(n);

	CmdLCD(0xc0);
	DisplayonlyMedicineTime(*hour,*min);

	StrLCD("  EDITED");
	delay_ms(1000);

	CmdLCD(0x01);
}

// Edit medicine minute with validation
void editMedicineMin(s32 *hour,s32 *min,s32 n)
{
	s32 y = 0;
	u32 k = 0;   // Input status flag: controls validation and error handling

MININPUT:
	CmdLCD(0x01);

	CharLCD('M');
	IntLCD(n);
	StrLCD(".MIN [0-59] ");

	y = ReadNUMWithDisp(&k);

	if(y < 0 || y > 59)
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

	*min = y;   // Store valid minute

	CmdLCD(0x01);
	StrLCD("MEDICINE ");
	IntLCD(n);

	CmdLCD(0xc0);
	DisplayonlyMedicineTime(*hour,*min);

	StrLCD("  EDITED");
	delay_ms(1000);

	CmdLCD(0x01);
}

// Display medicine time with label
void DisplayMedicineTime(s32 hour,s32 min,s32 n)
{
	CmdLCD(0x01);

	StrLCD("MEDICINE ");
	IntLCD(n);

	CmdLCD(0xc0);
	DisplayonlyMedicineTime(hour,min);

	delay_ms(1000);
	CmdLCD(0x01);
}

// Display only HH:MM format
void DisplayonlyMedicineTime(s32 Hour,s32 Min)
{
	CharLCD(Hour/10 + 48);
	CharLCD(Hour%10 + 48);
	CharLCD(':');
	CharLCD(Min/10 + 48);
	CharLCD(Min%10 + 48);
}

// Set all medicine times sequentially
void setMedicineTime(s32 *m1Min,s32 *m1Hour,
											s32 *m2Min,s32 *m2Hour,
											s32 *m3Min,s32 *m3Hour)
{
	CmdLCD(0x01);
	StrLCD(" -SET MEDICINE-");

	CmdLCD(0xc0);
	StrLCD("     TIMES     ");

	delay_ms(1000);
	CmdLCD(0x01);

	// Configure all medicines one by one
	editMedicineHour(m1Hour,m1Min,1);
	editMedicineMin(m1Hour,m1Min,1);

	editMedicineHour(m2Hour,m2Min,2);
	editMedicineMin(m2Hour,m2Min,2);

	editMedicineHour(m3Hour,m3Min,3);
	editMedicineMin(m3Hour,m3Min,3);
}

