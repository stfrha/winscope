/*
   //*************************************************************************
   //
   //                  P H I L I P S   P R O P R I E T A R Y
   //
   //           COPYRIGHT (c)   1997 BY PHILIPS SINGAPORE.
   //                     --  ALL RIGHTS RESERVED  --
   //
   // File Name:	PROTODMA.C
   // Author:		Wenkai Du
   // Created:		18 Aug 98
   // Modified:
   // Revision:		2.0
   //
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

extern CONTROL_XFER ControlData;
extern IO_REQUEST idata ioRequest;
extern EPPFLAGS bEPPflags;

void get_firmware_version()
{
	unsigned char i;

	i = 0x21; // firmware version number, return 0x11 for PC kit using 8052, 0x21 for USB-EPP
	single_transmit((unsigned char *)&i, 1);
}

void setup_dma_request()
{
	memcpy((unsigned char *)&ioRequest + ControlData.DeviceRequest.wValue,
		ControlData.dataBuffer,
		ControlData.DeviceRequest.wLength);

	ioRequest.uSize = SWAP(ioRequest.uSize);
	ioRequest.uAddressL = SWAP(ioRequest.uAddressL);

	if(ioRequest.uSize > DMA_BUFFER_SIZE) { // Unaccepted request
		stall_ep0();
	}
	else {

		if(bEPPflags.bits.dma_state == DMA_IDLE) {
			DISABLE;
			bEPPflags.bits.setup_dma = 1;
			ENABLE;
		}
		else {
			DISABLE;
			bEPPflags.bits.dma_state = DMA_PENDING;
			ENABLE;
		}

	} // else if accepted request
}

void read_write_register(void)
{
	unsigned char i;

	if(ControlData.DeviceRequest.bmRequestType & (unsigned char)USB_ENDPOINT_DIRECTION_MASK) {

		if(bEPPflags.bits.verbose)
			printf("Read Registers: Offset = 0x%x, Length = 0x%x, Index = 0x%x.\n",
				ControlData.DeviceRequest.wValue,
				ControlData.DeviceRequest.wLength,
				ControlData.DeviceRequest.wIndex);

		if(ControlData.DeviceRequest.wIndex == GET_FIRMWARE_VERSION &&
			ControlData.DeviceRequest.wValue == 0 &&
			ControlData.DeviceRequest.wLength == 1)
			get_firmware_version();
		else
			stall_ep0();

	}	// if read register
	else{
		if(bEPPflags.bits.verbose) {

			printf("Write Registers: Offset = 0x%x, Length = 0x%x, Index = 0x%x.\n",
				ControlData.DeviceRequest.wValue,
				ControlData.DeviceRequest.wLength,
				ControlData.DeviceRequest.wIndex);

			printf("Data: ");
			for(i = 0; i < ControlData.DeviceRequest.wLength; i ++)
				printf("0x%bx, ", *((ControlData.dataBuffer)+i));
			printf("\n");
		}

		if(ControlData.DeviceRequest.wIndex == SETUP_DMA_REQUEST &&
			ControlData.DeviceRequest.wValue == 0 &&
			ControlData.DeviceRequest.wLength == 6)
			setup_dma_request();
		else
			stall_ep0();
	}	// if write register
}
