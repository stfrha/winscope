// UsbDriver.h: interface for the CUsbDriver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USBDRIVER_H__768B51EA_CCE1_4EB5_BF41_6C639F0D6E5B__INCLUDED_)
#define AFX_USBDRIVER_H__768B51EA_CCE1_4EB5_BF41_6C639F0D6E5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "devioctl.h"

#include <setupapi.h>
#include <basetyps.h>
//#include "BulkUsr.h"

//#include "usbdi.h"

class CUsbDriver  
{
public:
	CUsbDriver();
	virtual ~CUsbDriver();

private:
	char	m_completeDeviceName[256];
	CString	m_msg;

	HANDLE	OpenOneDevice(IN HDEVINFO HardwareDeviceInfo, IN PSP_DEVICE_INTERFACE_DATA DeviceInfoData, IN char *devName);
	HANDLE	OpenUsbDevice( LPGUID  pGuid, char *outNameBuf);
	BOOL	GetUsbDeviceFileName( LPGUID  pGuid, char *outNameBuf);
public:
	HANDLE	open_file( char *filename);

};

#endif // !defined(AFX_USBDRIVER_H__768B51EA_CCE1_4EB5_BF41_6C639F0D6E5B__INCLUDED_)
