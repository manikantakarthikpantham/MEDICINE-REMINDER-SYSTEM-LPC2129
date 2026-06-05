#include "type.h"

// Main menu to manage all medicine timings
void Medicine(s32 *m1Min,s32 *m1Hour,s32 *m2Min,s32 *m2Hour,s32 *m3Min,s32 *m3Hour);

// Menu for Medicine 1 settings
void Medicine_1(s32 *m1Hour,s32 *m1Min);

// Menu for Medicine 2 settings
void Medicine_2(s32 *m2Hour,s32 *m2Min);

// Menu for Medicine 3 settings
void Medicine_3(s32 *m3Hour,s32 *m3Min);

// Edit hour of selected medicine
void editMedicineHour(s32 *hour,s32 *min,s32 n);

// Edit minute of selected medicine
void editMedicineMin(s32 *hour,s32 *min,s32 n);

// Display medicine time with label
void DisplayMedicineTime(s32 hour,s32 min,s32 n);

// Display only HH:MM format
void DisplayonlyMedicineTime(s32 Hour,s32 Min);

// Set all medicine times sequentially
void setMedicineTime(s32 *m1Min,s32 *m1Hour,s32 *m2Min,s32 *m2Hour,s32 *m3Min,s32 *m3Hour);

