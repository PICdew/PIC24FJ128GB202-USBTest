#include "system.h"

#include <stdint.h>
#include <string.h>
#include <stddef.h>

#include "usb/usb.h"
#include "app_device_cdc_basic.h"
#include "usb_config.h"

static uint8_t readBuffer[CDC_DATA_OUT_EP_SIZE];
static uint8_t writeBuffer[CDC_DATA_IN_EP_SIZE];


void APP_DeviceCDCBasicDemoInitialize()
{
    CDCInitEP();

    line_coding.bCharFormat = 0;
    line_coding.bDataBits = 8;
    line_coding.bParityType = 0;
    line_coding.dwDTERate = 9600;
}

void APP_DeviceCDCBasicDemoTasks()
{
    /* Check to see if there is a transmission in progress, if there isn't, then
     * we can see about performing an echo response to data received.
     */
    if( USBUSARTIsTxTrfReady() == true)
    {
        uint8_t i;
        uint8_t numBytesRead;

        numBytesRead = getsUSBUSART(readBuffer, sizeof(readBuffer));

        /* For every byte that was read... */
        for(i=0; i<numBytesRead; i++)
        {
            switch(readBuffer[i])
            {
                /* If we receive new line or line feed commands, just echo
                 * them direct.
                 */
                case 0x0A:
                case 0x0D:
                    writeBuffer[i] = readBuffer[i];
                    break;

                /* If we receive something else, then echo it plus one
                 * so that if we receive 'a', we echo 'b' so that the
                 * user knows that it isn't the echo enabled on their
                 * terminal program.
                 */
                default:
                    writeBuffer[i] = readBuffer[i] + 1;
                    break;
            }
        }

        if(numBytesRead > 0)
        {
            /* After processing all of the received data, we need to send out
             * the "echo" data now.
             */
            putUSBUSART(writeBuffer,numBytesRead);
        }
    }

    CDCTxService();
}