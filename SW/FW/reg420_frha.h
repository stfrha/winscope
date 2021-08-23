/*--------------------------------------------------------------------------
REG420.H

Header file for Dallas 80C420.
Copyright (c) 1988-2002 Keil Elektronik GmbH and Keil Software, Inc.
All rights reserved.

Date    Sign    Change
030128  FRHA    Added interrupt vectors from file io320.h of IAR. Changed 
                all sbit to bit for IAR compatability.
--------------------------------------------------------------------------*/

#ifndef __REG420_H__
#define __REG420_H__

/*  BYTE Registers  */
sfr P0        = 0x80;
sfr SP        = 0x81;
sfr DPL     = 0x82;
sfr DPH     = 0x83;
sfr DPL1    = 0x84;
sfr DPH1    = 0x85;
sfr DPS     = 0x86;
sfr PCON    = 0x87;
sfr TCON    = 0x88;
sfr TMOD    = 0x89;
sfr TL0     = 0x8A;
sfr TL1     = 0x8B;
sfr TH0     = 0x8C;
sfr TH1     = 0x8D;
sfr CKCON   = 0x8E;
sfr P1        = 0x90;
sfr EXIF    = 0X91;
sfr CKMOD   = 0x96;
sfr SCON0   = 0x98;
sfr SBUF0   = 0x99;
sfr ACON    = 0x9D;
sfr P2        = 0xA0;
sfr IE      = 0xA8;
sfr SADDR0  = 0xA9;
sfr SADDR1  = 0xAA;
sfr P3        = 0xB0;
sfr IP1     = 0xB1;
sfr IP0     = 0xB8;
sfr SADEN0  = 0xB9;
sfr SADEN1  = 0xBA;
sfr SCON1   = 0xC0;
sfr SBUF1   = 0xC1;
sfr ROMSIZ  = 0xC2;
sfr PMR     = 0xC4;
sfr STATUS  = 0xC5;
sfr TA      = 0xC7;
sfr T2CON   = 0xC8;
sfr T2MOD   = 0xC9;
sfr RCAP2L  = 0xCA;
sfr RCAP2H  = 0xCB;
sfr TL2     = 0xCC;
sfr TH2     = 0xCD;
sfr PSW     = 0xD0;
sfr FCNTL   = 0xD5;
sfr FDATA   = 0xD6;
sfr WDCON   = 0xD8;
sfr ACC     = 0xE0;
sfr EIE     = 0xE8;
sfr B        = 0xF0;
sfr EIP1    = 0xF1;
sfr EIP0    = 0xF8;
/*                 */
/*  BIT Registers  */
/*  TCON */
bit IT0    = 0x88;
bit IE0    = 0x89;
bit IT1    = 0x8A;
bit IE1    = 0x8B;
bit TR0    = 0x8C;
bit TF0    = 0x8D;
bit TR1    = 0x8E;
bit TF1    = 0x8F;

/* SCON0 */ 
bit RI_0    = 0x98;
bit TI_0    = 0x99;
bit RB8_0   = 0x9A;
bit TB8_0   = 0x9B;
bit REN_0   = 0x9C;
bit SM2_0   = 0x9D;
bit SM1_0   = 0x9E;
bit SM0FE_0 = 0x9F;

/* IE     */
bit EX0    = 0xA8;
bit ET0    = 0xA9;
bit EX1    = 0xAA;
bit ET1    = 0xAB;
bit ES0    = 0xAC;
bit ET2    = 0xAD;
bit ES1    = 0xAE;
bit EA     = 0xAF;

/*  IP0    */
bit LPX0   = 0xB8;
bit LPT0   = 0xB9;
bit LPX1   = 0xBA;
bit LPT1   = 0xBB;
bit LPS0   = 0xBC;
bit LPT2   = 0xBD;
bit LPS1   = 0xBE;

/*  SCON1  */
bit RI_1    = 0xC0;
bit TI_1    = 0xC1;
bit RB8_1   = 0xC2;
bit TB8_1   = 0xC3;
bit REN_1   = 0xC4;
bit SM2_1   = 0xC5;
bit SM1_1   = 0xC6;
bit SM0FE_1 = 0xC7;

/* T2CON */
bit CP_RL2 = 0xC8;
bit C_T2   = 0xC9;
bit TR2    = 0xCA;
bit EXEN2  = 0xCB;
bit TCLK   = 0xCC;
bit RCLK   = 0xCD;
bit EXF2   = 0xCE;
bit TF2    = 0xCF;

/* PSW    */
bit P        = 0xD0;
bit F1     = 0xD1;
bit OV     = 0xD2;
bit RS0    = 0xD3;
bit RS1    = 0xD4;
bit F0     = 0xD5;
bit AC     = 0xD6;
bit CY     = 0xD7;

/*  WDCON   */
bit RWT    = 0xD8;
bit EWT    = 0xD9;
bit WTRF   = 0xDA;
bit WDIF   = 0xDB;
bit PFI    = 0xDC;
bit EPFI   = 0xDD;
bit POR    = 0xDE;
bit SM0D_1 = 0xDF;

/*  EIE   */
bit EX2    = 0xE8;
bit EX3    = 0xE9;
bit EX4    = 0xEA;
bit EX5    = 0xEB;
bit EWDI   = 0xEC;

/*  EIP0   */
bit LPX2   = 0xF8;
bit LPX3   = 0xF9;
bit LPX4   = 0xFA;
bit LPPX5  = 0xFB;
bit LPWDI  = 0xFC;

/*==============================*/
/* Interrupt Vector Definitions */
/*==============================*/

interrupt [0x03] void EX0_int (void);      /* External Interrupt 0 */

interrupt [0x0B] void T0_int (void);      /* Timer 0 Overflow */

interrupt [0x13] void EX1_int (void);      /* External Interrupt 1 */

interrupt [0x1B] void T1_int (void);      /* Timer 1 Overflow */

interrupt [0x23] void SCON0_int (void);   /* Serial Port 0 */

interrupt [0x2B] void T2_int (void);      /* Timer 2 Overflow */

interrupt [0x33] void PFI_int (void);     /* Power Fail Interrupt */

interrupt [0x3B] void SCON1_int (void);   /* Serial Port 1 */

interrupt [0x43] void EX2_int (void);     /* External Interrupt 2 */

interrupt [0x4B] void EX3_int (void);     /* External Interrupt 3 */

interrupt [0x53] void EX4_int (void);     /* External Interrupt 4 */

interrupt [0x5B] void EX5_int (void);     /* External Interrupt 5 */

interrupt [0x63] void WDTI_int (void);    /* Watchdog Time-out Interrupt */

/* end */

#endif
