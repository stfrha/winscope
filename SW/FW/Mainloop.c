/*
   //*************************************************************************
   //
   //                  P H I L I P S   P R O P R I E T A R Y
   //
   //           COPYRIGHT (c)   1997 BY PHILIPS SINGAPORE.
   //                     --  ALL RIGHTS RESERVED  --
   //
   // File Name:    MainLoop.C
   // Author:        Wenkai Du
   // Created:        19 Dec 97
   // Modified:
   // Revision:        2.2
   //
   //*************************************************************************
   //
   // 98/11/25        Added I/O access support on Main endpoints. (WK)
   // 03/01/31        Removed all non C51 code
   //*************************************************************************
   */

#include <stdio.h>
#include <string.h>

#include <reg51.h>                /* special function register declarations   */

#include "epphal.h"
#include "d12ci.h"
#include "mainloop.h"
#include "usb100.h"
#include "chap_9.h"
#include "protodma.h"
/*
//*************************************************************************
// USB protocol function pointer arrays
//*************************************************************************
*/
code void (*StandardDeviceRequest[])(void) =
{
    get_status,
    clear_feature,
    reserved,
    set_feature,
    reserved,
    set_address,
    get_descriptor,
    reserved,
    get_configuration,
    set_configuration,
    get_interface,
    set_interface,
    reserved,
    reserved,
    reserved,
    reserved
};

code void (*VendorDeviceRequest[])(void) =
{
    reserved,
    reserved,
    reserved,
    reserved,
    reserved,
    reserved,
    reserved,
    reserved,
    reserved,
    reserved,
    reserved,
    reserved,
    read_write_register,
    reserved,
    reserved,
    reserved
};

/*
//*************************************************************************
//  Public static data
//*************************************************************************
*/

extern EPPFLAGS bEPPflags;
extern unsigned long ClockTicks;
extern unsigned char idata GenEpBuf[];
extern IO_REQUEST idata ioRequest;

CONTROL_XFER ControlData;

code char * _NAME_USB_REQUEST_DIRECTION[] =
{
"Host_to_device",
"Device_to_host"
};

code char * _NAME_USB_REQUEST_RECIPIENT[] =
{
"Device",
"Interface",
"Endpoint(0)",
"Other"
};

code char * _NAME_USB_REQUEST_TYPE[] =
{
"Standard",
"Class",
"Vendor",
"Reserved"
};

code char * _NAME_USB_STANDARD_REQUEST[] =
{
"GET_STATUS",
"CLEAR_FEATURE",
"RESERVED",
"SET_FEATURE",
"RESERVED",
"SET_ADDRESS",
"GET_DESCRIPTOR",
"SET_DESCRIPTOR",
"GET_CONFIGURATION",
"SET_CONFIGURATION",
"GET_INTERFACE",
"SET_INTERFACE",
"SYNC_FRAME"
};

void help_devreq(unsigned char typ, unsigned char req)
{
    typ >>= 5;

    if(typ == USB_STANDARD_REQUEST) {
        printf("Request Type = %s, Request = %s.\n", _NAME_USB_REQUEST_TYPE[typ],
            _NAME_USB_STANDARD_REQUEST[req]);
    }
    else
    {
        if(bEPPflags.bits.verbose)
            printf("Request Type = %s, bRequest = 0x%bx.\n", _NAME_USB_REQUEST_TYPE[typ],
                req);
    }
}

/* Configure Timer 0
   - Mode                  = 1
   - Interrupt                   = ENABLED
   - Clock Source                = INTERNAL
   - Enable Gating Control    = DISABLED
*/

void init_timer0(void)
{
    TMOD &= 0XF0;                    /* clear Timer 0   */
    TMOD  |= 0X1;
    TL0 = 0X0;                   /* value set by user    */
    TH0 = 0X0;                  /* value set by user  */
    ET0 = 1;        /* IE.1*/
    TR0 = 1;                /* TCON.4 start timer  */
    PT0 = 1;

    EA = 1;
}

/* Interrupt Control Unit */
/*   ****  Enabled interrupts in Interrupt Enable Register ****
     ****  GLOBAL INTERRUPT MUST BE ENABLED FOR ANY OTHER
     ****  INTERRUPT TO WORK!
*/
/*                  GLOBAL INTERRUPT DISABLED ALL INTERRUPTS
                    ARE DISABLED          */
/*                    External interrupt 0      */
/*                        Priority Level = 0    */
/*                        Timer 0 interrupt     */
/*                       Priority Level = 0     */
void init_special_interrupts(void)
{
    IT0 = 0;
    EX0 = 1;
    PX0 = 0;
}

