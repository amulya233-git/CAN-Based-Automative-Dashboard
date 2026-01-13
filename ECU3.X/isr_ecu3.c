/*
 * File:   isr_ecu3.c
 * Author: Amulya
 * Description : CAN Based automative dashboard
 */


#include <xc.h>

extern unsigned int flag; 
//extern unsigned int count;// Flag value received from CAN (indicates indicator direction)

/* Interrupt Service Routine */
void __interrupt() isr(void)
{
	static unsigned int count; // Counter for timing LED blinking
   // RB5 = 1;
	if (TMR0IF) // Check if Timer0 interrupt occurred
	{
        
		TMR0 = TMR0 + 8; // Reload Timer0 to maintain timing accuracy
        count++; // Increment count for LED blink timing
        
        // --- LEFT INDICATOR ON (flag = 1) ---
        if(flag == 1)
        {
      //      RB4 = 5;
            if (count <= 10000) // LED OFF duration
		    {
			    RB0 = 0;
                RB1 = 0;
		    }
            else if (count <= 20000) // LED ON duration
            {
                RB0 = 1;
                RB1 = 1;
            }
            else if (count > 20000) // Reset cycle
            {
                count = 0;
            }
        }

        // --- RIGHT INDICATOR ON (flag = 2) ---
        else if(flag == 2)
        {
            if (count <= 10000) // LED OFF duration
		    {
			    RB7 = 0;
                RB6 = 0;
            }
            else if (count <= 20000) // LED ON duration
            {
                RB7 = 1;
                RB6 = 1;
            }
            else if (count > 20000) // Reset cycle
            {
                count = 0;
            } 
        }

        // --- INDICATOR OFF (flag = 0) ---
        else if(flag == 0)
        {
            RB0 = 0;
            RB1 = 0;
            RB6 = 0;
            RB7 = 0;
            count = 0; // Reset blink counter
        }

        TMR0IF = 0; // Clear Timer0 interrupt flag
    }
}
