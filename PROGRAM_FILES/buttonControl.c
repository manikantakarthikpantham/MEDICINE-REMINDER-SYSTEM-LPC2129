#include "type.h"
#include "lcd.h"
#include "kpm.h"
#include "displayWarnings.h"
#include "buttonControl.h"
#include <lpc21xx.h>
#include "interuptdefinesh.h"
#include "delay.h"
#include "RTC.h"
#include "medicine.h"
#include <stdlib.h>

extern u32 flag1;   // Flag to stop buzzer when medicine is taken
extern u32 flag2;   // Flag to enter edit mode

// Check if current RTC time matches any medicine time
void checkMedicineTime(s32 m1Min,s32 m1Hour,s32 m1Sec,
											 s32 m2Min,s32 m2Hour,s32 m2Sec,
											 s32 m3Min,s32 m3Hour,s32 m3Sec,
											 s32 hour,s32 min,s32 sec)
{
	if((m1Sec==sec) && (m1Hour==hour) && (m1Min==min))
	{
		if(flag1) return;   // Skip if already acknowledged
		BuzzerOn();         // Trigger alert
		return;
	}

	if((m2Hour==hour) && (m2Min==min) && (m2Sec==sec))
	{
		if(flag1) return;
		BuzzerOn();
		return;
	}
	
	if((m3Hour==hour) && (m3Min==min) && (m3Sec==sec))
	{
		if(flag1) return;
		BuzzerOn();
		return;
	}
	flag1 = 0;   // Reset flag when no match
}

// Custom characters for LCD animation (walking + icons)
u8 CGRAMLUT[40] = {
	0x0E,0x0E,0X04,0X06,0X05,0X0C,0X14,0X00,  // Frame 1
	0X0E,0X0E,0X04,0X0C,0X14,0X06,0X05,0X00,  // Frame 2
	0X0A,0X04,0X11,0X1B,0X1F,0X11,0X0E,0X00,  // Capsule
	0x0E,0X11,0X1F,0X1F,0X1F,0X11,0X0E,0X00,  // Filled icon
	0X11,0X0A,0X04,0X0A,0X11,0X00,0X00,0X00   // Cross/failed symbol
};

// Buzzer alert with LCD animation
void BuzzerOn()
{
	u32 i=0;
	s32 k=0;            // Moving position of animation
	u32 r = 15;       // Random target position
	u32 seed = 10;    // Seed for random movement

	CmdLCD(0x01);
	StrLCD("-MEDICINE TIME-");   // Alert message

	BuildCGRAM(CGRAMLUT,40);     // Load custom characters

	srand(seed);                 // Initialize random generator

	for(i = 0; i < 20; i++)      // Animation loop
	{
		if(flag1)   // If user acknowledged medicine
		{
			CmdLCD(0x01);
			StrLCD("-MEDICINE TAKEN-");

			CmdLCD(0xc0 + k);
			CharLCD(0);
			CharLCD(2);          // Show success animation

			delay_s(1);
			CmdLCD(0x01);
			return;
		}
		else
		{
			CmdLCD(0xc0 + k);

			// Move animation left or right
			if(r < k)
			{
				CharLCD(i%2 ? 1 : 0);   // Walking animation frames
				IOSET1 = 1<<BUZZ;       // Turn ON buzzer
				delay_ms(250);

				CmdLCD(0xc0 + k);
				CharLCD(32);            // Clear previous position
				--k;                    // Move left
			}
			else
			{
				CharLCD(i%2 ? 1 : 0);
				IOSET1 = 1<<BUZZ;
				delay_ms(250);

				CmdLCD(0xc0 + k);
				CharLCD(32);
				++k;                    // Move right
			}

			// Change random target position
			if(r == k)
				r = rand()%16 + 1;

			CmdLCD(0xc0 + r);
			CharLCD(3);   // Display target icon

			// Boundary handling for LCD (16 chars)
			if(k == 16) 
				k = 0;
			if(k < 0)   
				k = 16;
		}
	}

	IOCLR1 = 1<<BUZZ;   // Turn OFF buzzer

	CmdLCD(0x01);
	StrLCD("  ---FAILED---  ");   // Not acknowledged

	CmdLCD(0xc0 + k);
	CharLCD(1);
	CharLCD(4);          // Failure animation

	delay_ms(1000);
	CmdLCD(0x01);

	flag1 = 0;           // Reset acknowledgment flag
}

// Edit menu for RTC and Medicine timings
void editTime(s32 *m1Min,s32 *m1Hour,
							s32 *m2Min,s32 *m2Hour,
							s32 *m3Min,s32 *m3Hour)
{
	u32 x = 0;

	if(flag2)   // Enter edit mode only if flag is set
	{
		CmdLCD(0x01);

		while(1)
		{
			CmdLCD(0x80);
			StrLCD("1.RTC 3.EXIT");

			CmdLCD(0xC0);
			StrLCD("2.MEDICINE");

			x = ReadNUM();   // Read user choice

			switch(x)
			{
				case 1:
					RTC();   // Edit RTC time/date
					break;

				case 2:
					Medicine(m1Min,m1Hour,m2Min,m2Hour,m3Min,m3Hour); // Edit medicine timings
					break;

				case 3:
					CmdLCD(0x01);
					flag2 = 0;   // Exit edit mode
					return;

				default:
					DisplayWarning();   // Invalid option
					break;
			}
		}
	}
}