void init_port()
{
    P0 = 0xFF;
    P1 = 0xFF;
    P2 = 0xFF;
}

/*Serial Port */
/*Mode            = 1  /8-bit UART
  Serial Port Interrupt    = Disabled         */
/*Receive         = Enabled   */
/*Auto Addressing    = Disabled   */
void init_serial(void)
{
    SCON = 0X52;
    PCON = 0X80 | PCON;
    TMOD = 0X20;
    TCON = 0x69;    /* TCON */
    TH1 = 0xF3;
}

void on_exit(void)
{
}

void main(void)
{
    BOOL in_loop = TRUE;
    unsigned char key, i;
    
    init_port();

    init_serial();

    init_timer0();

    init_special_interrupts();

    printf("\nPDIUSBD12 evaluation board firmware V2.2.\n");
    printf("Copyright (c) Philips Semiconductors, 1998.\n");

    /* Power on reset, lightup LEDs for 1 sec,
       disconnect and reconnect Soft-Connect */
    printf("Re-connect PDIUSBD12 evaluation board to USB.\n");
    reconnect_USB();

    /* Main program loop */

    while( in_loop ){

        if (bEPPflags.bits.timer){
            DISABLE;
            bEPPflags.bits.timer = 0;
            ENABLE;

            if(bEPPflags.bits.configuration)
                check_key_LED();
        }

        if(RI) {
            key = _getkey();
            switch(key) {
                case 0x1b: /* ESC */
                    break;
                case 'r':
                    D12_SendResume();
                    break;
                case 'i':
                    if(bEPPflags.bits.control_state == USB_IDLE)
                        printf("Control state = USB_IDLE.");
                    else if(bEPPflags.bits.control_state == USB_RECEIVE)
                        printf("Control state = USB_RECEIVE.");
                    else if(bEPPflags.bits.control_state == USB_TRANSMIT)
                        printf("Control state = USB_TRANSMIT.");
                    printf(" ControlData.wCount = %x.\n", ControlData.wCount);
                    printf("Endpoint 4 (Bulk Out) Status = %bx, Endpoint 5 (Bulk In) Status = %bx.\n",
                       D12_ReadEndpointStatus(4), D12_ReadEndpointStatus(5));
                    if(bEPPflags.bits.dma_state == DMA_IDLE)
                        printf("DMA_State = DMA_IDLE.\n");
                    else if(bEPPflags.bits.dma_state == DMA_RUNNING)
                        printf("DMA_State = DMA_RUNNING.\n");
                    else if(bEPPflags.bits.dma_state == DMA_PENDING)
                        printf("DMA_State = DMA_PENDING.\n");

                    MCU_HOSTDACK = 1;
                    printf("CPLD counter read back = 0x%bx, 0x%bx.\n", inportb(CPLD_CNT0),
                        inportb(CPLD_CNT1));
                    MCU_HOSTDACK = 0;
                    printf("Last Device Request: bmRequestType = 0x%bx, bRequest= 0x%bx, wValue = 0x%x, wLength = 0x%x, wIndex = 0x%x.\n",

                        ControlData.DeviceRequest.bmRequestType,
                        ControlData.DeviceRequest.bRequest,
                        ControlData.DeviceRequest.wValue,
                        ControlData.DeviceRequest.wLength,
                        ControlData.DeviceRequest.wIndex);

                    printf("Data: ");
                    for(i = 0; i < ControlData.DeviceRequest.wLength; i ++)
                        printf("0x%bx, ", *((ControlData.dataBuffer)+i));
                    printf("\n");

                    break;
                case '\r':
                    reconnect_USB();
                    break;

                case 'v':
                    if(bEPPflags.bits.verbose == 0) {
                        printf("Verbose Mode = ON.\n");
                        DISABLE;
                        bEPPflags.bits.verbose = 1;
                        ENABLE;
                    }
                    else {
                        printf("Verbose Mode = OFF.\n");
                        DISABLE;
                        bEPPflags.bits.verbose = 0;
                        ENABLE;
                    }
                    break;
                default:
                    break;
            }
        }


        if (bEPPflags.bits.bus_reset) {
            DISABLE;
            bEPPflags.bits.bus_reset = 0;
            ENABLE;
            printf("Bus reset!\n");
        } // if bus reset

        if (bEPPflags.bits.suspend) {
            DISABLE;
            bEPPflags.bits.suspend= 0;
            ENABLE;
            suspend_change();
            printf("Suspend change!\n");
        } // if suspend change

        if (bEPPflags.bits.setup_packet){
            DISABLE;
            bEPPflags.bits.setup_packet = 0;
            ENABLE;
            control_handler();
        } // if setup_packet

        if(bEPPflags.bits.setup_dma) {
            DISABLE;
            bEPPflags.bits.setup_dma = 0;
            ENABLE;
            setup_dma();
        } // if setup_dma

    } // Main Loop

    on_exit();
}

