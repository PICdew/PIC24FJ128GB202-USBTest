

#include <p24fj128gb202.h>
#include "UART1.h"

#define BufferSize 32

char UART1_buffer[BufferSize];
unsigned int _index;

void UART1_init()
{
    UART1_buffer_clear();

    U1MODE = 0b1010000000001000;
    U1STA  = 0b0000010000000000;//cation set UTXEN after UARTEN
    U1BRG  = 34;  // 115k @FCY=16Mhz

    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;

    IFS4bits.U1ERIF = 0;
    IEC4bits.U1ERIE = 1; //error handling
}

void UART1_send_char(unsigned char text)
{
    while(!U1STAbits.TRMT);
    U1TXREG = text;
}

void UART1_send(char *text)
{
    unsigned int i=0;
    while(text[i]!='\0' && i<100){
        UART1_send_char(text[i++]);
    }
}

void UART1_send_unsignedint(unsigned int val)
{
    unsigned int ten = 10000;
    unsigned char i;
    unsigned char lock=0;
    for(i=0; i<4; i++){
        if(val/ten==0 && lock==0 && i!=3){
            ten/=10;
            continue;
        }
        lock=1;
        UART1_send_char(val/ten + 48);
        val%=ten;
        ten/=10;
    }

    UART1_send_char(val + 48);
}

void UART1_send_unsignedint5(unsigned int val)
{
    unsigned int ten = 10000;
    unsigned char i;
    unsigned char lock=0;
    for(i=0; i<4; i++){
        lock=1;
        UART1_send_char(val/ten + 48);
        val%=ten;
        ten/=10;
    }

    UART1_send_char(val + 48);
}

void UART1_send_unsignedlong(unsigned long val)
{
    unsigned long ten = 1000000000;
    unsigned char i;
    for(i=0; i<9; i++)
    {
        UART1_send_char(val/ten + 48);
        val%=ten;
        ten/=10;
    }

    UART1_send_char(val + 48);
}

char UART1_buffer_append_char(char text)
{
    if (_index >= BufferSize)
        return 0;
    UART1_buffer[_index++] = text;
    return 1;
}

void UART1_buffer_clear(void)
{
    _index = 0;
}

unsigned int serial_buffer_read_param_2integerAt(unsigned char index)
{
    return (UART1_buffer[index]-48)*10 + UART1_buffer[index+1]-48;
}

unsigned int serial_buffer_read_param_5integerAt(unsigned char index)
{
    unsigned int i,ten=10000,ret=0;
    for(i=0;i<5;i++){
        ret += (UART1_buffer[index++]-48)*ten;
        ten/=10;
    }

    return ret;
}

unsigned char is_equal_to_buffer_headstring(char*text_a)
{
    unsigned int i=0;
    for(i=0;i<BufferSize;i++){
        if(text_a[i]=='\0')return 1;
        if(text_a[i]!=UART1_buffer[i])return 0;
    }
    return 0;
}

unsigned int is_equal_to_buffer_paramstring(unsigned char *text_a)
{
    unsigned char i=0;
    unsigned int  ret=0;
    unsigned char operationLock=1;
    unsigned char head=0,length=0;
    unsigned char val=0;
    for(i=0;i<BufferSize;i++){
        val = UART1_buffer[i];
        if(val=='\n' || _index == i)break;
        if(val==' '){operationLock=0;continue;}
        if(operationLock)continue;
        if(head==0)head = i;

        length++;
    }
    if(head==0 || length==0)return ret;

    for(i=0;i<length;i++){
        if(text_a[i]!=UART1_buffer[i+head])return ret;
        if(text_a[i]==0)return ret;
    }

    if(text_a[length]==0)ret=1;

    return ret;
}

unsigned char is_equal_string(char*text_a, char*text_b)
{
    unsigned int i=0;
    for(i=0;i<BufferSize;i++){
        if(text_a[i]!=text_b[i])return 0;
        if(text_a[i]==0)return 1;
    }
    return 0;
}
