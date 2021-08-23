/*
   //*************************************************************************
   //
   //                  P H I L I P S   P R O P R I E T A R Y
   //
   //           COPYRIGHT (c)   1997 BY PHILIPS SINGAPORE.
   //                     --  ALL RIGHTS RESERVED  --
   //
   // File Name:        EPPHAL.C
   // Author:           Wenkai Du
   // Created:          8 Jun 98
   // Modified:
   // Revision:         2.0
   //
   //*************************************************************************
   //
   //*************************************************************************
   */

#include <stdio.h>

#ifdef __C51__

#include <reg51.h>                /* special function register declarations   */
#include "epphal.h"
#include "d12ci.h"
#include "mainloop.h"

extern EPPFLAGS bEPPflags;

void outportb(unsigned char port, unsigned char val)
{
	unsigned char xdata *ext_address;

	MCU_HOSTDACK =1;

	ext_address=0xff00 + port;
	*ext_address = val;

	MCU_HOSTDACK =0;
}

unsigned char inportb(unsigned char port)
{
	unsigned char c;
	unsigned char xdata *ext_address;

	MCU_HOSTDACK =1;

	ext_address=0xff00 + port;
	c = *ext_address ;

	MCU_HOSTDACK =0;

	return c;
}

void eppAwrite(unsigned char A_data)
{
	 MCU_HOSTDACK =1;

	 if(bEPPflags.bits.verbose)
		printf("eppAwrite = 0x%bx.\n", A_data);

	 P0 = (A_data | 0x80);

	 EPP_WRITE = 0;

	 EPP_DSTROBE = 1;
	 EPP_ASTROBE = 1;
	 EPP_ASTROBE = 0;
	 EPP_ASTROBE = 1;

	 P0 = (A_data & 0x7F);

	 EPP_ASTROBE = 0;
	 EPP_ASTROBE = 1;

	 MCU_HOSTDACK =0;
}

void program_cpld(unsigned short uSize, unsigned char bCommand)
{
	MCU_HOSTDACK = 1;

	outportb(CPLD_CNT0, LSB(uSize));   // set CPLD counter
	outportb(CPLD_CNT1, MSB(uSize));

	if(bEPPflags.bits.verbose)
		printf("CPLD counter read back = 0x%bx, 0x%bx.\n", inportb(CPLD_CNT0),
			inportb(CPLD_CNT1));

	if(bCommand & 0x1) {
		MCU_DMARD_WR = 0;
		EPP_WRITE = 1;     // IN_TOKEN_DMA;
	}
	else {
		MCU_DMARD_WR = 1;
		EPP_WRITE = 0;     // OUT_TOKEN_DMA;
	}

	MCU_HOSTDACK = 0;
}

void dma_start(PIO_REQUEST pio)
{
	eppAwrite( (pio->uAddressL >> 13) +  (pio->bAddressH << 3) );
	program_cpld(pio->uSize, pio->bCommand);
}

#endif
