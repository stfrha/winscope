/*
   *************************************************************************
   
                     P H I L I P S   P R O P R I E T A R Y
   
              COPYRIGHT (c)   1997 BY PHILIPS SINGAPORE.
                        --  ALL RIGHTS RESERVED  --
   
    File Name:    ISR.C
    Author:        Wenkai Du
    Created:        19 Dec 97
    Modified:
    Revision:        2.1
   
   *************************************************************************
   
    98/11/25         Added I/O mode Main endpoints access. (WK)
   *************************************************************************
   */
#include <stdio.h>
#include <string.h>
#include "reg420_frha.h"
#include "d12ci.h"
#include "USBTest.h"
#include "usb100.h"

extern void bus_reset(void);

extern void ep0_txdone(void);
extern void ep0_rxdone(void);

extern void ep1_txdone(void);
extern void ep1_rxdone(void);

extern void main_txdone(void);
extern void main_rxdone(void);

extern void dma_eot(void);

/*
*************************************************************************
  Public static data
*************************************************************************
*/

EPPFLAGS bEPPflags;

/* Control endpoint TX/RX buffers */
extern CONTROL_XFER ControlData;


extern char armInfo;

/* ISR static vars */
//unsigned char idata GenEpBuf[EP2_PACKET_SIZE];
IO_REQUEST idata ioRequest;

unsigned long ClockTicks = 0;


unsigned char   index;
unsigned char   testBuf[16];
 

interrupt [0x13] void EX1_int (void)                        /* External Interrupt 1 */
{
    unsigned int i_st;

    DISABLE;
 
    bEPPflags.bits.in_isr = 1;
    
    i_st = D12_ReadInterruptRegister();

//    WINSCOPE_LEDS = ~i_st;

    if(i_st != 0) {
        if(i_st & D12_INT_BUSRESET) {
            bus_reset();
            bEPPflags.bits.bus_reset = 1;
        }
        else {
            if(i_st & D12_INT_EOT) {
                dma_eot();
                printf("DMA EOT\n");
            }
            if(i_st & D12_INT_SUSPENDCHANGE)
                bEPPflags.bits.suspend = 1;

            if(i_st & D12_INT_ENDP0IN)
                ep0_txdone();
            if(i_st & D12_INT_ENDP0OUT)
                ep0_rxdone();
            if(i_st & D12_INT_ENDP1IN)
                ep1_txdone();
            if(i_st & D12_INT_ENDP1OUT)
                ep1_rxdone();
            if(i_st & D12_INT_ENDP2IN)
                main_txdone();
            if(i_st & D12_INT_ENDP2OUT)
                main_rxdone();
        }
    } else {
        printf("IR zero\n");
    }

    bEPPflags.bits.in_isr = 0;

    ENABLE;
}


void bus_reset(void)
{
}

