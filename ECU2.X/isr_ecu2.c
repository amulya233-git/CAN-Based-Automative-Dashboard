/*
 * File:   isr_ecu2.c
 * Author: Amulya
 * Description : CAN Based automative dashboard
 */


#include <xc.h>

void pattern1(void);
void pattern2(void);
void pattern3(void);
extern unsigned int flag;

void __interrupt() isr(void)
{
	static unsigned short count;
     
	if (TMR0IF)
	{
		TMR0 = TMR0 + 8;
        count++;
            if(flag == 1)
            {
                if (count <= 10000)
                {
                   RB0 = 0;
                   RB1 = 0;
                }
                else if(count <= 20000)
                {
                    RB0 = 1;
                   RB1 = 1;
                }
                else if(count > 20000)
                    count = 0;
            }   
                
            else if(flag == 2)
            {
                if (count <= 10000)
                {
                   RB6 = 0;
                   RB7 = 0;
                }
                else if(count <= 20000)
                {
                    RB6 = 1;
                   RB7 = 1;
                }
                else if(count > 20000)
                    count = 0;
            }
        
            else if(flag == 0)
            {
                PORTB = 0;
            }
		
		TMR0IF = 0;
	}
}
