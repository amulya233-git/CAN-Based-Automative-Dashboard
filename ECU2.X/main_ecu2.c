/*
 * File:   main_ecu2.c
 * Author: Amulya
 * Description : CAN Based automative dashboard 
 */

#include <xc.h>
#include "adc_ecu2.h"
#include "dkp_ecu2.h"
#include "isr_ecu2.h"
#include "timer_ecu2.h"
#include "can_ecu2.h"
#include "uart_ecu2.h"
#include "msz_id.h"

#define _XTAL_FREQ 20000000

unsigned int flag = 0, delay = 0;  // Used for LED pattern control and delay counter
int get_rpm()
{
    unsigned long int adc_val = 0, adc_rpm = 0;
    unsigned char rpm[4];
    
    adc_val = read_adc(CHANNEL4);          // Read analog value from ADC channel 4
    adc_rpm = ((adc_val / 10.23) * 60);    // Convert ADC value to RPM (scaling logic)
    
    // Convert RPM value to individual digits and store as ASCII
    rpm[0] = ((adc_rpm / 1000) + '0');
    rpm[1] = (((adc_rpm / 100) % 10) + '0');
    rpm[2] = ((adc_rpm / 10) % 10 + '0');
    rpm[3] = ((adc_rpm % 10) + '0');
    rpm[4] = '\0';                         // Null terminate the string
    
    can_transmit(RPM_MSG_ID, rpm, 4);      // Transmit RPM data over CAN bus
    __delay_ms(80);                        // Small delay between transmissions
}

void main(void) 
{
    init_adc();             // Initialize ADC module
    init_digital_keypad();  // Initialize digital keypad
    init_timer0();          // Initialize Timer0
    init_uart();            // Initialize UART communication
    TRISB = TRISB | 0X80;   // Set RB7 as input (or preserve bit 7)
    init_can();             // Initialize CAN module
    
    PORTB = 0x00;           // Clear PORTB initially
    
    unsigned int r_msz_id, r_len, i_msz, i_len;  // Variables for message info (not used)
    unsigned char r_ssd[4], arr_led[1], rec_arr_led[3];  // Arrays for CAN data
    
    while(1)
    {
        get_rpm();  // Continuously read and transmit RPM value
        
        unsigned char key = read_digital_keypad(STATE_CHANGE);  // Read keypad input
        
        // Based on key pressed, set indicator LED state and send over CAN
        if(key == SWITCH1)
        {
            arr_led[0] = '1';    // Example: Left indicator ON
        }
        if(key == SWITCH3)
        {
            arr_led[0] = '2';    // Example: Right indicator ON
        }
        if(key == SWITCH2)
        {
            arr_led[0] = '0';    // Example: Both indicators OFF
        }
        
        arr_led[1] = '\0';       // Null terminate string
        
        can_transmit(INDICATOR_MSG_ID, arr_led, 1);  // Send indicator status via CAN
        __delay_ms(80);          // Delay for stability
    }
    
    return;
}
//void pattern1() this method is for using without timer
//{
//    if(delay <= 100)
//    {
//        RB0 = 0;
//        RB1 = 0;
//    }za
//    else if(delay <= 300)
//    {
//       RB0 = 1;
//        RB1 = 1; 
//    }
//    else
//        delay = 0;  
//    delay++;
//}
//void pattern2()
//{
//    if(delay <= 100)
//    {
//        RB6 = 0;
//        RB7 = 0;
//    }
//    else if(delay <= 300)
//    {
//        RB6 = 1;
//        RB7 = 1; 
//    }
//    else
//        delay = 0;
//    
//    delay++;
//}
//void pattern3()
//{
//    PORTB = 0;
//}
//    if(key == SWITCH1)//Using delay 
//    { flag = 1;
//      delay = 0;
//      PORTB = 0X00;}
//    if(key == SWITCH3)
//    { flag = 2;
//      delay = 0;
//      PORTB = 0X00;}
//    if(key == SWITCH2)
//    { flag = 3;
//      delay = 0;
//      PORTB = 0X00;   }
//    if(flag == 1)
//        pattern1(); 
//    if(flag == 2)
//        pattern2();
//    if(flag == 3)
//        PORTB = 0X00;