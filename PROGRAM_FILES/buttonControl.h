#include "type.h"
// Checks if current time matches any medicine schedule and triggers alert
void checkMedicineTime(s32 m1Min,s32 m1Hour,s32 m1Sec,
											 s32 m2Min,s32 m2Hour,s32 m2Sec,
											 s32 m3Min,s32 m3Hour,s32 m3Sec,
											 s32 hour,s32 min,s32 sec);
// Activates buzzer and runs LCD animation for medicine alert
void BuzzerOn(void);
// Opens edit menu to modify RTC and medicine timings
void editTime(s32 *m1Min,s32 *m1Hour,
							s32 *m2Min,s32 *m2Hour,
							s32 *m3Min,s32 *m3Hour);
