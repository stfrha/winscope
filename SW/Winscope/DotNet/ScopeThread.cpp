
#include "stdafx.h"
#include "Winscope.h"
#include "ScopeThread.h"
#include "UsbDriver.h"
#include "BitFileReader.h"
#include "ProgressDlg.h"

WS_COMM_PACKET*	commPacket;

UINT scopeThreadCallback( LPVOID pParam )
{
	CUsbDriver		usb;
	CBitFileReader	bitReader;
	HANDLE			txUsb;
	HANDLE			rxUsb;
	char			outPipe[32] = "PIPE03";
	char			inPipe[32] = "PIPE02";
	CProgressDlg	progressDlg;
	unsigned long	i, j;
	unsigned long	len;
	char			buf[64];
	unsigned char	c;
	BOOL			error = FALSE;
	PRB_POD_CTRL_TYPE	prbPodReg;

	commPacket = (WS_COMM_PACKET*) pParam;

	if (WSC_CONFIG != commPacket->commandType) {	// If HW is being configured this check can not be done
		switch (ReadRegister(ADR_ID, &c)) {
		case ST_COULD_NOT_OPEN_USB:
			commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Could not open USB port\nIs HW connected and power on?\nCheck and then try menu HW->Configure Hardware");
			while (commPacket->commandType != WSC_TERMINATE_THREAD);
			c = HWC_ID;								// Make sure if-statement below is false, not resulting in another error message
			break;
		case ST_READFILE_ERROR:
			commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Error reading from HW via USB");
			while (commPacket->commandType != WSC_TERMINATE_THREAD);
			c = HWC_ID;								// Make sure if-statement below is false, not resulting in another error message
			break;
		case ST_ILLEGAL_RESPONSE:
			commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Illegal response");
			while (commPacket->commandType != WSC_TERMINATE_THREAD);
			c = HWC_ID;								// Make sure if-statement below is false, not resulting in another error message
			break;
		};

		if (HWC_ID != c) {
			commPacket->msgReceiver->PostMessage(WM_USER_HW_NOT_CONFIGURED);
			while (commPacket->commandType != WSC_TERMINATE_THREAD);
		}

	}
	while (true) {

		while (WSC_NO_COMMAND == commPacket->commandType) {
			switch (ReadRegister(ADR_STATUS, &(commPacket->state->status.field))) {
			case ST_COULD_NOT_OPEN_USB:
				commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Could not open USB port\nIs HW connected and power on?\nCheck and then try menu HW->Configure Hardware");
				commPacket->threadBusy = FALSE;
				while (commPacket->commandType != WSC_TERMINATE_THREAD);
				break;
			case ST_READFILE_ERROR:
				commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Error reading from HW via USB");
				commPacket->threadBusy = FALSE;
				while (commPacket->commandType != WSC_TERMINATE_THREAD);
				break;
			case ST_ILLEGAL_RESPONSE:
				commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Illegal response");
				commPacket->threadBusy = FALSE;
				while (commPacket->commandType != WSC_TERMINATE_THREAD);
				break;
			};
		}

		commPacket->threadBusy = TRUE;

		switch (commPacket->commandType) {


		case WSC_SAY_HELLO:
			commPacket->msgReceiver->PostMessage(WM_USER_SAY_HELLO);
			break;


			
		case WSC_WRITE_SETTING:
			switch (WriteRegister(ADR_TRIGG_SETUP, commPacket->settings->m_triggSetup.field)) {
			case ST_COULD_NOT_OPEN_USB:
				commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Could not open USB port\nIs HW connected and power on?\nCheck and then try menu HW->Configure Hardware");
				commPacket->threadBusy = FALSE;
				error = TRUE;
				break;
			case ST_READFILE_ERROR:
				commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Error reading from HW via USB");
				commPacket->threadBusy = FALSE;
				error = TRUE;
				break;
			case ST_ILLEGAL_RESPONSE:
				commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Illegal response");
				commPacket->threadBusy = FALSE;
				error = TRUE;
				break;
			};
			if (error) break;
			// Now, assume all usb accesses will work, (a bit dangeruos, I know, but I can't fucking check everything)
			WriteRegister(ADR_TRIGG_SETUP, commPacket->settings->m_triggSetup.field);
			WriteRegister(ADR_PRBA_TRG_LEVEL, commPacket->settings->m_prbATriggLevel);
			WriteRegister(ADR_PRBB_TRG_LEVEL, commPacket->settings->m_prbBTriggLevel);
			WriteRegister(ADR_PODP_TRG_PTRN, commPacket->settings->m_podPTriggPtrn);
			WriteRegister(ADR_PODQ_TRG_PTRN, commPacket->settings->m_podQTriggPtrn);
			WriteRegister(ADR_PODP_PTRN_VALID, commPacket->settings->m_podPPtrnValid);
			WriteRegister(ADR_PODQ_PTRN_VALID, commPacket->settings->m_podQPtrnValid);
			WriteRegister(ADR_PODP_EDGE_SLOPE, commPacket->settings->m_podPEdgeSlope);
			WriteRegister(ADR_PODQ_EDGE_SLOPE, commPacket->settings->m_podQEdgeSlope);
			WriteRegister(ADR_PODP_EDGE_VALID, commPacket->settings->m_podPEdgeValid);
			WriteRegister(ADR_PODQ_EDGE_VALID, commPacket->settings->m_podQEdgeValid);
			WriteRegister(ADR_TRG_DELY_L, (unsigned char) commPacket->settings->m_triggDelay & 0xFF);
			WriteRegister(ADR_TRG_DELY_M, (unsigned char) (commPacket->settings->m_triggDelay >> 8) & 0xFF);
			WriteRegister(ADR_TRG_DELY_H, (unsigned char) (commPacket->settings->m_triggDelay >> 16) & 0x01);
			WriteRegister(ADR_BUFFER_SIZE_L, (unsigned char) commPacket->settings->m_bufferSize & 0xFF);
			WriteRegister(ADR_BUFFER_SIZE_M, (unsigned char) (commPacket->settings->m_bufferSize >> 8) & 0xFF);
			WriteRegister(ADR_BUFFER_SIZE_H, (unsigned char) (commPacket->settings->m_bufferSize >> 16) & 0x01);
			ReadRegister(ADR_CONTROL, &c);
			c = (commPacket->settings->m_control.field & 0x04) | ( c & 0x03) | 0x08;	// Make sure NEXT_STATE and LOCK_PRB_POD is zero, IRQ_EN active and SWEEP_MODE is unchanged
													// In other words: only CLK_SELECT will change according to the struct
			WriteRegister(ADR_CONTROL, c);
			WriteRegister(ADR_CLK_TOGGLE_L, (unsigned char) (commPacket->settings->m_clockToggle) & 0xFF);
			WriteRegister(ADR_CLK_TOGGLE_M, (unsigned char) (commPacket->settings->m_clockToggle >> 8) & 0xFF);
			WriteRegister(ADR_CLK_TOGGLE_H, (unsigned char) (commPacket->settings->m_clockToggle >> 16) & 0xFF);
			WriteRegister(ADR_CLK_RESET_L, (unsigned char) (commPacket->settings->m_clockReset) & 0xFF);
			WriteRegister(ADR_CLK_RESET_M, (unsigned char) (commPacket->settings->m_clockReset >> 8) & 0xFF);
			WriteRegister(ADR_CLK_RESET_H, (unsigned char) (commPacket->settings->m_clockReset >> 16) & 0xFF);
			WriteRegister(ADR_PRB_GAIN, commPacket->settings->m_prbGain.field);
			prbPodReg.field = commPacket->settings->m_prbPodCntrl.field;
			prbPodReg.bits.PODP_ACTIVE = 1;
			prbPodReg.bits.PODQ_ACTIVE = 1;
			prbPodReg.bits.PRBA_ACTIVE = 1;
			prbPodReg.bits.PRBB_ACTIVE = 1;
			WriteRegister(ADR_PRB_POD_CTRL, prbPodReg.field);
			WriteRegister(ADR_LEDS, commPacket->settings->m_leds);
			break;
		

			
		
		
		
		
		case WSC_CONFIG:
			if (commPacket->configurationFile.IsEmpty()) {
				commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "No configuration file specified");
				commPacket->threadBusy = FALSE;
				break;
			}
			txUsb = usb.open_file(outPipe);
			if (txUsb == INVALID_HANDLE_VALUE) {
				commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Could not open USB port\nIs HW connected and power on?\nCheck and then try menu HW->Configure Hardware");
				commPacket->threadBusy = FALSE;
				break;
			}
			rxUsb = usb.open_file(inPipe);
			if (rxUsb == INVALID_HANDLE_VALUE) {
				CloseHandle(txUsb);
				commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Could not open USB port\nIs HW connected and power on?\nCheck and then try menu HW->Configure Hardware");
				commPacket->threadBusy = FALSE;
				break;
			}
			progressDlg.Create(IDD_TEMP_PROGRESS, NULL);
			progressDlg.SetWindowText("Configuring HW");

			buf[0] = 0x40;
			WriteFile(txUsb, (LPCSTR) buf, 1, &len, NULL);

			if (ReadFile(rxUsb, buf, 64, &len, NULL)) {
				if (buf[0] != 0x40) {
					CloseHandle(txUsb);
					CloseHandle(rxUsb);
					commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Illegal response\nExpected 0x40");
					commPacket->threadBusy = FALSE;
					break;
				}
			} else {
				CloseHandle(txUsb);
				CloseHandle(rxUsb);
				commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Error reading from HW via USB");
				commPacket->threadBusy = FALSE;
				break;
			}
			if (bitReader.OpenBitFile(commPacket->configurationFile)) {
				progressDlg.m_progress.SetPos(0);
				progressDlg.m_progress.SetRange32(0, ST_BITFILE_ITERATIONS-1);

				for (i=0 ; i<ST_BITFILE_ITERATIONS ; i++) {				//1325*63=83475, *8=667800 bitar, 48 bytes till skickas vid terminering
					buf[0] = 0x41;
					for (j=1 ; j<64 ; j++) buf[j] = bitReader.GetNextByte();
					WriteFile(txUsb, (LPCSTR) buf, 64, &len, NULL);
					if (ReadFile(rxUsb, buf, 64, &len, NULL)) {
						if (buf[0] != 0x41) {
							CloseHandle(txUsb);
							CloseHandle(rxUsb);
							bitReader.CloseBitFile();
							commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Illegal response\nExpected 0x41");
							commPacket->threadBusy = FALSE;
							error = TRUE;
						}
					} else {
						CloseHandle(txUsb);
						CloseHandle(rxUsb);
						bitReader.CloseBitFile();
						commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Error reading from HW via USB");
						commPacket->threadBusy = FALSE;
						error = TRUE;
					}
					progressDlg.m_progress.SetPos(i);
				}
				if (error) break;
			} else {
				CloseHandle(txUsb);
				CloseHandle(rxUsb);
				bitReader.CloseBitFile();
				commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Could not open bit file");
				commPacket->threadBusy = FALSE;
				break;
			}

			buf[0] = 0x42;
			for (j=1 ; j<48 ; j++) buf[j] = bitReader.GetNextByte();
			WriteFile(txUsb, (LPCSTR) buf, 49, &len, NULL);
			if (ReadFile(rxUsb, buf, 64, &len, NULL)) {
				if (buf[0] != 0x42) {
					CloseHandle(txUsb);
					CloseHandle(rxUsb);
					bitReader.CloseBitFile();
					commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Illegal response\nExpected 0x42");
					commPacket->threadBusy = FALSE;
					break;
				}
			} else {
				CloseHandle(txUsb);
				CloseHandle(rxUsb);
				bitReader.CloseBitFile();
				commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Error reading from HW via USB");
				commPacket->threadBusy = FALSE;
				break;
			}
			
			progressDlg.DestroyWindow();
			CloseHandle(txUsb);
			CloseHandle(rxUsb);
			bitReader.CloseBitFile();
			commPacket->msgReceiver->PostMessage(WM_USER_COMMAND_COMPLETE);
			break;


		
		





		case WSC_FW_VERSIONS:
			switch (GetCPUVersion(commPacket->fwIdVersions->cpuVersionString)) {
			case ST_COULD_NOT_OPEN_USB:
				commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Could not open USB port\nIs HW connected and power on?\nCheck and then try menu HW->Configure Hardware");
				commPacket->threadBusy = FALSE;
				error = TRUE;
				break;
			case ST_READFILE_ERROR:
				commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Error reading from HW via USB");
				commPacket->threadBusy = FALSE;
				error = TRUE;
				break;
			case ST_ILLEGAL_RESPONSE:
				commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Illegal response");
				commPacket->threadBusy = FALSE;
				error = TRUE;
				break;
			};

			if (error) break;
			// Now, assume all usb accesses will work, (a bit dangeruos, I know)
			ReadRegister(ADR_ID, &commPacket->fwIdVersions->fpgaId);
			ReadRegister(ADR_REV, &commPacket->fwIdVersions->fpgaVersion);
			commPacket->msgReceiver->PostMessage(WM_USER_FW_VERSIONS);
			break;











		case WSC_INFINITE_LOOP:
			while (!commPacket->loopAbort) {
				MessageBeep(0xffffff);
				Sleep(5000);
			}
			break;

		
		
		
		
		case WSC_UPDATE_STATE:
			switch (ReadRegister(ADR_CONTROL, &(commPacket->state->status.field))) {
			case ST_COULD_NOT_OPEN_USB:
				commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Could not open USB port\nIs HW connected and power on?\nCheck and then try menu HW->Configure Hardware");
				commPacket->threadBusy = FALSE;
				break;
			case ST_READFILE_ERROR:
				commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Error reading from HW via USB");
				commPacket->threadBusy = FALSE;
				break;
			case ST_ILLEGAL_RESPONSE:
				commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Illegal response");
				commPacket->threadBusy = FALSE;
				break;
			};
			break;

			
			
			
			
			
		case WSC_RESET_SWEEP:
			GotoIdleState(commPacket);
			break;
		

			




			
			
			
		case WSC_SWEEP_SINGLE:
			*commPacket->debugString = "Going to Idle state";
//			MessageBox(NULL, "First goto Idle", "Hello", MB_OK);
			GotoIdleState(commPacket);
			*commPacket->debugString = "Starting sweep";
			IssueNextState();
			*commPacket->debugString = "Waiting for sweep complete...";
			do {


				switch (ReadRegister(ADR_STATUS, &(commPacket->state->status.field))) {
				case ST_COULD_NOT_OPEN_USB:
					commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Could not open USB port\nIs HW connected and power on?\nCheck and then try menu HW->Configure Hardware");
					commPacket->threadBusy = FALSE;
					error = TRUE;
					break;
				case ST_READFILE_ERROR:
					commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Error reading from HW via USB");
					commPacket->threadBusy = FALSE;
					error = TRUE;
					break;
				case ST_ILLEGAL_RESPONSE:
					commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Illegal response");
					commPacket->threadBusy = FALSE;
					error = TRUE;
					break;
				};

	/*
				if (ReadRegister(ADR_STATUS, &(commPacket->state->status.field)) != ST_SUCCESS) {
					MessageBox(NULL, "Error in wait loop", "Error", MB_OK);
					commPacket->loopAbort = TRUE;
				}
	*/
			
			
			} while ((!commPacket->loopAbort) && (WSC_SWEEP_SINGLE == commPacket->commandType) && (S_SS_FULL != commPacket->state->status.bits.SWEEP_STATE) && (!error));
			if (error) break;
			if (commPacket->loopAbort)  break;		// Was loop aborted?
			if (commPacket->commandType != WSC_SWEEP_SINGLE) break;
			// DMA transfer
//			MessageBox(NULL, "About to transfer", "Hello", MB_OK);
			*commPacket->debugString = "Transfering DMA...";
			DMATransfer(commPacket);
	//		MessageBox(NULL, "DMA transfer done", "Hello", MB_OK);
			while ((!commPacket->loopAbort) && (commPacket->dataValid) && (WSC_SWEEP_SINGLE == commPacket->commandType));			// Wait for view to paint old data
			if (WSC_SWEEP_SINGLE != commPacket->commandType) break;
			if (commPacket->loopAbort)  break;		// Was loop aborted?
			
			*commPacket->debugString = "Copying data...";
			
			commPacket->updatingBuffers = TRUE;		// Should disable view to paint false data
			CopyDmaDataToBuffer(commPacket);		// copy dma data to buffers in struct
			commPacket->updatingBuffers = FALSE;
			commPacket->dataValid = TRUE;
			commPacket->msgReceiver->PostMessage(WM_USER_NEW_DATA_AVAILABLE);
			*commPacket->debugString = "Entering Idle state again...";
//			MessageBox(NULL, "Goto Idle", "Hello", MB_OK);
//			IssueNextState();
			GotoIdleState(commPacket);
			*commPacket->debugString = "Sweep complete";
			break;

		
		
		
		


			






		case WSC_SWEEP_NORMAL:
			*commPacket->debugString = "Going to Idle state";
//			GotoIdleState(commPacket);

			CONTROL_TYPE	c;

			switch (ReadRegister(ADR_CONTROL, &c.field)) {
			case ST_COULD_NOT_OPEN_USB:
				*commPacket->debugString = "SN, could not open USB";
				break;
			case ST_READFILE_ERROR:
				*commPacket->debugString = "SN, Readfile error";
				break;
			case ST_ILLEGAL_RESPONSE:
				*commPacket->debugString = "SN, Illegal response";
				break;
			};

			c.bits.SWEEP_MODE = TC_SM_NORMAL;
			WriteRegister(ADR_CONTROL, c.field);

			*commPacket->debugString = "Starting sweep";
			IssueNextState();
			*commPacket->debugString = "Waiting for sweep complete...";
			while ((!commPacket->loopAbort) && (WSC_SWEEP_NORMAL == commPacket->commandType)) {
				do {


					switch (ReadRegister(ADR_STATUS, &(commPacket->state->status.field))) {
					case ST_COULD_NOT_OPEN_USB:
						commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Could not open USB port\nIs HW connected and power on?\nCheck and then try menu HW->Configure Hardware");
						commPacket->threadBusy = FALSE;
						error = TRUE;
						break;
					case ST_READFILE_ERROR:
						commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Error reading from HW via USB");
						commPacket->threadBusy = FALSE;
						error = TRUE;
						break;
					case ST_ILLEGAL_RESPONSE:
						commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Illegal response");
						commPacket->threadBusy = FALSE;
						error = TRUE;
						break;
					};

		/*
					if (ReadRegister(ADR_STATUS, &(commPacket->state->status.field)) != ST_SUCCESS) {
						MessageBox(NULL, "Error in wait loop", "Error", MB_OK);
						commPacket->loopAbort = TRUE;
					}
		*/
				
				
				} while ((!commPacket->loopAbort) && (WSC_SWEEP_NORMAL == commPacket->commandType) && (S_SS_FULL != commPacket->state->status.bits.SWEEP_STATE) && (!error));
				if (error) break;
				if (commPacket->loopAbort)  break;		// Was loop aborted?
				if (commPacket->commandType != WSC_SWEEP_NORMAL) break;
				// DMA transfer
	//			MessageBox(NULL, "About to transfer", "Hello", MB_OK);
				*commPacket->debugString = "Transfering DMA...";
				DMATransfer(commPacket);
		//		MessageBox(NULL, "DMA transfer done", "Hello", MB_OK);
				while ((!commPacket->loopAbort) && (commPacket->dataValid) && (WSC_SWEEP_NORMAL == commPacket->commandType));			// Wait for view to paint old data
				if (WSC_SWEEP_NORMAL != commPacket->commandType) break;
				if (commPacket->loopAbort)  break;		// Was loop aborted?
				
				*commPacket->debugString = "Copying data...";
				
				commPacket->updatingBuffers = TRUE;		// Should disable view to paint false data
				CopyDmaDataToBuffer(commPacket);		// copy dma data to buffers in struct
				commPacket->updatingBuffers = FALSE;
				commPacket->dataValid = TRUE;
				commPacket->msgReceiver->PostMessage(WM_USER_NEW_DATA_AVAILABLE);
				*commPacket->debugString = "Entering Idle state again...";
	//			MessageBox(NULL, "Goto Idle", "Hello", MB_OK);
				IssueNextState();
	//			GotoIdleState(commPacket);
				*commPacket->debugString = "Sweep complete";
			}
//			GotoIdleState(commPacket);
			break;














		case WSC_SWEEP_AUTO:
			int			softAutoState;

			switch (ReadRegister(ADR_CONTROL, &c.field)) {
			case ST_COULD_NOT_OPEN_USB:
				*commPacket->debugString = "SN, could not open USB";
				break;
			case ST_READFILE_ERROR:
				*commPacket->debugString = "SN, Readfile error";
				break;
			case ST_ILLEGAL_RESPONSE:
				*commPacket->debugString = "SN, Illegal response";
				break;
			};

			c.bits.SWEEP_MODE = TC_SM_NORMAL;
			WriteRegister(ADR_CONTROL, c.field);
			softAutoState = ST_SAS_NORMAL;

//			softAutoState = ST_SAS_AUTO;
//			c.bits.SWEEP_MODE = TC_SM_AUTO;
//			WriteRegister(ADR_CONTROL, c.field);

			IssueNextState();
			while ((!commPacket->loopAbort) && (WSC_SWEEP_AUTO == commPacket->commandType)) {
				if (ST_SAS_NORMAL == softAutoState) {
					// Normal in auto mode!!!
					// Wait for armed
					*commPacket->debugString = "N: Wait for armed";
					do {
						switch (ReadRegister(ADR_STATUS, &(commPacket->state->status.field))) {
						case ST_COULD_NOT_OPEN_USB:
							commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Could not open USB port\nIs HW connected and power on?\nCheck and then try menu HW->Configure Hardware");
							commPacket->threadBusy = FALSE;
							error = TRUE;
							break;
						case ST_READFILE_ERROR:
							commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Error reading from HW via USB");
							commPacket->threadBusy = FALSE;
							error = TRUE;
							break;
						case ST_ILLEGAL_RESPONSE:
							commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Illegal response");
							commPacket->threadBusy = FALSE;
							error = TRUE;
							break;
						};

					} while ((!commPacket->loopAbort) && (WSC_SWEEP_AUTO == commPacket->commandType) && 
							(S_SS_PRE_FILL == commPacket->state->status.bits.SWEEP_STATE) && 
							(!error));
					if (error) break;
					if (commPacket->loopAbort)  break;		// Was loop aborted?
					if (commPacket->commandType != WSC_SWEEP_AUTO) break;
					*commPacket->debugString = "N: Wait for trigger";
					commPacket->waitTriggerAbort = FALSE;
					commPacket->msgReceiver->PostMessage(WM_USER_SWEEP_STATE_ARMED);
					// Wait for full
					do {
						switch (ReadRegister(ADR_STATUS, &(commPacket->state->status.field))) {
						case ST_COULD_NOT_OPEN_USB:
							commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Could not open USB port\nIs HW connected and power on?\nCheck and then try menu HW->Configure Hardware");
							commPacket->threadBusy = FALSE;
							error = TRUE;
							break;
						case ST_READFILE_ERROR:
							commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Error reading from HW via USB");
							commPacket->threadBusy = FALSE;
							error = TRUE;
							break;
						case ST_ILLEGAL_RESPONSE:
							commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Illegal response");
							commPacket->threadBusy = FALSE;
							error = TRUE;
							break;
						};

					} while ((!commPacket->loopAbort) && (WSC_SWEEP_AUTO == commPacket->commandType) && (S_SS_FULL != commPacket->state->status.bits.SWEEP_STATE) && (!error) && (!commPacket->waitTriggerAbort));
					if (error) break;
					if (commPacket->loopAbort)  break;		// Was loop aborted?
					if (commPacket->commandType != WSC_SWEEP_AUTO) break;
					if (commPacket->waitTriggerAbort) {
						*commPacket->debugString = "Wait for trigger, timed out";
						// Wait for trigger timed out, now enter auto in auto mode
						c.bits.SWEEP_MODE = TC_SM_AUTO;
						WriteRegister(ADR_CONTROL, c.field);
						softAutoState = ST_SAS_AUTO;
					} else {
						*commPacket->debugString = "Found trigger";
						// Trigger found, continue in normal in auto mode
						commPacket->msgReceiver->PostMessage(WM_USER_SWEEP_STATE_FULL);
						// DMA transfer
						DMATransfer(commPacket);
						while ((!commPacket->loopAbort) && (commPacket->dataValid) && (WSC_SWEEP_AUTO == commPacket->commandType));			// Wait for view to paint old data
						if (WSC_SWEEP_AUTO != commPacket->commandType) break;
						if (commPacket->loopAbort)  break;		// Was loop aborted?
						commPacket->updatingBuffers = TRUE;		// Should disable view to paint false data
						CopyDmaDataToBuffer(commPacket);		// copy dma data to buffers in struct
						commPacket->updatingBuffers = FALSE;
						commPacket->dataValid = TRUE;
						commPacket->msgReceiver->PostMessage(WM_USER_NEW_DATA_AVAILABLE);
						*commPacket->debugString = "Sweep complete";
					}
				} else if (ST_SAS_AUTO == softAutoState) {
					// Auto in auto mode!!!
					// Wait for full
					do {
						switch (ReadRegister(ADR_STATUS, &(commPacket->state->status.field))) {
						case ST_COULD_NOT_OPEN_USB:
							commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Could not open USB port\nIs HW connected and power on?\nCheck and then try menu HW->Configure Hardware");
							commPacket->threadBusy = FALSE;
							error = TRUE;
							break;
						case ST_READFILE_ERROR:
							commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Error reading from HW via USB");
							commPacket->threadBusy = FALSE;
							error = TRUE;
							break;
						case ST_ILLEGAL_RESPONSE:
							commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Illegal response");
							commPacket->threadBusy = FALSE;
							error = TRUE;
							break;
						};

					} while ((!commPacket->loopAbort) && (WSC_SWEEP_AUTO == commPacket->commandType) && (S_SS_FULL != commPacket->state->status.bits.SWEEP_STATE) && (!error));
					if (error) break;
					if (commPacket->loopAbort)  break;		// Was loop aborted?
					if (commPacket->commandType != WSC_SWEEP_AUTO) break;
					// DMA transfer
					DMATransfer(commPacket);
					while ((!commPacket->loopAbort) && (commPacket->dataValid) && (WSC_SWEEP_AUTO == commPacket->commandType));			// Wait for view to paint old data
					if (WSC_SWEEP_AUTO != commPacket->commandType) break;
					if (commPacket->loopAbort)  break;		// Was loop aborted?
					commPacket->updatingBuffers = TRUE;		// Should disable view to paint false data
					CopyDmaDataToBuffer(commPacket);		// copy dma data to buffers in struct
					commPacket->updatingBuffers = FALSE;
					commPacket->dataValid = TRUE;
					commPacket->msgReceiver->PostMessage(WM_USER_NEW_DATA_AVAILABLE);
					// Was trigger issued during last sweep?
					switch (ReadRegister(ADR_STATUS, &(commPacket->state->status.field))) {
					case ST_COULD_NOT_OPEN_USB:
						commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Could not open USB port\nIs HW connected and power on?\nCheck and then try menu HW->Configure Hardware");
						commPacket->threadBusy = FALSE;
						error = TRUE;
						break;
					case ST_READFILE_ERROR:
						commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Error reading from HW via USB");
						commPacket->threadBusy = FALSE;
						error = TRUE;
						break;
					case ST_ILLEGAL_RESPONSE:
						commPacket->msgReceiver->PostMessage(WM_USER_ERROR, NULL, (LPARAM) "Illegal response");
						commPacket->threadBusy = FALSE;
						error = TRUE;
						break;
					};
					if (commPacket->state->status.bits.DID_TRIGG) {
						// Yes, trigger was issued, now enter normal in auto mode.
						c.bits.SWEEP_MODE = TC_SM_NORMAL;
						WriteRegister(ADR_CONTROL, c.field);
						softAutoState = ST_SAS_NORMAL;
					}
					*commPacket->debugString = "Sweep complete";
				}
				IssueNextState();
			}
			break;

		
		
		
		
		
		
		
		
		case WSC_TERMINATE_THREAD :
			commPacket->msgReceiver->PostMessage(WM_USER_THREAD_TERMINATED);	// Is this necessary???
			commPacket->threadBusy = FALSE;										// -"-
			commPacket->loopAbort = FALSE;										// -"-
//			MessageBox(NULL, "Terminating immediately after command", "Even before", MB_OK);
			AfxEndThread(0, TRUE);
			return 0;
		};

		if (WSC_TERMINATE_THREAD == commPacket->commandType) {	// Test if commandType chenged during any command
//			MessageBox(NULL, "Terminating immediately after command", "Hello", MB_OK);
			GotoIdleState(commPacket);
			commPacket->threadBusy = FALSE;
			commPacket->loopAbort = FALSE;
			AfxEndThread(0, TRUE);
			return 0;
		}

		commPacket->commandType = WSC_NO_COMMAND;
		commPacket->threadBusy = FALSE;

		if (commPacket->loopAbort == TRUE) {
			commPacket->msgReceiver->PostMessage(WM_USER_THREAD_ABORTED);
		} else {
			commPacket->msgReceiver->PostMessage(WM_USER_COMMAND_COMPLETE);
		}
		commPacket->loopAbort = FALSE;

	}

}












