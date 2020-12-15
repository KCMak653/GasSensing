#include <string>
#include <Windows.h>
#include <stdio.h>
#include<stdlib.h>
#include <malloc.h>
#include <iostream>
#include<time.h>
#include "C:/Program Files/National Instruments/Shared/ExternalCompilerSupport/C/include/ni4882.h"
#include "gmCmd.h"

//COM communication parameters

#define BAUDRATE_GM CBR_9600 //Baud rate
#define BYTESIZE_GM 8 //Byte size
#define STOPBITS_GM ONESTOPBIT //Number of stop bits
#define PARITY_GM NOPARITY //Parity state

//timeouts

#define RDINTTIME 50
#define RDTOTTIME 50
#define RDTOTMULT 10
#define WTTOTTIME 50
#define WTTOTMULT 10

//const int comPortArrSize = 5;

//const int analyteGasInd = 9;
//const int analyteGasArrSize = 11;

namespace GM
{
	gmCmd::gmCmd() {
		std::cout << "Opening Serial Port" << std::endl;
		comPortName_ = "COM1";
		flowRateCMD_ = "CONC TOT TARGET ";
		analyteConcCMD_ = "CONC GAS TARGET ";
		analyteGasCMD_ = "CONC GAS ";
		measFlowRateCMD_ = "CONC TOT ACTUAL ?\n";
		measAnalyteConcCMD_ = "CONC GAS ACTUAL ?\n";

		hComm_ = CreateFile(L"COM1",
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
		if (hComm_ == INVALID_HANDLE_VALUE) {
			std::cout << "Error in opening serial port" << std::endl;
			DWORD lastError = GetLastError();
			std::cout << lastError << std::endl;
		}
		

		DCB dcbSerialParams = { 0 };
		dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
		Status = GetCommState(hComm_, &dcbSerialParams);

		if (!Status) {
			std::cout << "Error in GetCommState()" << std::endl;
		}

		dcbSerialParams.BaudRate = BAUDRATE_GM;
		dcbSerialParams.ByteSize = BYTESIZE_GM;
		dcbSerialParams.StopBits = STOPBITS_GM;
		dcbSerialParams.Parity = PARITY_GM;

		Status = SetCommState(hComm_, &dcbSerialParams);

		if (!Status) {
			std::cout << "Error in setting DCB Structure" << std::endl;
		}

		//Set the timeouts in ms

		COMMTIMEOUTS timeouts;
		timeouts.ReadIntervalTimeout = RDINTTIME;
		timeouts.ReadTotalTimeoutConstant = RDTOTTIME;
		timeouts.ReadTotalTimeoutMultiplier = RDTOTMULT;
		timeouts.WriteTotalTimeoutConstant = WTTOTTIME;
		timeouts.WriteTotalTimeoutMultiplier = WTTOTMULT;

		Status = SetCommTimeouts(hComm_, &timeouts);

		if (!Status) {
			std::cout << "Error in setting timeouts" << std::endl;
		}
		std::cout << "Gas Mixer Initialized" << std::endl;

		//std::cout<<"Ok here"<<std::endl;
		//CloseHandle(hComm_);
		
	}
	template<typename T>
	int gmCmd::_sendCmd(const std::string baseCmd, T val, bool valNeeded)
	{
		_purge();
		DWORD dNoOfBytesWritten;
		std::string strCMD = baseCmd;
		if (valNeeded) {
			strCMD = strCMD + std::to_string(val) + "\n";
		}


		char* charCMD;
		charCMD = &strCMD[0];
		DWORD cmdSize = strlen(strCMD.c_str());

		Status = WriteFile(hComm_,
			charCMD,
			cmdSize,
			&dNoOfBytesWritten,
			NULL);

		if (!Status) {
			std::cout << "Error sending command" << std::endl;
		}

		return 0;
	}
	
	int gmCmd::_readResponse(bool outputToConsole)
	{
		//Read response
		DWORD dwEventMask;
		char TempChar;
		DWORD nBytesRead;
		int i = 0;
		//Set receive mask
		Status = SetCommMask(hComm_, EV_RXCHAR); //Configure Windows to monitor for character reception
		if (Status == FALSE) {
			std::cout << "Error! in Setting CommMask" << std::endl;
			// return(6);
		}


		// Set WaitComm() Event



		Status = WaitCommEvent(hComm_, &dwEventMask, NULL); //Wait for character to be received


		if (Status == FALSE)
		{
			std::cout << "Error! in Setting WaitCommEvent()" << std::endl;
			//return(7);
		}
		else { // If true, read the RXed data using ReadFile()

			do
			{
				Status = ReadFile(hComm_, &TempChar, sizeof(TempChar), &nBytesRead, NULL);
				SerialBuffer_[i] = TempChar;

				i++;
			} while (nBytesRead > 0);
			bufferSize_ = i-3;
			if (outputToConsole)
			{
				//Print to console
				std::cout << "\n\n  Measured Value:   ";
				int j = 0;
				for (j = 0; j < i - 1; j++)
					std::cout << SerialBuffer_[j];
			}
		}

		return 0;
	}
	int gmCmd::_updateCMD()
	{
		DWORD dNoOfBytesWritten;
		char updateCmd[] = "CONC UPDATE";
		DWORD cmdSize = sizeof(updateCmd);
		Status = WriteFile(hComm_,
			updateCmd,
			cmdSize,
			&dNoOfBytesWritten,
			NULL);

		if (!Status) {
			std::cout << "Error sending command" << std::endl;
		}

		return 0;
	}

	int gmCmd::setAnalyteGas(const int analyteNum)
	{
		Status = _sendCmd(analyteGasCMD_, analyteNum, TRUE);
		
		return 0;

	}
	
	int gmCmd::setFlowRate(int flowRate)
	{
		Status = _sendCmd(flowRateCMD_, flowRate, TRUE);
		//_readResponse(TRUE);
		Status = _updateCMD();
		return 0;
	}
	int gmCmd::setAnalyteConc(int conc)
	{
		Status = _sendCmd(analyteConcCMD_, conc, TRUE);
		Status = _updateCMD();
		return 0;
	}
	
	int gmCmd::measFlowRate(double& measVal, bool outputToConsole)
	{
		Status = _sendCmd(measFlowRateCMD_, -1, FALSE);
		_purge();
		_readResponse(outputToConsole);
		//_readResponse(outputToConsole);
		std::string strVal;
		for (unsigned int i = 0; i < bufferSize_; i++)
		{
			strVal.push_back(SerialBuffer_[i]);
		}
		measVal = std::stod(strVal);

		return 0;
	}

	int gmCmd::measAnalyteConc(double& measVal, bool outputToConsole)
	{
		Status = _sendCmd(measAnalyteConcCMD_, -1, FALSE);
		_purge();
		_readResponse(outputToConsole);
		//_readResponse(outputToConsole);
		std::string strVal;
		for (unsigned int i = 0; i < bufferSize_; i++)
		{
			std::cout << SerialBuffer_[i];
			strVal.push_back(SerialBuffer_[i]);
		}
		measVal = std::stod(strVal);

		return 0;
	}

	int gmCmd::sendPersCmd(char cmd[], int len)
	{
		_purge();
		DWORD dNoOfBytesWritten;
		Status = WriteFile(hComm_,
			cmd,
			len,
			&dNoOfBytesWritten,
			NULL);

		if (!Status) {
			std::cout << "Error sending command" << std::endl;
		}

		return 0;
	}

	int gmCmd::setCarrBase()
	{
		//Set the flow rate to 200CCM - the lowest rate.
		//Need to do this incrementally because it won't begin to flow if
		// it is immediately set. Once it is flowing at 200CCM, it is easy to increase
		// if necessary.
		bool notSet = TRUE;
		int nTimes = 0;
		//Try 3 times, if it didn't work
		int concs[] = { 2000, 1000, 500, 300, 200 };
		double measFlow;
		while (notSet)
		{
			for (int i = 0; i < 5; i++) {
				setFlowRate(concs[i]);
				Sleep(7000);
			}

			measFlowRate(measFlow, TRUE);
			notSet = (measFlow < 150.);
			nTimes++;
			if (nTimes > 3) {
				std::cout << "Unable to set minimum flow rate" << std::endl;
				break;
			}

		}
		return 0;
	}



	int gmCmd::_purge() {
		DWORD dwFlags = PURGE_RXABORT;
		bool purgeStatus;
		purgeStatus = PurgeComm(hComm_, dwFlags);
		if (!purgeStatus) {
			std::cout << "Error with purge" << std::endl;
			return 1;
		}
		dwFlags = PURGE_RXCLEAR;
		purgeStatus = PurgeComm(hComm_, dwFlags);
		if (!purgeStatus) {
			std::cout << "Error with purge" << std::endl;
			return 1;
		}
		dwFlags = PURGE_TXABORT;
		purgeStatus = PurgeComm(hComm_, dwFlags);
		if (!purgeStatus) {
			std::cout << "Error with purge" << std::endl;
			return 1;
		}
		dwFlags = PURGE_TXCLEAR;
		purgeStatus = PurgeComm(hComm_, dwFlags);

		if (!purgeStatus) {
			std::cout << "Error with purge" << std::endl;
			return 1;
		}
		return 0;
	}

	gmCmd::~gmCmd() {
		//At some point include command to close all channels
		// chAllOff();
		setFlowRate(0);
		Sleep(3000);
		CloseHandle(hComm_);
		std::cout << "Gas Mixer Shut Down" << std::endl;
		
	}

}