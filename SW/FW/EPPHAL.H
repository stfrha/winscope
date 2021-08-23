/*
   //*************************************************************************
   //
   //                  P H I L I P S   P R O P R I E T A R Y
   //
   //           COPYRIGHT (c)   1997 BY PHILIPS SINGAPORE.
   //                     --  ALL RIGHTS RESERVED  --
   //
   // File Name:        EPPHAL.H
   // Author:           Wenkai Du
   // Created:          19 Dec 97
   // Modified:
   // Revision:         2.1
   //
   //*************************************************************************
   //
   // 98/11/27			Move part of defination from D12CI.H (WK)
   //*************************************************************************
   */


#ifndef __EPPHAL_H__
#define __EPPHAL_H__


	#define portbase 2

	#define D12_COMMAND (portbase + 1)
	#define D12_DATA portbase

	#define CPLD_CNT0 0x00
	#define CPLD_CNT1 0x01

	#define COUNT_L   0x00
	#define COUNT_H   0x00
	#define EPP_PORT  P1
	#define DATA_BUS  P0


	sbit MCU_HOSTDREQ = P2^0;
	sbit MCU_HOSTDACK = P2^1;
	sbit MCU_EPPINIT  = P2^2;
	sbit MCU_WHICH_PW = P2^3;
	sbit MCU_DMARD_WR = P2^4;
	sbit MCU_SWM1     = P2^5;
	sbit MCU_SWM2     = P2^6;
	sbit MCU_ALE1     = P2^7;

	sbit EPP_WRITE    = P1^0;
	sbit EPP_DSTROBE  = P1^1;
	sbit EPP_ASTROBE  = P1^2;
	sbit EPP_WAIT     = P1^3;
	sbit EPP_INTR     = P1^4;
	sbit EPP_PE       = P1^5;
	sbit EPP_ERROR    = P1^6;
	sbit EPP_SELECT   = P1^7;

	sbit MCU_LED0	  = P3^4;
	sbit MCU_LED1	  = P3^5;

	#define DISABLE		EA=0
	#define ENABLE		EA=1

	void outportb(unsigned char port, unsigned char val);
	unsigned char inportb(unsigned char port);

#endif
