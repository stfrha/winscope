/*------------------------------------------------------------------------------
HELLO.C

Copyright 1995-2002 Keil Software, Inc.
------------------------------------------------------------------------------*/

#include <REG420.H>               /* special function register declarations   */
                                  /* for the intended 8051 derivative         */

#include <stdio.h>                /* prototype declarations for I/O functions */


#ifdef MONITOR                           /* Debugging with Monitor-51 needs   */
char code reserve [3] _at_ 0x23;         /* space for serial interrupt if     */
#endif                                   /* Stop Exection with Serial Intr.   */
                                         /* is enabled                        */


/*------------------------------------------------
The main C function.  Program execution starts
here after stack initialization.
------------------------------------------------*/
void main (void) {

unsigned int counter;

/*------------------------------------------------
Setup the serial port for 19200 baud at 22.1MHz.
------------------------------------------------*/
#ifndef MONITOR
    SCON0  = 0x50; 		        /* SCON0: mode 1, 8-bit UART, enable rcvr       */
    TMOD |= 0x20;             /* TMOD: timer 1, mode 2, 8-bit reload          */
    TH1   = 0xFD;            /* TH1:  reload value for 19200 baud @ 22.100 MHz*/
    TR1   = 1;                /* TR1:  timer 1 run                            */
    TI_0  = 1;                /* TI_0:   set TI to send first char of UART    */
#endif

/*------------------------------------------------
Note that an embedded program never exits (because
there is no operating system to return to).  It
must loop and execute forever.
------------------------------------------------*/
  while (1) {
    
    for ( counter = 1; counter <= 1000; counter++ )  {
      
      P1 ^= 0x01;     		                    /* Toggle P1.0 each time we print */
      printf ("\ncounter = %d", counter );    /* Print the context of counter   */
    }      
    
  }
}