unsigned char	WriteRegister(unsigned char address, unsigned char data)
{
	CUsbDriver		usb;
	HANDLE			txUsb;
	HANDLE			rxUsb;
	char			outPipe[32] = "PIPE03";
	char			inPipe[32] = "PIPE02";
	unsigned long	len;
	unsigned char	buf[64];
	unsigned char	ret = ST_SUCCESS;;

	txUsb = usb.open_file(outPipe);

	if (txUsb == INVALID_HANDLE_VALUE) return ST_COULD_NOT_OPEN_USB;

	buf[0] = 0x20;
	buf[1] = address;
	buf[2] = data;

	WriteFile(txUsb, (LPCSTR) buf, 3, &len, NULL);

	rxUsb = usb.open_file(inPipe);

	if (rxUsb == INVALID_HANDLE_VALUE) {
		CloseHandle(txUsb);
		return ST_COULD_NOT_OPEN_USB;
	}
	if (ReadFile(rxUsb, buf, 64, &len, NULL)) {
		if (buf[0] != 0x20) ret = ST_ILLEGAL_RESPONSE;
	} else {
		ret = ST_READFILE_ERROR;
	}

	CloseHandle(txUsb);
	CloseHandle(rxUsb);
	return ret;
}

unsigned char	ReadRegister(unsigned char address, unsigned char* data)
{
	CUsbDriver		usb;
	HANDLE			txUsb;
	HANDLE			rxUsb;
	char			outPipe[32] = "PIPE03";
	char			inPipe[32] = "PIPE02";
	unsigned long	len;
	unsigned char	buf[64];
	unsigned char	ret = ST_SUCCESS;
	BOOL			done = TRUE;

	txUsb = usb.open_file(outPipe);

	if (txUsb == INVALID_HANDLE_VALUE) return ST_COULD_NOT_OPEN_USB;

	buf[0] = 0x22;
	buf[1] = address;

	WriteFile(txUsb, (LPCSTR) buf, 2, &len, NULL);
	rxUsb = usb.open_file(inPipe);

	if (rxUsb == INVALID_HANDLE_VALUE) {
		CloseHandle(txUsb);
		return ST_COULD_NOT_OPEN_USB;
	}
	do {
		if (ReadFile(rxUsb, buf, 64, &len, NULL)) {
			if (buf[0] != 0x22) {
				done = FALSE;
				ret = ST_ILLEGAL_RESPONSE;
				commPacket->debugString->Format("RR Illegal response: 0x%02X (expected 0x22)", buf[0]);

				buf[0] = 0x22;
				buf[1] = address;

				WriteFile(txUsb, (LPCSTR) buf, 2, &len, NULL);
			} else {
				done = TRUE;
			}
		} else {
			ret = ST_READFILE_ERROR;
		}
	} while (!done);

	CloseHandle(txUsb);
	CloseHandle(rxUsb);
	*data = buf[1];
	return ret;
}

