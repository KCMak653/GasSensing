#pragma once
#include<Windows.h>
#include <iostream>
#include<string.h>
namespace GM
{
	class gmCmd
	{
	public:
		gmCmd();
		int sendPersCmd(char cmd[], int len);
		int setCarrBase(); //Set carrier flow to minimum flow 200ccm
		int setFlowRate(int flowRate);
		int setAnalyteGas(const int analyteNum); //ccm
		int setAnalyteConc(int conc); //ppm
		int measFlowRate(double &measVal, bool outputToConsole);
		int measAnalyteConc(double &measVal, bool outputToConsole);
		~gmCmd();
	private:
		bool Status;
		HANDLE hComm_;
		char SerialBuffer_[256];
		DWORD bufferSize_ = 0;
		std::string comPortName_{};
		std::string flowRateCMD_{};
		std::string analyteConcCMD_{};
		std::string analyteGasCMD_{};
		std::string measFlowRateCMD_{};
		std::string measAnalyteConcCMD_{};
		int _updateCMD();

		template<typename T>
		int _sendCmd(const std::string baseCmd, T val, bool valNeeded);
		
		int _readResponse(bool outputToConsole);
		int _readValue(double &measVal, bool outputToConsole);

		int _purge();
	};
}


