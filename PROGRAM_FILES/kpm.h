#include "type.h"

void InitKPM(void);              // Init keypad
u32 colScan(void);               // Check key press
u32 rowCheck(void);              // Get row
u32 colCheck(void);              // Get column
u32 keyScan(void);               // Scan key
u32 ReadNUM(void);               // Read digit
u32 ReadNUMWithDisp(u32 *k);     // Read number with status
