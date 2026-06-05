#include "type.h"
#include "interuptdefinesh.h"
#include <lpc21xx.h>
#include "interupt.h"

extern u32 flag1;   // Flag to stop buzzer (medicine taken)
extern u32 flag2;   // Flag to enter edit mode

// Initialize external interrupts (EINT0 & EINT1)
void InitINTERUPT()
{
	PINSEL0 = PINSEL0 & ~(3<<EINT0);   // Clear EINT0 pin function bits
	PINSEL0 |= EINT0_INPUT_PIN;        // Configure pin as EINT0

	VICIntEnable = 1<<EINT0_CHNO;      // Enable EINT0 interrupt
	VICVectCntl0 = (1<<5) | EINT0_CHNO; // Enable vector slot 0
	VICVectAddr0 = (u32)eint0_irq;     // Assign ISR address

	IODIR1 |= 1<<BUZZ;                 // Configure buzzer pin as output

	PINSEL0 = PINSEL0 & ~(3<<EINT1);   // Clear EINT1 pin function bits
	PINSEL0 |= EINT1_INPUT_PIN;        // Configure pin as EINT1

	VICIntEnable = 1<<EINT1_CHNO;      // Enable EINT1 interrupt
	VICVectCntl1 = (1<<5) | EINT1_CHNO; // Enable vector slot 1
	VICVectAddr1 = (u32)eint1_irq;     // Assign ISR address
}

// ISR for EINT0 – used to stop buzzer when button is pressed
void eint0_irq(void) __irq
{
	IOCLR1 = 1<<BUZZ;   // Turn OFF buzzer
	flag1 = 1;          // Set acknowledgment flag

	EXTINT = 1<<0;      // Clear EINT0 interrupt flag
	VICVectAddr = 0;    // End of interrupt
}

// ISR for EINT1 – used to enter edit mode
void eint1_irq(void) __irq
{
	flag2 = 1;          // Enable edit mode flag

	EXTINT = 1<<1;      // Clear EINT1 interrupt flag
	VICVectAddr = 0;    // End of interrupt
}
