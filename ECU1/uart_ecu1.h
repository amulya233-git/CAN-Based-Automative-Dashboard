/* 
 * File:   uart_ecu1.h
 * Author: Amulya
 * Description : CAN Based automative dashboard
 */

#ifndef UART_ECU1_H
#define	UART_ECU1_H

#define RX_PIN					TRISC7
#define TX_PIN					TRISC6

void init_uart(void);
void putch(unsigned char byte);
int puts(const char *s);
unsigned char getch(void);
unsigned char getch_with_timeout(unsigned short max_time);
unsigned char getche(void);

#endif


