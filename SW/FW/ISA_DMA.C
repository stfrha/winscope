/*
   //*************************************************************************
   //
   //                  P H I L I P S   P R O P R I E T A R Y
   //
   //           COPYRIGHT (c)   1997 BY PHILIPS SINGAPORE.
   //                     --  ALL RIGHTS RESERVED  --
   //
   // File Name:	ISADMA.C
   // Author:		Wenkai Du
   // Created:		16 Mar 97
   // Modified:
   // Revision:		1.1
   //
   //*************************************************************************
   //
   //*************************************************************************
   */

#include <stdio.h>
#include <dos.h>
#include <bios.h>
#include <mem.h>

#include "mainloop.h"

#define	DMA37MD_SINGLE	0x40	/* single pass mode */
#define	DMA37MD_BLOCK	0x80	/* single pass mode */
#define	DMA37MD_AUTO	0x10	/* autoinitialise single pass mode */
#define	DMA37MD_DEMAND	0x00	/* demand mode */
#define	DMA37MD_WRITE	0x04	/* read the device, write memory operation */
#define	DMA37MD_READ	0x08	/* write the device, read memory operation */

/*
**  Register definitions for DMA controller 1 (channels 0..3):
*/
#define	DMA1_CHN(c)	(2*(c))	/* addr reg for channel c */
#define DMA1_CMD    (8)     /* command register */
#define	DMA1_SMSK	(10)	/* single mask register */
#define	DMA1_MODE	(11)	/* mode register */
#define	DMA1_FFC	(12)	/* clear first/last FF */

#define VALID_DMA_MASK (7)

extern EPPFLAGS bEPPflags;
extern unsigned long dmaBuffer;

void dma_start(PIO_REQUEST pio)
{
	static short pageport[8] = { 0x87, 0x83, 0x81, 0x82, 0x8f, 0x8b, 0x89, 0x8a };
	unsigned short dmaport, nbytes, ch;
	unsigned long offset, buf_phy_addr;
	BOOL b_read;

	offset = (((unsigned long)pio->bAddressH)<<16)
		+ pio->uAddressL;

	b_read = pio->bCommand & 0x1;
	buf_phy_addr = dmaBuffer + offset;
	nbytes = pio->uSize;
	ch = 3;

	if(bEPPflags.bits.in_isr == 0) {
		if(b_read)
			printf("DMA channel %d IN token DMA start at %lx of %u bytes.\n", ch, buf_phy_addr, nbytes);
		else
			printf("DMA channel %d OUT token DMA start at %lx of %u bytes.\n", ch, buf_phy_addr, nbytes);
	}

	if ((ch & 4) == 0) {

		if (b_read)
			outportb(DMA1_MODE, DMA37MD_DEMAND|DMA37MD_READ|ch);
//			outportb(DMA1_MODE, DMA37MD_SINGLE|DMA37MD_READ|ch);
		else
			outportb(DMA1_MODE, DMA37MD_DEMAND|DMA37MD_WRITE|ch);
//			outportb(DMA1_MODE, DMA37MD_SINGLE|DMA37MD_WRITE|ch);

		outportb(DMA1_FFC, 0);

		/* send start address */
		dmaport =  DMA1_CHN(ch);
		outportb(dmaport, buf_phy_addr);
		outportb(dmaport, buf_phy_addr>>8);
		outportb(pageport[ch], buf_phy_addr>>16);

		/* send count */
		outportb(DMA1_FFC, 0);
		outportb(dmaport + 1, --nbytes);
		outportb(dmaport + 1, nbytes>>8);

		outportb(DMA1_CMD, 0x0);

		/* unmask channel */
		outportb(DMA1_SMSK, ch);
	}
}

int dma_getcount(int ch)
{
	unsigned short dmaport;
	unsigned short highbyte, lowbyte;

	dmaport =  DMA1_CHN(ch);
	outportb(DMA1_FFC, 0);

	lowbyte = inportb(dmaport + 1);
	highbyte = inportb(dmaport + 1);
	highbyte <<= 8;
	highbyte += lowbyte;

	return highbyte;
}
