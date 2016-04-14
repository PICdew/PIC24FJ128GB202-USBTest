
#include "system.h"
#include "configration_bits.h"

#include "system_config.h"
#include "usb/usb.h"
#include "usb/usb_device.h"
#include "usb/usb_device_cdc.h"

#include "UART1.h"

#include "app_device_cdc_basic.h"

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

    USBDeviceInit();
    USBDeviceAttach();

    while(1) {
        if( USBGetDeviceState() < CONFIGURED_STATE ) { continue; }
        if( USBIsDeviceSuspended() == true ) { continue; }
        
        APP_DeviceCDCBasicDemoTasks();
    }
}

bool USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, uint16_t size)
{
    switch( (int) event )
    {
        case EVENT_TRANSFER:
            UART1_send("Interrupt trnsfer\n");
            break;

        case EVENT_SOF:
            //UART1_send("Interrupt sof\n");
            break;

        case EVENT_SUSPEND:
            UART1_send("Interrupt suspend\n");
            break;

        case EVENT_RESUME:
            UART1_send("Interrupt resume\n");
            break;

        case EVENT_CONFIGURED:
            UART1_send("Interrupt configured\n");
            APP_DeviceCDCBasicDemoInitialize();
            break;

        case EVENT_SET_DESCRIPTOR:
            UART1_send("Interrupt descriptor\n");
            break;

        case EVENT_EP0_REQUEST:
            /* We have received a non-standard USB request.  The HID driver
             * needs to check to see if the request was for it. */
            UART1_send("Interrupt request\n");
            USBCheckCDCRequest();
            break;

        case EVENT_BUS_ERROR:
            UART1_send("Interrupt error\n");
            break;

        case EVENT_TRANSFER_TERMINATED:
            UART1_send("Interrupt terminated\n");
            break;

        default:
            UART1_send("Interrupt Other\n");
            break;
    }
    return true;
}

//===================================================================//
// Exceptions
//===================================================================//
void __attribute__((interrupt,auto_psv)) _OscillatorFail(void) {

}

void __attribute__((interrupt,auto_psv)) _AddressError(void) {

}

void __attribute__((interrupt,auto_psv)) _StackError(void) {

}

void __attribute__((interrupt,auto_psv)) _MathError(void) {

}

//===================================================================//
// Interrupts
//===================================================================//
void __attribute__((interrupt,auto_psv)) _U1RXInterrupt(void) {

    IFS0bits.U1RXIF = 0;

    // read receive val
    char val = U1RXREG;
    // if there is buffered val. ignore first data
    while(U1STAbits.URXDA==1)val = U1RXREG;

    // ignore error
    U1STAbits.OERR = 0;
    
    // isAdmin? => echo data
    if(0){
        while(!U1STAbits.TRMT);
        U1TXREG = val;
    }

    // "Enter" = \r \n
    if(val=='\r'){
        
    }else if(val=='\n'){
        
    }else{
        
    }
}

void __attribute__((interrupt,auto_psv)) _USB1Interrupt()
{
    USBDeviceTasks();
}