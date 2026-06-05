#include "type.h"
void InitRTC(void);
void GetRTCTimeInfo(s32 *hour,s32 *min,s32 *sec);
void DisplayRTCTime(s32 hour,s32 min,s32 sec);
void SetRTCTimeInfo(s32,s32,s32);
void GetRTCDateInfo(s32 *date,s32 *day,s32 *month,s32 *year);
void DisplayRTCDate(s32 date,s32 day,s32 month,s32 year);
void SetRTCDateInfo(s32 date,s32 day,s32 month,s32 year);
void RTC(void);

void DisplayonlyMedicineTime(s32 Hour,s32 Min);
void DisplayMedicineTime(s32 hour,s32 min,s32 n);



