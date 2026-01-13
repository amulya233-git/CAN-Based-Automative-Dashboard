/*
 * File:   main_ecu3.c
 * Author: 2003a
 * Description : CAN Based automative dashboard
 */


#include <xc.h>
#include "can_ecu3.h"
#include "clcd_ecu3.h"
#include "msz_id_ecu3.h"
#include "timer0_ecu3.h"
#include "uart_ecu3.h"

unsigned int flag = 0;
static void init_config(void) {
    // Initialize CLCD and CANBUS
    init_clcd();
    init_can();
    init_uart();
    TRISB = TRISB | 0X80;
    TRISB0 = 0;
      TRISB1 = 0;
      
      TRISB6 = 0;
      TRISB7 = 0;
    PORTB = 0x00;

    // Enable Interrupts
    PEIE = 1;
    GIE = 1;
    init_timer0();
}

void main(void) 
{
    init_config();
    
    unsigned int recv_msg_id = 0, recv_length = 0;
    unsigned char recv_data[5];  // Buffer to store received CAN data

    while(1)
    {
       // RB0 = 1;
        clcd_print("SP  GR  RPM  ID", LINE1(0));  

        // Receive CAN message (Message ID, Data Array, and Length)
        can_receive(&recv_msg_id, recv_data, &recv_length);
      
        // ----- Display SPEED on CLCD -----
        if(SPEED_MSG_ID == recv_msg_id)
        {
            recv_data[2] = '\0';                 // Null-terminate the string
            clcd_print(recv_data, LINE2(0));     // Print speed value at position 0
        }

        // ----- Display GEAR on CLCD -----
        else if(GEAR_MSG_ID == recv_msg_id)
        {
            recv_data[2] = '\0';                 // Null-terminate the string
            clcd_print(recv_data, LINE2(4));     // Print gear value at position 4
        }

        // ----- Display RPM on CLCD -----
        else if(RPM_MSG_ID == recv_msg_id)
        {
            recv_data[recv_length] = '\0'; // Null-terminate based on received length
            clcd_print(recv_data, LINE2(7));     // Print RPM value at position 7
        }

        // ----- Display INDICATOR Status on CLCD -----
        else if(INDICATOR_MSG_ID == recv_msg_id)
        {
            
            if(recv_data[0] == '1')              // Indicator LEFT ON
            {
                RB0= 1;
                flag = 1;
                  //     count =0;
                clcd_print("<-", LINE2(13));           // Show left arrow
            }
            else if(recv_data[0] == '2')         // Indicator RIGHT ON
            {
                flag = 2;
                clcd_print("->", LINE2(13));           // Show right arrow
            }
            else if(recv_data[0] == '0')         // Indicator OFF
            {
                flag = 0;
                clcd_print("OF", LINE2(13));           // Show OFF text
            }
        }
    }
    return;
}
