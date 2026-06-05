#include "type.h"

// Writes raw data/command to LCD
void WriteLCD(u8 data);

// Sends command to control LCD
void CmdLCD(u8 cmd);

// Displays a single character on LCD
void CharLCD(u8 ch);

// Initializes LCD module
void InitLCD(void);

// Displays a string on LCD
void StrLCD(u8 *str);

// Displays an integer on LCD
void IntLCD(u32 n);

// Loads custom characters into CGRAM
void BuildCGRAM(u8 *ptr,u32 n);