unsigned char GetCPUVersion(unsigned char* dest) 
{
	CUsbDriver		usb;
	HANDLE			txUsb;
	HANDLE			rxUsb;
	char			outPipe[32] = "PIPE03";
	char			inPipe[32] = "PIPE02";
	unsigned long	len;
	char			buf[64];
	unsigned char	ret = ST_SUCCESS;;

	txUsb = usb.open_file(outPipe);

	if (txUsb == INVALID_HANDLE_VALUE) return ST_COULD_NOT_OPEN_USB;

	buf[0] = 0x10;

	WriteFile(txUsb, (LPCSTR) buf, 1, &len, NULL);
	rxUsb = usb.open_file(inPipe);

	if (rxUsb == INVALID_HANDLE_VALUE) {
		CloseHandle(txUsb);
		return ST_COULD_NOT_OPEN_USB;
	}
	if (ReadFile(rxUsb, buf, 64, &len, NULL)) {
		if (buf[0] != 0x10) ret = ST_ILLEGAL_RESPONSE;
		else {
			buf[30] = 0;	// Terminate string
			strcpy((char*) dest, (const char*) buf+1);
		}

	} else {
		ret = ST_READFILE_ERROR;
	}

	CloseHandle(txUsb);
	CloseHandle(rxUsb);
	return ret;
}


