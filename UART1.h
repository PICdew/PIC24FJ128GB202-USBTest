/* 
 * File:   UART1.h
 * Author: oyabunn
 *
 * Created on December 14, 2014, 10:43 AM
 */

#ifndef UART1_H
#define	UART1_H

void UART1_init();

// send
void UART1_send_char(unsigned char text);

// send util
void UART1_send(char *text);
void UART1_send_unsignedint(unsigned int val);
void UART1_send_unsignedint5(unsigned int val);
void UART1_send_unsignedlong(unsigned long val);

// receive
char UART1_buffer_append_char(char text);
void UART1_buffer_clear(void);

#endif	/* UART0_H */

