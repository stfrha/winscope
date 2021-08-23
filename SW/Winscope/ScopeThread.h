#if !defined(____scopeThread____)
#define ____scopeThread____

#include <afxtempl.h>
#include "PodLabel.h"
#include "RegisterIF.h"
#include "WsSetting.h"

// Winscope user-defined messages
#define WM_USER_SAY_HELLO			(WM_USER + 0x1000)
#define WM_USER_ERROR				(WM_USER + 0x1001)
#define WM_USER_COMMAND_COMPLETE	(WM_USER + 0x1002)
#define WM_USER_FW_VERSIONS			(WM_USER + 0x1003)
#define WM_USER_THREAD_ABORTED		(WM_USER + 0x1004)
#define WM_USER_NEW_DATA_AVAILABLE	(WM_USER + 0x1005)
#define WM_USER_THREAD_TERMINATED	(WM_USER + 0x1006)
#define WM_USER_HW_NOT_CONFIGURED	(WM_USER + 0x1007)
#define WM_USER_SWEEP_STATE_ARMED	(WM_USER + 0x1008)
#define WM_USER_SWEEP_STATE_FULL	(WM_USER + 0x1009)

// Winscope commands
#define WSC_NO_COMMAND			0x00
#define	WSC_CONFIG				0x01
#define	WSC_WRITE_SETTING		0x02
#define	WSC_READ_SETTING		0x03
#define WSC_SWEEP_SINGLE		0x04
#define WSC_SWEEP_NORMAL		0x05
#define WSC_SWEEP_AUTO			0x06
#define WSC_SAY_HELLO			0x07
#define WSC_FW_VERSIONS			0x08
#define WSC_INFINITE_LOOP		0x09
#define WSC_UPDATE_STATE		0x0A	// Update state variable of commPacket struct
#define WSC_RESET_SWEEP			0x0B	// Sets SWEEP_STATE to Idle
#define WSC_TERMINATE_THREAD	0x0C	// Sets SWEEP_STATE to Idle

// ScopeThread result codes
#define ST_SUCCESS				0x00
#define ST_ILLEGAL_RESPONSE		0x01
#define ST_READFILE_ERROR		0x02
#define ST_COULD_NOT_OPEN_USB	0x03


// ScopeThread soft auto states
#define ST_SAS_AUTO				0x01
#define ST_SAS_NORMAL			0x02


/*
typedef struct _WS_SETTING {
	TRIGG_SETUP_TYPE	triggSetup;
	unsigned char		prbATriggLevel;
	unsigned char		prbBTriggLevel;
	unsigned char		podPTriggPtrn;
	unsigned char		podQTriggPtrn;
	unsigned char		podPPtrnValid;
	unsigned char		podQPtrnValid;
	unsigned char		podPEdgeSlope;
	unsigned char		podQEdgeSlope;
	unsigned char		podPEdgeValid;
	unsigned char		podQEdgeValid;
	unsigned long		triggDelay;
	unsigned long		bufferSize;
	unsigned long		clockToggle;
	unsigned long		clockReset;
	CONTROL_TYPE		control;
	PRB_POD_CTRL_TYPE	prbPodCntrl;
	PRB_GAIN_TYPE		prbGain;
	unsigned char		leds;
	CTypedPtrList<CObList, CPodLabel*>*	symbolList;
} WS_SETTING;
*/

typedef struct _WS_ID_VER {
	unsigned char*		cpuVersionString;
	unsigned char		fpgaId;
	unsigned char		fpgaVersion;
} WS_ID_VER;

typedef struct _WS_STATE {
	STATUS_TYPE			status;
} WS_STATE;

typedef struct _WS_COMM_PACKET {
	int					commandType;
	BOOL				threadBusy;
	BOOL				loopAbort;
	BOOL				waitTriggerAbort;
	BOOL				updatingBuffers;
	CWsSetting*			settings;
	CString*			debugString;
	WS_STATE*			state;
	WS_ID_VER*			fwIdVersions;
	BOOL				dataValid;
	unsigned char*		dmaBuffer;
	unsigned char*		prbAData;
	unsigned char*		prbBData;
	unsigned char*		podPData;
	unsigned char*		podQData;
	CWnd*				msgReceiver;
	CString				configurationFile;
} WS_COMM_PACKET;

#define ST_BITFILE_ITERATIONS	1325



void			GotoIdleState(WS_COMM_PACKET*	commPacket);
void			IssueNextState( void );
unsigned char	WriteRegister(unsigned char address, unsigned char data);
unsigned char	ReadRegister(unsigned char address, unsigned char* data);
unsigned char	GetCPUVersion(unsigned char* dest);
unsigned long	DMATransfer(WS_COMM_PACKET*	commPacket);
void			CopyDmaDataToBuffer(WS_COMM_PACKET*	commPacket);
	

UINT scopeThreadCallback( LPVOID pParam );




#endif