void			GotoIdleState(WS_COMM_PACKET*	commPacket)
{
	CONTROL_TYPE	c;
	int				timeoutCounter;
	int				i;

	*commPacket->debugString = "GIS Reading state";

	switch (ReadRegister(ADR_STATUS, &(commPacket->state->status.field))) {
	case ST_COULD_NOT_OPEN_USB:
		*commPacket->debugString = "GIS, RS could not open USB";
		break;
	case ST_READFILE_ERROR:
		*commPacket->debugString = "GIS, RS Readfile error";
		break;
	case ST_ILLEGAL_RESPONSE:
//		*commPacket->debugString = "GIS, RS Illegal response";
		break;
	};
	
	
	if (S_SS_IDLE == commPacket->state->status.bits.SWEEP_STATE) return;

//	*commPacket->debugString = "GIS Get current Sweep mode";

//	ReadRegister(ADR_CONTROL, &c.field);	// Set SWEEP_MODE to SM_SINGLE

	switch (ReadRegister(ADR_CONTROL, &c.field)) {
	case ST_COULD_NOT_OPEN_USB:
		*commPacket->debugString = "GIS, GC could not open USB";
		break;
	case ST_READFILE_ERROR:
		*commPacket->debugString = "GIS, GC Readfile error";
		break;
	case ST_ILLEGAL_RESPONSE:
		*commPacket->debugString = "GIS, GC Illegal response";
		break;
	};

	
	*commPacket->debugString = "GIS Set sweep mode to single";
	c.bits.SWEEP_MODE = TC_SM_SINGLE;
	WriteRegister(ADR_CONTROL, c.field);
	*commPacket->debugString = "GIS Issue next state";
	c.bits.NEXT_STATE = 1;
	WriteRegister(ADR_CONTROL, c.field);
//	Sleep(10);
	timeoutCounter = 0;
	*commPacket->debugString = "GIS Wait for Idle state";
	i = 0;
	do {
		ReadRegister(ADR_STATUS, &(commPacket->state->status.field));
		timeoutCounter++;
		if (timeoutCounter > 10) {			// Have we tried ten times?
			commPacket->debugString->Format("GIS Reissue next state, iteration: %d", i++);
			c.bits.NEXT_STATE = 1;			// Yes, obviously we must reassert NEXT_STATE
			WriteRegister(ADR_CONTROL, c.field);
			timeoutCounter = 0;
		}
	} while (S_SS_IDLE != commPacket->state->status.bits.SWEEP_STATE);
}

