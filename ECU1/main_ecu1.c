/*
 * File:   main_ecu1.c
 * Author: Amulya
 * Description : CAN Based automative dashboard
 */


#include <xc.h>
#include "adc_ecu1.h"
#include "clcd_ecu1.h"
#include "dkp_ecu1.h"
#include "can_ecu1.h"
#include "uart_ecu1.h"
#include "msz_id.h"

int get_speed()
{
    unsigned int adc_val, adc_speed;
    char speed[3];

    adc_val = read_adc(CHANNEL4);        // Read analog input from ADC channel 4
    
    // Convert ADC reading (0?1023) to approximate speed value (0?99)
    adc_speed = adc_val / 10.23;         // Scale down ADC value
    
    // Convert speed value into ASCII characters (for CAN transmission)
    speed[0] = (adc_speed % 10) + '0';  // Units place
    speed[1] = (adc_speed / 10) + '0';  // Tens place
    speed[2] = '\0';                    // Null terminator for string safety
    
    // Transmit speed data over CAN with SPEED_MSG_ID
    can_transmit(SPEED_MSG_ID, speed, 2);
    __delay_ms(80);                     // Small delay between transmissions
}
void main(void) 
{
    init_adc();             // Initialize ADC for analog input (speed)
    init_clcd();            // Initialize LCD display (for debugging or display)
    init_digital_keypad();  // Initialize digital keypad (for gear selection)
    init_can();             // Initialize CAN module for communication
    init_uart();            // Initialize UART for serial debugging (if needed)
    
    // Gear states ? each gear represented by a 2-character code
    // ON = ignition ON, GN = Neutral, G1-G5 = forward gears, GR = Reverse, C_ = Clutch
    char gear[9][3] = {"ON", "GN", "G1", "G2", "G3", "G4", "G5", "GR", "C_"};
    unsigned int g_index = 0;   // Gear index (points to the current gear)

    while(1)
    {
        // Continuously read and send speed data
        get_speed();
  
        // Read current key press (only when state changes)
        unsigned char key = read_digital_keypad(STATE_CHANGE);

        // If currently in "Clutch" state (index 8)
        if(g_index == 8)
        {
            // Allow returning to gear mode using SWITCH1 or SWITCH2
            if(key == SWITCH1 || key == SWITCH2)
            {
                g_index = 1;    // Reset to Neutral (GN)
            }
        }
        else
        {
            // SWITCH1 = Increase gear
            if(key == SWITCH1)
            {
                if(g_index < 7) // Limit gear up to G5
                    g_index++;
            }
            // SWITCH2 = Decrease gear
            else if(key == SWITCH2)
            {
                if(g_index > 0) // Limit gear down to ON
                    g_index--;
            }
            // SWITCH3 = Engage clutch
            else if(key == SWITCH3)
            {
                g_index = 8;    // Move to clutch mode (C_)
            }
        }

        // Transmit the selected gear over CAN bus
        can_transmit(GEAR_MSG_ID, gear[g_index], 2);
        __delay_ms(80);

    }

    return;
}