void suspend_change(void)
{
}

void stall_ep0(void)
{
    D12_SetEndpointStatus(0, 1);
    D12_SetEndpointStatus(1, 1);
}

void disconnect_USB(void)
{
    // Initialize D12 configuration
    D12_SetMode(D12_NOLAZYCLOCK|D12_CLOCKRUNNING, D12_SETTOONE | D12_CLOCK_12M);
}

void connect_USB(void)
{
    // reset event flags
    DISABLE;
    bEPPflags.value = 0;
    ENABLE;

    // V2.1 enable normal+sof interrupt
    D12_SetDMA(D12_ENDP4INTENABLE | D12_ENDP5INTENABLE);

    // Initialize D12 configuration
    D12_SetMode(D12_NOLAZYCLOCK|D12_CLOCKRUNNING|D12_SOFTCONNECT, D12_SETTOONE | D12_CLOCK_12M);
}


void reconnect_USB(void)
{
    unsigned long clk_cnt;

    MCU_LED0 = 0;
    MCU_LED1 = 0;
    disconnect_USB();

    printf("Wait for 1 second ...\n");

    clk_cnt = ClockTicks;
    while(ClockTicks < clk_cnt + 20)
        ;

    connect_USB();

    MCU_LED0 = 1;
    MCU_LED1 = 1;
}

void init_unconfig(void)
{
    unsigned char i;

    D12_SetEndpointEnable(0);    /* Disable all endpoints but EPP0. */
}

void init_config(void)
{
    D12_SetEndpointEnable(1);    /* Enable  generic/iso endpoints. */
}

void single_transmit(unsigned char * buf, unsigned char len)
{
    if( len <= EP0_PACKET_SIZE) {
        D12_WriteEndpoint(1, buf, len);
    }
}

void code_transmit(unsigned char code * pRomData, unsigned short len)
{
    ControlData.wCount = 0;
    if(ControlData.wLength > len)
        ControlData.wLength = len;

    ControlData.pData = pRomData;
    if( ControlData.wLength >= EP0_PACKET_SIZE) {
        D12_WriteEndpoint(1, ControlData.pData, EP0_PACKET_SIZE);
        ControlData.wCount += EP0_PACKET_SIZE;

        DISABLE;
        bEPPflags.bits.control_state = USB_TRANSMIT;
        ENABLE;
    }
    else {
        D12_WriteEndpoint(1, pRomData, ControlData.wLength);
        ControlData.wCount += ControlData.wLength;
        DISABLE;
        bEPPflags.bits.control_state = USB_IDLE;
        ENABLE;
    }
}

void check_key_LED(void)
{
    static unsigned char c, last_key = 0xf;

    c = MCU_SWM1 & MCU_SWM2;
    c &= 0x0f;
    if (c != last_key) {
        D12_WriteEndpoint(3, &c, 1);
    }

    last_key = c;

    if(bEPPflags.bits.ep1_rxdone) {
        DISABLE;
        bEPPflags.bits.ep1_rxdone = 0;
        ENABLE;
        MCU_LED0 = !(GenEpBuf[3] & 0x1);
        MCU_LED1 = !(GenEpBuf[3] & 0x2);
    }
}

void setup_dma()
{
    bEPPflags.bits.dma_disable = 0; // V2.1

    dma_start(&ioRequest);

    DISABLE;
    bEPPflags.bits.dma_state = DMA_RUNNING;
    ENABLE;

    single_transmit(0, 0);

    if(ioRequest.bCommand & 0x1)
        D12_SetDMA(D12_BURST_16 | D12_DMAENABLE | D12_DMA_INTOKEN);
    else
        D12_SetDMA(D12_BURST_16 | D12_DMAENABLE);
}


void control_handler()
{
    unsigned char type, req;

    type = ControlData.DeviceRequest.bmRequestType & USB_REQUEST_TYPE_MASK;
    req = ControlData.DeviceRequest.bRequest & USB_REQUEST_MASK;

    help_devreq(type, req); // print out device request

    if (type == USB_STANDARD_REQUEST)
        (*StandardDeviceRequest[req])();
    else if (type == USB_VENDOR_REQUEST)
        (*VendorDeviceRequest[req])();
    else
        stall_ep0();
}
 