void			IssueNextState( void )
{
	CONTROL_TYPE	c;

	ReadRegister(ADR_CONTROL, &c.field);
	c.bits.NEXT_STATE = 1;
	WriteRegister(ADR_CONTROL, c.field);
}

unsigned long	DMATransfer(WS_COMM_PACKET*	commPacket)
{
	// Perform dma transfer to commPacket->dmaBuffer
	STATUS_TYPE			reg;
	CUsbDriver			usb;

	HANDLE			txUsb;
	HANDLE			rxUsb;
	char			outPipe[32] = "PIPE03";
	char			inPipe[32] = "PIPE02";
	unsigned long	len;
	unsigned char	buf[64];
	unsigned char	numberOfBytes;
	unsigned long	numberOfPackets;
	unsigned long	i;
	unsigned long	numberOfTx, lastTxSize;
	CString			msg;

	ReadRegister(ADR_STATUS, &(reg.field));

	if (reg.bits.SWEEP_STATE & 0x07 != S_SS_FULL) return 0;

/*		For simplicity, all probes and pods are activated in HW, fix later!!!!!!!!!!!!!!!!!!
	numberOfBytes = 0;
	if (commPacket->settings->m_prbPodCntrl.bits.PODP_ACTIVE) numberOfBytes++;
	if (commPacket->settings->m_prbPodCntrl.bits.PODQ_ACTIVE) numberOfBytes++;
	if (commPacket->settings->m_prbPodCntrl.bits.PRBA_ACTIVE) numberOfBytes++;
	if (commPacket->settings->m_prbPodCntrl.bits.PRBB_ACTIVE) numberOfBytes++;
*/
	numberOfBytes = 4;



	if ((numberOfBytes == 0) || (commPacket->settings->m_bufferSize == 0)) {
//		m_status = "ERROR: There is nothing to read";
//		UpdateData(FALSE);
		return 0;
	}
	if ((commPacket->settings->m_bufferSize * numberOfBytes ) % 64 != 0) numberOfPackets = ((commPacket->settings->m_bufferSize * numberOfBytes ) / 64) + 1;
	else numberOfPackets = ((commPacket->settings->m_bufferSize * numberOfBytes ) / 64);

	if (!commPacket->dmaBuffer) {
		return 0;
	}

	txUsb = usb.open_file(outPipe);

	if (txUsb != INVALID_HANDLE_VALUE) 
	{
		buf[0] = 0x80;

		WriteFile(txUsb, (LPCSTR) buf, 1, &len, NULL);
		rxUsb = usb.open_file(inPipe);

		if (rxUsb != INVALID_HANDLE_VALUE) 
		{
			if (ReadFile(rxUsb, buf, 64, &len, NULL))
			{
				if (buf[0] != 0x80) {
					CString msg;
					msg.Format("Illegal response: 0x%02X (expected 0x80)", buf[0]);
					MessageBox(NULL, msg, "DMAError", MB_OK);
//					m_status.Format("Illegal response (0x80): 0x%02X", buf[0]);
//					UpdateData(FALSE);
				} else {
//					m_status = "DMA Transfer...";
//					UpdateData(FALSE);
//					tStart = clock();

					numberOfTx = (numberOfPackets * 64) / 65536;
					lastTxSize = (numberOfPackets * 64) % 65536;

//					msg.Format("numberOfTx: %d, lastTx: %d", numberOfTx, lastTxSize);
//					MessageBox(NULL, msg, "Hello", MB_OK);
					for (i=0 ; i<numberOfTx ; i++) {
						ReadFile(rxUsb, commPacket->dmaBuffer+i*65536, 65536, &len, NULL);
					}
					ReadFile(rxUsb, commPacket->dmaBuffer+i*65536, lastTxSize, &len, NULL);
					Sleep(50);

				}
			} else {
//				m_status.Format("ReadFile error: %d", GetLastError());
//				UpdateData(FALSE);
			}
		}
	}

	if (txUsb != INVALID_HANDLE_VALUE) CloseHandle(txUsb);
	if (rxUsb != INVALID_HANDLE_VALUE) CloseHandle(rxUsb);
	return i;
}
	
