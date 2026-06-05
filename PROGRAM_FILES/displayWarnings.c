#include "delay.h"      // Includes delay functions
#include "lcd.h"        // Includes LCD control functions
#include "string.h"     // Includes string handling functions

// Function to display empty input warning
void DisplayEmptyString()
{
	CmdLCD(0x01);                     // Clear LCD screen
	StrLCD("--ENTER VALUE--");        // Display message
	delay_ms(1000);                  // Wait for 1 second
	CmdLCD(0x01);                    // Clear LCD again
}

// Function to display invalid input warning
void DisplayWarning()
{
	CmdLCD(0x01);                    // Clear LCD screen
	StrLCD("INVALID!");              // Display invalid message
	delay_ms(1000);                  // Wait for 1 second
	CmdLCD(0x01);                    // Clear LCD again
}

// Function to guide user how to enter input
void DisplayHowtoEnter()
{
	CmdLCD(0x01);                    // Clear LCD screen
	StrLCD("ENTER NUM & THEN");      // Display first line instruction
	CmdLCD(0xc0);                   // Move cursor to second line
	StrLCD("PRESS '=' ");           // Display second line instruction
	delay_ms(1000);                 // Wait for 1 second
	CmdLCD(0x01);                   // Clear LCD again
}

// Function to scroll project title on LCD
void DisplayInfo()
{
    u8 str[] = "USER-CONFIGURABLE MEDICINE REMINDER SYSTEM"; // String to display
    s32 i, j, len = 0;                                      // Loop variables and length

    while(str[len] != '\0') len++;   // Calculate string length

    for(i = -15; i < len; i++)       // Loop for scrolling effect
    {
        CmdLCD(0x80);               // Set cursor to beginning of first line

        for(j = 0; j < 16; j++)     // Display 16 characters (LCD width)
        {
            s32 idx = i + j;        // Calculate current character index

            if(idx >= 0 && idx < len)
                CharLCD(str[idx]);  // Display valid character
            else
                CharLCD(' ');       // Fill with space if out of bounds
        }

        delay_ms(150);              // Delay for smooth scrolling
    }

    CmdLCD(0x01);                   // Clear LCD after scrolling ends
}

