//lcdDefines.h

#define RS 16                 // Register Select pin
#define RW 17                 // Read/Write pin
#define EN 18                 // Enable pin
#define LCD 8                 // Data pin starting position

#define _8BIT_LINE1 0X30      // 8-bit mode, 1-line
#define _8BIT_LINE2 0X38      // 8-bit mode, 2-line
#define DISP_ON 0X0C          // Display ON, cursor OFF
#define DISP_CUR_ON 0X0E      // Display ON, cursor ON
#define DISP_CUR_BLINK_ON 0X0F // Display ON, blinking cursor
#define CLEAR_LCD 0X01        // Clear display
#define SHIFT_CUR_RIGHT 0X06  // Move cursor right
#define GOTO_LINE1_POS0 0X80  // Cursor to line 1 start
#define GOTO_LINE2_POS0 0XC0  // Cursor to line 2 start
