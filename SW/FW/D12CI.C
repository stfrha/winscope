/*
   *************************************************************************
   
                     P H I L I P S   P R O P R I E T A R Y
   
              COPYRIGHT (c)   1997 BY PHILIPS SINGAPORE.
                        --  ALL RIGHTS RESERVED  --
   
    File Name:    D12CI.C
    Author:        Wenkai Du
    Created:        8 Jun 98
    Modified:
    Revision:        2.2
   
   *************************************************************************
   
    98/11/27      I/O mode Main endpoints read/write update (WK)
    98/12/2       Added D12_ReadMainEndpoint (WK)
   *************************************************************************
   */
#include <stdio.h>
#include "reg420_frha.h"
#include "USBTest.h"
#include "d12ci.h"

extern EPPFLAGS bEPPflags;

void D12_SetAddressEnable(unsigned char bAddress, unsigned char bEnable)
{
    if (bEPPflags.bits.in_isr == 0) DISABLE;
 
    D12_CMND = 0xD0;
    WaitBetween();
    if (bEnable) bAddress |= 0x80;
    D12_DATA = bAddress;

    if (bEPPflags.bits.in_isr == 0) ENABLE;
}

void D12_SetEndpointEnable(unsigned char bEnable)
{
    if (bEPPflags.bits.in_isr == 0) DISABLE;
 
    D12_CMND = 0xD8;
    WaitBetween();
    if(bEnable)
        D12_DATA = 1;
    else
        D12_DATA = 0;

    if (bEPPflags.bits.in_isr == 0) ENABLE;
}

void D12_SetMode(unsigned char bConfig, unsigned char bClkDiv)
{
    if (bEPPflags.bits.in_isr == 0) DISABLE;
 
    D12_CMND = 0xF3;
    WaitBetween();
    WaitBetween();
    D12_DATA = bConfig;
    WaitBetween();
    WaitBetween();
    D12_DATA = bClkDiv;

    if (bEPPflags.bits.in_isr == 0) ENABLE;
}

void D12_SetDMA(unsigned char bMode)
{
    if (bEPPflags.bits.in_isr == 0) DISABLE;
 
    D12_CMND = 0xFB;
    WaitBetween();
    D12_DATA = bMode;

    if (bEPPflags.bits.in_isr == 0) ENABLE;
}

unsigned short D12_ReadInterruptRegister(void)
{
    unsigned char b1;
    unsigned int j;

    D12_CMND = 0xF4;
    WaitBetween();
    b1 = D12_DATA;
    WaitBetween();
    j = D12_DATA;

    j <<= 8;
    j += b1;

    return j;
}

unsigned char D12_SelectEndpoint(unsigned char bEndp)
{
    unsigned char c;

    if (bEPPflags.bits.in_isr == 0) DISABLE;
 
    D12_CMND = bEndp;
    WaitBetween();
    c = D12_DATA;

    if (bEPPflags.bits.in_isr == 0) ENABLE;
 
    return c;
}

unsigned char D12_ReadLastTransactionStatus(unsigned char bEndp)
{
    D12_CMND = 0x40 + bEndp;
    WaitBetween();
    return D12_DATA;
}

unsigned char D12_ReadEndpointStatus(unsigned char bEndp)
{
    unsigned char c;

    if (bEPPflags.bits.in_isr == 0) DISABLE;
 
    D12_CMND = 0x80 + bEndp;
    WaitBetween();
    c = D12_DATA;

    if (bEPPflags.bits.in_isr == 0) ENABLE;
 
    return c;
}

void D12_SetEndpointStatus(unsigned char bEndp, unsigned char bStalled)
{
    if (bEPPflags.bits.in_isr == 0) DISABLE;
 
    D12_CMND = 0x40 + bEndp;
    WaitBetween();
    D12_DATA = bStalled;

    if (bEPPflags.bits.in_isr == 0) ENABLE;
}

