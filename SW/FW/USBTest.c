#include <stdio.h>
#include <string.h>
#include "reg420_frha.h"
#include "USBTest.h"
#include "d12ci.h"
#include "usb100.h"
#include "chap_9.h"
#include "RegisterIF.h"
 
/*
*************************************************************************
 USB protocol function pointer arrays
*************************************************************************
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
    reserved,
    reserved,
    reserved,
    reserved
};

/*
*************************************************************************
  Public static data
*************************************************************************
*/

extern EPPFLAGS bEPPflags;
extern unsigned char   index;
extern unsigned char   testBuf[16];
//extern unsigned char idata GenEpBuf[];

unsigned char       triggedInterrupt;
CONTROL_XFER        ControlData;
char                armInfo = 0;
unsigned char idata buf[EP2_PACKET_SIZE];
char                fpgaConfigState;            //State of FPGA configuration

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

void main( void )
{   
    char    out_data = 1;
    int     count = 0;
    char    rounds = 0;
    unsigned char key, i;
//    unsigned char len;
    
    RW_SOURCE_FPGA = 0;             /* CPU must (initaily) control the RD and WR signals to the D12 */
                                    /* This enables the output of the three state buffer */

    D12_A0 = 1;                     /* D12 Address latch in use, tie A0 of D12 to high */

    D12_CS = 0;                     /* Before FPGA is configured CPU can only access D12 */
                                    /* Therefore D12 chip select is always active until FPGA */ 
                                    /* gets configured. When FPGA is configured, it supplies the */
                                    /* D12_CS from interna address decoding. Therefore the */
                                    /* CPU D12_CS must be put high (pull up) by CPU. Before configuration */ 
                                    /* ends. */


    /* D12 now (after reset) generates 4MHz clock to CPU, set D12 clock output to 24MHz */
    D12_SetMode(D12_NOLAZYCLOCK|D12_CLOCKRUNNING, D12_SETTOONE | D12_CLOCK_24M);
    
    /* Initialize serial communication 9600 baud (increase to 115200 when function established) */
    /* Use Serial port 0, with timer 2 as baudrate generator gives: */
    /* BAUDRATE = 1/16 * (OSC/2)/(65536-RELOAD) and hence */
    /* RELOAD = 65536 - (osc/2)/(16*BAUDRATE) */
    /* With BAUDRATE = 9600, RELOAD = 65458 = 0xFFB2 */

    SCON0 = 0x50;                                   /* Set serial port 2 to 8 bit, no parity */
    RCAP2H = 0xFF;                                  /* Set MSB of timer 2 relaod */
    RCAP2L = 0xB2;                                  /* Set LSB of timer 2 relaod */

    /* Upon reset timer 2 is set at autoreload timer function */
    TR2 = 1;                                        /* Start timer 2 running */
    TCLK = 1;                                       /* Serial port 0 transmitter use timer 2 baud generator */
    RCLK = 1;                                       /* Serial port 0 receiver use timer 2 baud generator */
    TI_0 = 1;

    /* Enable external interrupt 1 (D12 IRQ) and put it at priority level 1 */
    LPX1 = 1;                                       /* External interrupt priority level 1 */
    EX1 = 1;                                        /* External interrupt 1 enabled */

    /* Enable external interrupt 0 (FPGA IRQ) and put it at priority level 0 */
    LPX0 = 0;                                       /* External interrupt priority level 0 */
    EX0 = 1;                                        /* External interrupt 0 enabled */

    IP1 = 0;                                        /* External interrupt priority level */

    EA = 1;                                         /* Enabled all individually enabled interrupts */

    printf("WinScope USBTest\n");                   /* Send welcome string */
    
    for (i=0 ; i<16 ; i++) testBuf[i] = 0xFF;
    index = 0;
    
    WINSCOPE_LEDS = 0xFF;                           /* Turns all leds off */
 
    reconnect_USB();                                /* Connects USB on host PC */

    printf("OK...\n");                              /* Send welcome string */
    
//    index = 0;
    fpgaConfigState = FCS_IDLE;                     // Default state of FPGA configuration procedures
    triggedInterrupt = 0;

    while (1)
    {
/*
        if (armInfo) 
        {
            armInfo = 0;
            printf("EP0: %x\n", D12_ReadLastTransactionStatus(0));
            printf("EP1: %x\n", D12_ReadLastTransactionStatus(1));
            printf("EP2: %x\n", D12_ReadLastTransactionStatus(2));
            printf("EP3: %x\n", D12_ReadLastTransactionStatus(3));
            printf("EP4: %x\n", D12_ReadLastTransactionStatus(4));
            printf("EP5: %x\n", D12_ReadLastTransactionStatus(5));
        }
*/
/*
        count++;
        if (count > 32000)
        {
            rounds++;
            count = 0;
            if (rounds >= 5) 
            {
                rounds = 0;
                out_data = out_data<<1;
                if (out_data == 0) out_data = 1;
//                WINSCOPE_LEDS = ~out_data;
            }
        }
*/
        if(RI_0) {
            key = GetKey();
            switch(key) {
                case 'w':
                    LEDS_REG = 0xAA;
                    break;
                case 'l':
                    WINSCOPE_LEDS = ~LEDS_REG;
                    LEDS_REG = 0x55;
                    break;
                case 'ö':
                    printf("EP0: %x\n", D12_ReadLastTransactionStatus(0));
                    printf("EP1: %x\n", D12_ReadLastTransactionStatus(1));
                    printf("EP2: %x\n", D12_ReadLastTransactionStatus(2));
                    printf("EP3: %x\n", D12_ReadLastTransactionStatus(3));
                    printf("EP4: %x\n", D12_ReadLastTransactionStatus(4));
                    printf("EP5: %x\n", D12_ReadLastTransactionStatus(5));
                    break;
 
                case 'b':
                    for (i=0 ; i<index ; i++) {
                        printf("%d: %x\n", i, testBuf[i]);
                    }
                    for (i=0 ; i<50 ; i++) testBuf[i] = 0xFF;
                    index = 0;
                    break;
                    
                case 'q':
                    i = D12_ReadChipID();
                    printf("Device ID: %x\n", i);
                    break;
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
                    printf("Endpoint 4 (Bulk Out) Status = %x, Endpoint 5 (Bulk In) Status = %x.\n",
                       D12_ReadEndpointStatus(4), D12_ReadEndpointStatus(5));
                    if(bEPPflags.bits.dma_state == DMA_IDLE)
                        printf("DMA_State = DMA_IDLE.\n");
                    else if(bEPPflags.bits.dma_state == DMA_RUNNING)
                        printf("DMA_State = DMA_RUNNING.\n");
                    else if(bEPPflags.bits.dma_state == DMA_PENDING)
                        printf("DMA_State = DMA_PENDING.\n");

                    printf("Last Device Request: bmRequestType = 0x%x, bRequest= 0x%x, wValue = 0x%x, wLength = 0x%x, wIndex = 0x%x.\n",

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
//            setup_dma();
        } // if setup_dma
 
        if(bEPPflags.bits.ep1_rxdone) {
            DISABLE;
            bEPPflags.bits.ep1_rxdone = 0;
            ENABLE;
            printf("Received EP1!!!\n");
        }

        if(bEPPflags.bits.main_rxdone) {
            DISABLE;
            bEPPflags.bits.main_rxdone = 0;
            ENABLE;
            MainHandler();
/*
            len = D12_ReadEndpoint(4, GenEpBuf, sizeof(GenEpBuf));
            printf("Received main !!!\n");
            for (i=0 ; i<16 ; i++)
            {
                printf("%d: -%x-\n", i, GenEpBuf[i]);
            }
            GenEpBuf[0] = 'F';
            GenEpBuf[1] = 'r';
            GenEpBuf[2] = 'e';
            GenEpBuf[3] = 'd';
            GenEpBuf[4] = 'r';
            GenEpBuf[5] = 'i';
            GenEpBuf[6] = 'k';
            GenEpBuf[7] = 0x00;
            
            len = D12_WriteEndpoint(5, GenEpBuf, 8);
            printf("D12_WriteEndpoint claims to write: %d bytes\n", len);
*/
        }
        
    }
}

void    WaitKey( void )
{
    do { } while (!RI_0);
    RI_0 = 0;
} 

unsigned char   GetKey( void )
{
    unsigned char c;
    
    do { } while (!RI_0);
    c = SBUF0;
    RI_0 = 0;
    return c;
} 

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

void suspend_change(void)
{
    if (P1.0 == 1) printf("D12 in suspend mode\n");
    else printf("D12 NOT in suspend mode\n");
}

void stall_ep0(void)
{
    D12_SetEndpointStatus(0, 1);
    D12_SetEndpointStatus(1, 1);
}

void disconnect_USB(void)
{
    // Initialize D12 configuration
    D12_SetMode(D12_NOLAZYCLOCK|D12_CLOCKRUNNING, D12_SETTOONE | D12_CLOCK_24M);
}

void connect_USB(void)
{
    // reset event flags
    DISABLE;
    bEPPflags.value = 0;
    ENABLE;

    // Initialize D12 configuration
    D12_SetMode(D12_NOLAZYCLOCK|D12_CLOCKRUNNING|D12_SOFTCONNECT, D12_SETTOONE | D12_CLOCK_24M);
}


void reconnect_USB(void)
{
    int count = 0;
    char rounds = 0;

    disconnect_USB();

    printf("Wait for 1 second ...\n");

    while (rounds < 50) 
    {
        count++;
        if (count > 32000) 
        {   
            count = 0; 
            rounds++;
        }
    }
 
    connect_USB();
}

void init_unconfig(void)
{
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

void control_handler( void )
{
    unsigned char type, req;

    type = ControlData.DeviceRequest.bmRequestType & USB_REQUEST_TYPE_MASK;
    req = ControlData.DeviceRequest.bRequest & USB_REQUEST_MASK;

    help_devreq(type, req); // print out device request

    if (type == USB_STANDARD_REQUEST) (*StandardDeviceRequest[req])();
    else if (type == USB_VENDOR_REQUEST) (*VendorDeviceRequest[req])();
    else stall_ep0();
}

void    MainHandler( void )
{
    unsigned char   len, dat, i;
    int             address;

    len = D12_ReadEndpoint(4, buf, sizeof(buf));
    
//    printf("buf[0]: %d\n", buf[0]);
    
    if (buf[0] == 0x22) {                                       // Read register data command
        address = buf[1];                                       // Get address from command
        dat = (*(unsigned char xdata *) address);               // Read FPGA register
        buf[1] = dat;                                           // Generate response
        len = D12_WriteEndpoint(5, buf, 2);                     // Send response, 0x20 + data
//        printf("Read command, address: %x, data: %x\n", address, dat);
    } else if (buf[0] == 0x20) {                                // Write register command
        address = buf[1];                                       // Get address from command
        dat = buf[2];                                           // Get data from command
        (*(unsigned char xdata *) address) = dat;               // Write FPGA register
        i = 0;                                                  // Delay between write and read
        while (i<255) i++;
        dat = (*(unsigned char xdata *) address);               // Read data back from register
        buf[1] = dat;                                           // Generate response
        len = D12_WriteEndpoint(5, buf, 2);                     // Send response, 0x20 + data
//        printf("Write command, address: %x, data: %x\n", address, dat);
    } else if (buf[0] == 0x24) {                                // GetFPGAStatus command
        buf[1] = TRIGG_SETUP_REG.field;
//        printf("1: %x\n", TRIGG_SETUP_REG.field);
        buf[2] = PRBA_TRG_LEVEL_REG;
//        printf("2: %x\n", PRBA_TRG_LEVEL_REG);
        buf[3] = PRBB_TRG_LEVEL_REG;
//        printf("3: %x\n", PRBB_TRG_LEVEL_REG);
        buf[4] = PODP_TRG_PTRN_REG;
//        printf("4: %x\n", PODP_TRG_PTRN_REG);
        buf[5] = PODQ_TRG_PTRN_REG;
        buf[6] = PODP_PTRN_VALID_REG;
        buf[7] = PODQ_PTRN_VALID_REG;
        buf[8] = PODP_EDGE_SLOPE_REG;
        buf[9] = PODQ_EDGE_SLOPE_REG;
        buf[10] = PODP_EDGE_VALID_REG;
        buf[11] = PODQ_EDGE_VALID_REG;
        buf[12] = TRG_DELY_L_REG;
        buf[13] = TRG_DELY_M_REG;
        buf[14] = TRG_DELY_H_REG;
        buf[15] = BUFFER_SIZE_L_REG;
        buf[16] = BUFFER_SIZE_M_REG;
        buf[17] = BUFFER_SIZE_H_REG;
        buf[18] = CONTROL_REG.field;
        buf[19] = STATUS_REG.field;
        buf[20] = CLK_TOGGLE_L_REG;
        buf[21] = CLK_TOGGLE_M_REG;
        buf[22] = CLK_TOGGLE_H_REG;
        buf[23] = CLK_RESET_L_REG;
        buf[24] = CLK_RESET_M_REG;
        buf[25] = CLK_RESET_H_REG;
        buf[26] = PRB_GAIN_REG.field;
        buf[27] = PRB_POD_CTRL_REG.field;
        buf[28] = LEDS_REG;
        buf[29] = ID_REG;
        buf[30] = REV_REG;
        len = D12_WriteEndpoint(5, buf, 33);                    // Send response, 0x24 + All register
    } else if (buf[0] == 0x26) {                                // IsInterruptTrigged
        buf[1] = triggedInterrupt;
        len = D12_WriteEndpoint(5, buf, 2);                     // Send response
        triggedInterrupt = 0;
    } else if (buf[0] == 0x80) {                                // SendSampledData
        len = D12_WriteEndpoint(5, buf, 1);                     // Send response
        D12_SetDMA(0x0F);                                       // DMA = 16 byte burst, Enabled, dir = 1, Reload = 0, irq mode = 0, no irq
    } else if (buf[0] == 0x10) {                                // Get Version String command
        strcpy((char*) buf+1, CPU_FW_VER_STR);
        len = D12_WriteEndpoint(5, buf, 31);                    // Send response, 0x10 + string
//        printf("GetCpuFwVer command\n");
    } else if (buf[0] == 0x30) {                                // Set leds command
        WINSCOPE_LEDS = buf[1];                                 // Set leds
        len = D12_WriteEndpoint(5, buf, 1);                     // Send response, 0x30
//        printf("SetCpuLeds command\n");
    } else if (buf[0] == 0x40) {                                // InitiateFPGAConfiguration
        InitiateFPGA();
    } else if (buf[0] == 0x41) {                                // FPGAConfigurationData
        FPGAConfigurationData();            
    } else if (buf[0] == 0x42) {                                // TerminateFPGAConfiguration
        TerminateFPGAConfiguration();            
    } else if (buf[0] == 0x50) {                                // CyclicEnduranceUSBTest
        D12_WriteEndpoint(5, buf, 64);
    }
}

void    InitiateFPGA( void )
{
    int count = 0;
    char rounds = 0;
 
    fpgaConfigState = FCS_CONFIGURING;

    PROGRAM_N = 0;                                              // FPGA continiously clears its memory
 
//    printf("PROGRAM_N is now low\n");
//    WaitKey();
    while (count < 32000) {count++;}                            // Wait a while
        
    PROGRAM_N = 1;
    
    D12_CS = 0;                                                 // FPGA is not configured (even if it was before)
                                                                // The CS to D12 must manually be pulled low
    RW_SOURCE_FPGA = 0;                                         // RD and WR signals now originate from CPU
                                                                
//    printf("PROGRAM_N is now high\n");
//    WaitKey();

    while (INIT_N == 0);                                        // Wait until INIT_N goes high

    count = 0;                                                  // Delay at least 5us after INIT_N goes high
    rounds = 0;
    while (rounds < 1) {count++;  if (count > 32000) { count = 0; rounds++;}}  // Wait a while

    CCLK = 0;
    count = 0;
    while(count < 5) count++;

//    printf("Before initial pulse\n");
//    WaitKey();
    CCLK = '1';                                                 // One initial pulse on CCLK
    count = 0;
    while(count < 5) count++;

    printf("c");

//    printf("CCLK is now high, what is rdy/busy?\n");
/*    if (RDY_BUSY_N) {
        printf("RDY/BUSY high\n");
    } else {
        printf("RDY/BUSY low \n");
    }
    WaitKey();
*/    
    CCLK = '0';
    count = 0;
    while(count < 5) count++;

    
    buf[0] = 0x40;                                              // Send command response
    D12_WriteEndpoint(5, buf, 1);
}

void    FPGAConfigurationData( void )
{
    char    i, j;
    char    count = 0;
    
    if (fpgaConfigState != FCS_CONFIGURING) {
        buf[0] = 0x00;                                              // Send error response
        D12_WriteEndpoint(5, buf, 1);
        fpgaConfigState = FCS_IDLE;
        return;
    }
    for (i=1 ; i<64 ; i++) {                                        // 63 bytes of data per packet
        P0 = buf[i];
        count = 0;
        while(count < 5) count++;
//        printf("Before positive CCLK edge (with data)\n");
//        WaitKey();
        CCLK = 1;
        count = 0;
        while(count < 5) count++;
/*        printf("CCLK is now high, what is rdy/busy?\n");
        if (RDY_BUSY_N) {
            printf("RDY/BUSY high\n");
        } else {
            printf("RDY/BUSY low \n");
        }
        WaitKey();
*/        

        if (!RDY_BUSY_N) {                                          // Did we not get an ack?
            printf("Error, no data acknowledge\n");
            buf[0] = 0x00;                                          // Send error response
            D12_WriteEndpoint(5, buf, 1);
            fpgaConfigState = FCS_IDLE;
            return;
        }
        CCLK = '0';
        count = 0;
        while(count < 5) count++;
        for (j=0 ; j<7 ; j++) {
//            printf("Before positive CCLK edge\n");
//            WaitKey();
            CCLK = '1';                                             // Seven more clock pulses to clock out serial data
            count = 0;
            while(count < 5) count++;
/*            printf("CCLK is now high, what is rdy/busy?\n");
            if (RDY_BUSY_N) {
                printf("RDY/BUSY high\n");
            } else {
                printf("RDY/BUSY low\n");
            }
            WaitKey();
*/            
            CCLK = '0';
            count = 0;
            while(count < 5) count++;
        }
    }
    if (!INIT_N) {                                                   // Is INIT_N low indicating a CRC error
                                                                    // note CRC is calculated on frames wich 
                                                                    // sizes not match a USB FPGAConfigurationData
                                                                    // packet. The CRC error could happen in the middle
                                                                    // of this packet.
        printf("Error in CRC check\n");
        buf[0] = 0x00;                                              // Send error response
        D12_WriteEndpoint(5, buf, 1);
        fpgaConfigState = FCS_IDLE;
        return;
    }
    buf[0] = 0x41;                                                  // Send command response
    D12_WriteEndpoint(5, buf, 1);
}

void TerminateFPGAConfiguration( void )
{
    char    i, j;
    char    count = 0;
    
    D12_CS = 1;                                                     // After configuration D12_CS is supplied by the FPGA.
                                                                    // CPU D12_CS must therefore be tied high. 
    
    if (fpgaConfigState != FCS_CONFIGURING) {
        buf[0] = 0x00;                                              // Send error response
        D12_WriteEndpoint(5, buf, 1);
        fpgaConfigState = FCS_IDLE;
        return;
    }
    for (i=1 ; i<49 ; i++) {                                        // 48 bytes of data per packet
        P0 = buf[i];
        count = 0;
        while(count < 254) count++;
        CCLK = 1;
        count = 0;
        while(count < 5) count++;
        if (!RDY_BUSY_N) {                                          // Did we not get an ack?
            printf("Error, no data acknowledge\n");
            buf[0] = 0x00;                                          // Send error response
            D12_WriteEndpoint(5, buf, 1);
            fpgaConfigState = FCS_IDLE;
            printf("i = %d\n", i);
            if (INIT_N) {
                printf("INIT_N is high\n");
            } else {
                printf("INIT_N is low\n");
            }
            return;
        }
        CCLK = '0';
        count = 0;
        while(count < 5) count++;
        for (j=0 ; j<7 ; j++) {
            CCLK = '1';                                             // Seven more clock pulses to clock out serial data
            count = 0;
            while(count < 5) count++;
            CCLK = '0';
            count = 0;
            while(count < 5) count++;
        }
    }
    
    if (!INIT_N) {                                                   // Is INIT_N low indicating a CRC error
                                                                    // note CRC is calculated on frames wich 
                                                                    // sizes not match a USB FPGAConfigurationData
                                                                    // packet. The CRC error could happen in the middle
                                                                    // of this packet.
        printf("Error in CRC check\n");
        buf[0] = 0x00;                                              // Send error response
        D12_WriteEndpoint(5, buf, 1);
        fpgaConfigState = FCS_IDLE;
        return;
    }

    i = 0;
    while ((i<255) && (!DONE)) {                                    // Continue cycle clk until DONE goes high.
        CCLK = '1'; 
        count = 0;
        while(count < 5) count++;
        CCLK = '0';
        count = 0;
        while(count < 5) count++;
        i++;
    }
    
    if (!DONE) {                                                    // Timeout after 255 tries.
        printf("Error: DONE did not go high\n");
        if (INIT_N) {
            printf("INIT_N is high\n");
        } else {
            printf("INIT_N is low\n");
        }
        
        buf[0] = 0x00;                                              // Send error response
        D12_WriteEndpoint(5, buf, 1);
        fpgaConfigState = FCS_IDLE;
        return;
    }
    
    RW_SOURCE_FPGA = 1;                                             // With FPGA configured the RD and WR signals 
                                                                    // originating from the CPU must be disable
    
    i = 0;
    while ((i<255) && (ID_REG != 0x55)) {                           // Continue cycle clk until connection with FPGA is established.
        CCLK = '1'; 
        count = 0;
        while(count < 5) count++;
        CCLK = '0';
        count = 0;
        while(count < 5) count++;
        i++;
    }

    if (ID_REG != 0x55) {
        printf("Error: Could not reach FPGA\n");
        printf("FR_ID_L: %x\n", ID_REG);
        buf[0] = 0x00;                                              // Send error response
        D12_WriteEndpoint(5, buf, 1);
        fpgaConfigState = FCS_IDLE;
        return;
    }

    CCLK = 1;      

    buf[0] = 0x42;                                                  // Send command response
    D12_WriteEndpoint(5, buf, 1);
    fpgaConfigState = FCS_IDLE;
}

interrupt [0x03] void EX0_int (void)                        /* External Interrupt 0 */
{
    triggedInterrupt = STATUS_REG.field & 0x03;
    if (triggedInterrupt > 1) {
        D12_SetDMA(0xC0);                                       // DMA = 0 byte burst, Disabled, dir = 0, Reload = 0, irq mode = 0, irq enabled
        
//        printf("DMA TX Done\n");
//        buf[0] = 0xAA;
//        D12_WriteEndpoint(5, buf, 1);
//        printf("Send data\n");
    }
}