void	CopyDmaDataToBuffer(WS_COMM_PACKET*	commPacket)
{
	unsigned char	numberOfBytes;
	unsigned char	activeBytes;
	unsigned long	i;

/*		For simplicity, all probes and pods are activated in HW, fix later!!!!!!!!!!!!!!!!!!
	numberOfBytes = 0;
	if (commPacket->settings->m_prbPodCntrl.bits.PRBA_ACTIVE) numberOfBytes++;
	if (commPacket->settings->m_prbPodCntrl.bits.PRBB_ACTIVE) numberOfBytes++;
	if (commPacket->settings->m_prbPodCntrl.bits.PODP_ACTIVE) numberOfBytes++;
	if (commPacket->settings->m_prbPodCntrl.bits.PODQ_ACTIVE) numberOfBytes++;
*/	
	numberOfBytes = 4;

	for (i=0 ; i < commPacket->settings->m_bufferSize ; i++) {
		activeBytes = 0;
//		if (commPacket->settings->m_prbPodCntrl.bits.PRBA_ACTIVE) {
			commPacket->prbAData[i] = commPacket->dmaBuffer[numberOfBytes*i];
			activeBytes++;
//		}
//		if (commPacket->settings->m_prbPodCntrl.bits.PRBB_ACTIVE) {
			commPacket->prbBData[i] = commPacket->dmaBuffer[numberOfBytes*i+activeBytes];
			activeBytes++;
//		}
//		if (commPacket->settings->m_prbPodCntrl.bits.PODP_ACTIVE) {
			commPacket->podPData[i] = commPacket->dmaBuffer[numberOfBytes*i+activeBytes];
			activeBytes++;
//		}
//		if (commPacket->settings->m_prbPodCntrl.bits.PODQ_ACTIVE) {
			commPacket->podQData[i] = commPacket->dmaBuffer[numberOfBytes*i+activeBytes];
//		}
	}
}
	