void D12_SendResume(void)
{
    D12_CMND = 0xF6;
}

unsigned short D12_ReadCurrentFrameNumber(void)
{
    unsigned short a, j;

    if (bEPPflags.bits.in_isr == 0) DISABLE;
 
    D12_CMND = 0xF5;
    WaitBetween();
    a = D12_DATA;
    WaitBetween();
    j = D12_DATA;

    a += (j<<8);

    if (bEPPflags.bits.in_isr == 0) ENABLE;
 
    return a;
}

unsigned short D12_ReadChipID(void)
{
    unsigned short a, j;

    if (bEPPflags.bits.in_isr == 0) DISABLE;
 
    D12_CMND = 0xFD;
    WaitBetween();
    a = D12_DATA;
    WaitBetween();
    j = D12_DATA;

    a += (j<<8);

    if (bEPPflags.bits.in_isr == 0) ENABLE;
 
    return a;
}

unsigned char  D12_ReadEndpoint(unsigned char endp, unsigned char * buf, unsigned char len)
{
    unsigned char i, j;

    if (bEPPflags.bits.in_isr == 0) DISABLE;
 
    D12_CMND = endp;
    if ((D12_DATA & D12_FULLEMPTY) == 0)
    {
        if (bEPPflags.bits.in_isr == 0) ENABLE;
        return 0;
    }

    D12_CMND = 0xF0;
    WaitBetween();
    j = D12_DATA;
    WaitBetween();
    j = D12_DATA;

    if(j > len)
        j = len;

    for(i=0; i<j; i++) {
        WaitBetween();
        *(buf+i) = D12_DATA;
    }
        

    D12_CMND = 0xF2;

    if (bEPPflags.bits.in_isr == 0) ENABLE;
 
    return j;
}

// D12_ReadMainEndpoint() added by V2.2 to support double-buffering.
// Caller should assume maxium 128 bytes of returned data.
unsigned char D12_ReadMainEndpoint(unsigned char * buf)
{
    unsigned char i, j, k = 0, bDblBuf = 1;

    if (bEPPflags.bits.in_isr == 0) DISABLE;
 
    D12_CMND, 0x84;
    WaitBetween();
    if( (D12_DATA & 0x60) == 0x60)
        bDblBuf = 2;

    while(bDblBuf) {
        D12_CMND = 4;
        if ((D12_DATA & D12_FULLEMPTY) == 0)
            break;

        D12_CMND = 0xF0;
        WaitBetween();
        j = D12_DATA;
        WaitBetween();
        j = D12_DATA;

        for(i=0; i<j; i++) {
            WaitBetween();
            *(buf+i+k) = D12_DATA;
        }

        k += j;

        D12_CMND = 0xF2;

        bDblBuf --;
    }

    if (bEPPflags.bits.in_isr == 0) ENABLE;
 
    return k;
}

unsigned char D12_WriteEndpoint(unsigned char endp, unsigned char * buf, unsigned char len)
{
    unsigned char i;

    if (bEPPflags.bits.in_isr == 0) DISABLE;

    D12_CMND = endp;
    WaitBetween();
    i = D12_DATA;                                               /* Dummy read */
    WaitBetween();

    D12_CMND = 0xF0;
    WaitBetween();
    D12_DATA = 0;
    WaitBetween();
    D12_DATA = len;

    for(i=0; i<len; i++) {
        WaitBetween();
        D12_DATA = *(buf+i);
    }
        

    D12_CMND = 0xFA;

    if (bEPPflags.bits.in_isr == 0) ENABLE;

    return len;
}

void D12_AcknowledgeEndpoint(unsigned char endp)
{
    D12_CMND = endp;
    WaitBetween();
    D12_CMND = 0xF1;
    WaitBetween();
    if (endp == 0) D12_CMND = 0xF2;
}

void    WaitBetween( void )
{
    unsigned char i, j;
   
    for (i=0 ; i<1 ; i++) j = i;
}
