/*
   *************************************************************************
   
                     P H I L I P S   P R O P R I E T A R Y
   
              COPYRIGHT (c)   1997 BY PHILIPS SINGAPORE.
                        --  ALL RIGHTS RESERVED  --
   
    File Name:    CHAP_9.H
    Author:        Wenkai Du
    Created:        19 Dec 97
    Modified:
    Revision:        2.0
   
   *************************************************************************
   
   *************************************************************************
*/


#ifndef __CHAP_9_H__
#define __CHAP_9_H__

/*
   *************************************************************************
    USB Protocol Layer
   *************************************************************************
*/

/*
   *************************************************************************
    USB standard device requests
   *************************************************************************
*/
void get_status(void);
void clear_feature(void);
void set_feature(void);
void set_address(void);
void get_descriptor(void);
void get_configuration(void);
void set_configuration(void);
void get_interface(void);
void set_interface(void);

void reserved(void);

#endif