/*	
	
	
//					tFinish = clock();

//					m_status.Format("DMA Done in %2.1f sec (%d bytes)", (double)(tFinish - tStart) / CLOCKS_PER_SEC, len);
//					UpdateData(FALSE);
						
					maxl = __max(__max(prbALen, prbBLen), __max(podPLen, podQLen));
					len = __min(maxl, commPacket->settings->m_bufferSize);

					for (i=0 ; i < len ; i++) {
						activeByte = 0;
						if (c&1) {
							if ((prbABuf != NULL) && (prbALen >= i)) {
								prbABuf[i] = db[numberOfBytes*i];
							}
							activeByte++;
						} else {
							if ((prbABuf != NULL) && (prbALen >= i)) {
								prbABuf[i] = 0;
							}
						}
										
						if (c&2) {
							if ((prbBBuf != NULL) && (prbBLen >= i)) {
								prbBBuf[i] = db[numberOfBytes*i+activeByte];
							}
							activeByte++;
						} else {
							if ((prbBBuf != NULL) && (prbBLen >= i)) {
								prbBBuf[i] = 0;
							}
						}
					
						if (c&4) {
							if ((podPBuf != NULL) && (podPLen >= i)) {
								podPBuf[i] = db[numberOfBytes*i+activeByte];
							}
							activeByte++;
						} else {
							if ((podPBuf != NULL) && (podPLen >= i)) {
								podPBuf[i] = 0;
							}
						}
					
						if (c&8) {
							if ((podQBuf != NULL) && (podQLen >= i)) {
								podQBuf[i] = db[numberOfBytes*i+activeByte];
							}
						} else {
							if ((podQBuf != NULL) && (podQLen >= i)) {
								podQBuf[i] = 0;
							}
						}
					}
					if (maxl > commPacket->settings->m_bufferSize) {
						for (i=len ; i<maxl ; i++) {
							if ((prbABuf != NULL) && (prbALen >= i)) {
								prbABuf[i] = 0;
							}
							if ((prbBBuf != NULL) && (prbBLen >= i)) {
								prbBBuf[i] = 0;
							}
							if ((podPBuf != NULL) && (podPLen >= i)) {
								podPBuf[i] = 0;
							}
							if ((podQBuf != NULL) && (podQLen >= i)) {
								podQBuf[i] = 0;
							}
						}
					}
				}
			} else {
				m_status.Format("ReadFile error: %d", GetLastError());
				UpdateData(FALSE);
			}
		}
	}

	if (txUsb != INVALID_HANDLE_VALUE) CloseHandle(txUsb);
	if (rxUsb != INVALID_HANDLE_VALUE) CloseHandle(rxUsb);
	free(db);
	return i;

}

unsigned long CUSBTestAppDlg::GetSampledData(unsigned char* prbABuf, unsigned long prbALen, 
											 unsigned char* prbBBuf, unsigned long prbBLen, 
											 unsigned char* podPBuf, unsigned long podPLen, 
											 unsigned char* podQBuf, unsigned long podQLen)
{
	STATUS_TYPE		reg;

	HANDLE			txUsb;
	HANDLE			rxUsb;
	char			outPipe[32] = "PIPE03";
	char			inPipe[32] = "PIPE02";
	unsigned long	len;
	unsigned char	buf[64];
	unsigned char	numberOfBytes;
	unsigned long	numberOfPackets;
	unsigned char	c, activeByte;
	unsigned long	i, maxl;
	unsigned long	numberOfTx, lastTxSize;
	unsigned char*	db;
	clock_t			tStart, tFinish;



	reg.field = ReadRegister(ADR_STATUS);

	if (reg.bits.SWEEP_STATE & 0x07 != S_SS_FULL) return 0;

	c = (ReadRegister(0x61) >> 4) & 0x0F;
	numberOfBytes = 0;
	if (c&1) numberOfBytes++;
	if (c&2) numberOfBytes++;
	if (c&4) numberOfBytes++;
	if (c&8) numberOfBytes++;
	if ((numberOfBytes == 0) || (bufferSize == 0)) {
		return 0;
	}
	if ((bufferSize * numberOfBytes ) % 64 != 0) numberOfPackets = ((bufferSize * numberOfBytes ) / 64) + 1;
	else numberOfPackets = ((bufferSize * numberOfBytes ) / 64);

	db = (unsigned char*) malloc(bufferSize * numberOfBytes);

	if (!db) {
		m_status = "ERROR: Could not allocate enough memory";
		UpdateData(FALSE);
		return 0;
	}

	m_scopeResponds = FALSE;						// Assume Winscope com not OK

	txUsb = m_usbDriver.open_file(outPipe);

	if (txUsb != INVALID_HANDLE_VALUE) 
	{
		buf[0] = 0x80;

		WriteFile(txUsb, (LPCSTR) buf, 1, &len, NULL);
		rxUsb = m_usbDriver.open_file(inPipe);

		if (rxUsb != INVALID_HANDLE_VALUE) 
		{
			if (ReadFile(rxUsb, buf, 64, &len, NULL))
			{
				m_scopeResponds = TRUE;						// Indicate to other parts of SW that fpga is configured and works.
				if (buf[0] != 0x80) {
					m_status.Format("Illegal response (0x80): 0x%02X", buf[0]);
					UpdateData(FALSE);
				} else {
					m_status = "DMA Transfer...";
					UpdateData(FALSE);
					tStart = clock();

					numberOfTx = (numberOfPackets * 64) / 65536;
					lastTxSize = (numberOfPackets * 64) % 65536;

					for (i=0 ; i<numberOfTx ; i++) {
						ReadFile(rxUsb, db+i*65536, 65536, &len, NULL);
					}
					ReadFile(rxUsb, db+i*65536, lastTxSize, &len, NULL);
					tFinish = clock();
					m_scopeResponds = TRUE;						// Indicate to other parts of SW that fpga is configured and works.
					m_status.Format("DMA Done in %2.1f sec (%d bytes)", (double)(tFinish - tStart) / CLOCKS_PER_SEC, len);
					UpdateData(FALSE);
						
					maxl = __max(__max(prbALen, prbBLen), __max(podPLen, podQLen));
					len = __min(maxl, bufferSize);

					for (i=0 ; i < len ; i++) {
						activeByte = 0;
						if (c&1) {
							if ((prbABuf != NULL) && (prbALen >= i)) {
								prbABuf[i] = db[numberOfBytes*i];
							}
							activeByte++;
						} else {
							if ((prbABuf != NULL) && (prbALen >= i)) {
								prbABuf[i] = 0;
							}
						}
										
						if (c&2) {
							if ((prbBBuf != NULL) && (prbBLen >= i)) {
								prbBBuf[i] = db[numberOfBytes*i+activeByte];
							}
							activeByte++;
						} else {
							if ((prbBBuf != NULL) && (prbBLen >= i)) {
								prbBBuf[i] = 0;
							}
						}
					
						if (c&4) {
							if ((podPBuf != NULL) && (podPLen >= i)) {
								podPBuf[i] = db[numberOfBytes*i+activeByte];
							}
							activeByte++;
						} else {
							if ((podPBuf != NULL) && (podPLen >= i)) {
								podPBuf[i] = 0;
							}
						}
					
						if (c&8) {
							if ((podQBuf != NULL) && (podQLen >= i)) {
								podQBuf[i] = db[numberOfBytes*i+activeByte];
							}
						} else {
							if ((podQBuf != NULL) && (podQLen >= i)) {
								podQBuf[i] = 0;
							}
						}
					}
					if (maxl > bufferSize) {
						for (i=len ; i<maxl ; i++) {
							if ((prbABuf != NULL) && (prbALen >= i)) {
								prbABuf[i] = 0;
							}
							if ((prbBBuf != NULL) && (prbBLen >= i)) {
								prbBBuf[i] = 0;
							}
							if ((podPBuf != NULL) && (podPLen >= i)) {
								podPBuf[i] = 0;
							}
							if ((podQBuf != NULL) && (podQLen >= i)) {
								podQBuf[i] = 0;
							}
						}
					}
				}
			} else {
				m_status.Format("ReadFile error: %d", GetLastError());
				UpdateData(FALSE);
			}
		}
	}

	if (txUsb != INVALID_HANDLE_VALUE) CloseHandle(txUsb);
	if (rxUsb != INVALID_HANDLE_VALUE) CloseHandle(rxUsb);
	free(db);
	return i;
}

  */