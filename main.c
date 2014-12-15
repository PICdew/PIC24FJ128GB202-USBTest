#include <p24fj128gb202.h>
#include "ConfigrationBits.h"

#include "UART1.h"

void IO_init()
{
    // PORTB Analog IO to Digital IO
    ANSB = 0;

    // UART1
    RPINR18bits.U1RXR  = 15;   // RPN15 == U1RX
    RPOR7bits.RP14R    = 3;    // RPN1 -> U1TX
}

void init()
{
    IO_init();
    UART1_init();
}

int main(int argc, char** argv)
{
    unsigned long i=0;
    
    // for Xtal Stabilization
    i=0;
    while(i++<0xFF);

    // initialize
    init();

    UART1_send("Hi\n");
    
    while(1){
        
    };
}