void ep0_rxdone(void)
{
    unsigned char ep_last, i;

    ep_last = D12_ReadLastTransactionStatus(0); // Clear interrupt flag

    if (index < 16) {
        testBuf[index++] = ep_last;
        testBuf[index++] = D12_ReadLastTransactionStatus(0);
    }

//    printf("%x\n", ep_last);
//    ep_last = D12_ReadLastTransactionStatus(0); // Clear interrupt flag
//    printf("%x\n", ep_last);

//    armInfo = 1;
    
    if (ep_last & D12_SETUPPACKET) {
    
//        printf("s");
 
        ControlData.wLength = 0;
        ControlData.wCount = 0;

        if( D12_ReadEndpoint(0, (unsigned char *)(&(ControlData.DeviceRequest)),
            sizeof(ControlData.DeviceRequest)) != sizeof(DEVICE_REQUEST) ) {

            D12_SetEndpointStatus(0, 1);
            D12_SetEndpointStatus(1, 1);
            bEPPflags.bits.control_state = USB_IDLE;
            
            return;
        }

        ControlData.DeviceRequest.wValue = SWAP(ControlData.DeviceRequest.wValue);
        ControlData.DeviceRequest.wIndex = SWAP(ControlData.DeviceRequest.wIndex);
        ControlData.DeviceRequest.wLength = SWAP(ControlData.DeviceRequest.wLength);

//        printf("u");

        // Acknowledge setup here to unlock in/out endp
        D12_AcknowledgeEndpoint(0);
        D12_AcknowledgeEndpoint(1);

        ControlData.wLength = ControlData.DeviceRequest.wLength;
        ControlData.wCount = 0;

        if (ControlData.DeviceRequest.bmRequestType & (unsigned char)USB_ENDPOINT_DIRECTION_MASK) {
            bEPPflags.bits.setup_packet = 1;
            bEPPflags.bits.control_state = USB_IDLE;        /* get command */
//            printf("g");
        }
        else {
            if (ControlData.DeviceRequest.wLength == 0) {
                bEPPflags.bits.setup_packet = 1;
                bEPPflags.bits.control_state = USB_IDLE;        /* set command */
//                printf("t");
            }
            else {
                if(ControlData.DeviceRequest.wLength > MAX_CONTROLDATA_SIZE) {
                    bEPPflags.bits.control_state = USB_IDLE;
                    D12_SetEndpointStatus(0, 1);
                    D12_SetEndpointStatus(1, 1);
                }
                else {
                    bEPPflags.bits.control_state = USB_RECEIVE;    /* set command with OUT token */
                }
            } // set command with data
        } // else set command
    } // if setup packet

    else if (bEPPflags.bits.control_state == USB_RECEIVE) {
        i =    D12_ReadEndpoint(0, ControlData.dataBuffer + ControlData.wCount,
            EP0_PACKET_SIZE);

        ControlData.wCount += i;
        if( i != EP0_PACKET_SIZE || ControlData.wCount >= ControlData.wLength) {
            bEPPflags.bits.setup_packet = 1;
            bEPPflags.bits.control_state = USB_IDLE;
        }
    }

    else {
        bEPPflags.bits.control_state = USB_IDLE;
    }
}

void ep0_txdone(void)
{
    short i = ControlData.wLength - ControlData.wCount;

    D12_ReadLastTransactionStatus(1); // Clear interrupt flag

    if (bEPPflags.bits.control_state != USB_TRANSMIT) 
        return;

    if( i >= EP0_PACKET_SIZE) {
        D12_WriteEndpoint(1, ControlData.pData + ControlData.wCount, EP0_PACKET_SIZE);
        ControlData.wCount += EP0_PACKET_SIZE;

        bEPPflags.bits.control_state = USB_TRANSMIT;
    }
    else if( i != 0) {
        D12_WriteEndpoint(1, ControlData.pData + ControlData.wCount, i);
        ControlData.wCount += i;

        bEPPflags.bits.control_state = USB_IDLE;
    }
    else if (i == 0){
        D12_WriteEndpoint(1, 0, 0); // Send zero packet at the end ???

        bEPPflags.bits.control_state = USB_IDLE;
    }
}

void dma_eot(void)
{
    if(bEPPflags.bits.dma_state == DMA_PENDING)
        bEPPflags.bits.setup_dma = 1;
    else
        bEPPflags.bits.dma_state = DMA_IDLE;
}

void ep1_txdone(void)
{
    D12_ReadLastTransactionStatus(3); /* Clear interrupt flag */
}

void ep1_rxdone(void)
{
//   unsigned char len;
 
    D12_ReadLastTransactionStatus(2); /* Clear interrupt flag */

//    len = D12_ReadEndpoint(2, GenEpBuf, sizeof(GenEpBuf));

//    if(len != 0)
//        bEPPflags.bits.ep1_rxdone = 1;
}

void main_txdone(void)
{

    D12_ReadLastTransactionStatus(5); /* Clear interrupt flag */

}

void main_rxdone(void)
{
    D12_ReadLastTransactionStatus(4); /* Clear interrupt flag */

    bEPPflags.bits.main_rxdone = 1;
}

