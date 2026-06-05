#include "type.h"
void delay_us(u32 x)
{
	u32 i;
	for(i=12*x;i>0;i--);
}
void delay_ms(u32 x)
{
	u32 i;
	for(i=12000*x;i>0;i--);
}
void delay_s(u32 x)
{
	u32 i;
	for(i=12000000*x;i>0;i--);
}

