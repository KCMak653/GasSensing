#include <string>
#include<Windows.h>
#include<stdio.h>
#include <iostream>
#include <conio.h>
#include "hpCmd.h"

#include "C:/Program Files/National Instruments/Shared/ExternalCompilerSupport/C/include/ni4882.h"

//HP Picoammeter constants

//Communication Parameters
#define ARRAYSIZE_RD 19 //Size of read buffer (HP)
#define BDINDEX 0 //GPIB board address

#ifndef PRIMARY_ADDR_HP
#define PRIMARY_ADDR_HP 18 //Primary address of HP
#endif

#define NO_SEC_ADD 0 //Sec of HP - 0
#define TIMEOUT 13 //TIME OUT VALUE FOR READ/WRITE 
#define EOTMODE 1//ENABLE END MESSAGE
#define EOSMODE 0 // DISABLE EOS MODE

#define ERRMSGSIZE 1024 //MAX SIZE OF SCPI COMMAND STRING

char ValueStr[ARRAYSIZE_RD + 1];
//Error codes - HP, int maps to code
static char ErrorMnemonic[29][5] = {
							  "EDVR", "ECIC", "ENOL", "EADR", "EARG",
							  "ESAC", "EABO", "ENEB", "EDMA", "",
							  "EOIP", "ECAP", "EFSO", "",     "EBUS",
							  "ESTB", "ESRQ", "",     "",      "",
							  "ETAB", "ELCK", "EARM", "EHDL",  "",
							  "",     "EWIP", "ERST", "EPWR"
};

const int setLRangeArrSize = 4;
const int setLRangeRangeInd = 1;

const int setRangeArrSize = 4;
const int setRangeRangeInd = 1;

const int setIntTimeArrSize = 3;
const int setIntTimeInd = 1;

const int setCompArrSize = 3;
const int setCompInd = 1;

const int setModeArrSize = 3;
const int setModeInd = 1;

namespace HP
{
	hpCmd::hpCmd() {
		static char const lRangeCMD[] = "H10";
		strcpy_s(setLRangeCMD_, setLRangeArrSize, lRangeCMD);
		static char const rangeCMD[] = "RA1";
		strcpy_s(setRangeCMD_, setRangeArrSize, rangeCMD);
		static char const intTimeCMD[] = "I2";
		strcpy_s(setIntTimeCMD_, setIntTimeArrSize, intTimeCMD);
		static char const VAcompCMD[] = "L2";
		strcpy_s(setVAcompCMD_, setCompArrSize, VAcompCMD);
		static char const VBcompCMD[] = "M2";
		strcpy_s(setVBcompCMD_, setCompArrSize, VBcompCMD);
		static char const VAmodeCMD[] = "A5";
		strcpy_s(setVAmodeCMD_, setModeArrSize, VAmodeCMD);
		static char const VBmodeCMD[] = "B1";
		strcpy_s(setVBmodeCMD_, setModeArrSize, VBmodeCMD);
		//static char const vAForce[] = "PA+0000.00;"
		
		//std::cout << setVAcompCMD_ << std::endl;
	


		//Open handle to Keithley
		Dev_ = ibdev(BDINDEX, PRIMARY_ADDR_HP, NO_SEC_ADD,
			TIMEOUT, EOTMODE, EOSMODE);
		if (Ibsta() & ERR)
		{
			printf("Unable to open device\nibsta = 0x%x iberr = %d\n",
				Ibsta(), Iberr());
			hpStatus = 1;
			return;
		}
		else
			std::cout << "Picoammeter properly initialized" << std::endl;

		//Clear any commands
		ibclr(Dev_);
		if (Ibsta() & ERR)
		{
			_GPIBCleanup("Unable to clear device");
			hpStatus = 1;
			return;
		}
		//ibwrt(Dev_, "US", 2);
		//ibwrt(Dev_, "DE CH2", 6);
		//ibwrt(Dev_, "IT1", 3);
		//ibwrt(Dev_, "RI 2, 1E-3, 1E-3", 16);
		//ibwrt(Dev_, "RG 2, 1E-3", 10);

		//Set current mode
		ibwrt(Dev_, "F1", 2);
		//ibwrt(Dev_, "A5");
		//char cmd4[] = "RI 2, 1E-5, 1E-5";
		//ibwrt(Dev_, cmd4, strlen(cmd4));
		hpStatus = 0;
	}

	void hpCmd::_GPIBCleanup(const char* ErrorMsg)
	{
		std::cout << "Error : " << ErrorMsg << std::endl;
		std::cout << "ibsta = 0x" << Ibsta() << " iberr = " << Iberr() <<
			"(" << ErrorMnemonic[Iberr()] << ")" << std::endl;
		if (hpStatus)
		{
			std::cout << "Cleanup: Taking device off-line" << std::endl;
			ibonl(Dev_, 0);

		}
	}

	int hpCmd::setVAcomp(const int vAcomp)
	{
		char j = '0' + vAcomp;
		setVAcompCMD_[setCompInd] = j;
		//std::cout << setVAcompCMD_ << std::endl;
		ibwrt(Dev_, setVAcompCMD_, setCompArrSize);
		if (Ibsta() & ERR)
		{
			_GPIBCleanup("Unable to set VA compliance");
			hpStatus = 1;
			return hpStatus;
		}
		return 0;
	}

	int hpCmd::setVBcomp(const int vBcomp)
	{
		char j = '0' + vBcomp;
		setVBcompCMD_[setCompInd] = j;
		//std::cout << setVBcompCMD_ << std::endl;
		ibwrt(Dev_, setVBcompCMD_, setCompArrSize);
		if (Ibsta() & ERR)
		{
			_GPIBCleanup("Unable to set VB compliance");
			hpStatus = 1;
			return hpStatus;
		}
		return 0;
	}

	int hpCmd::setVAmode(const int vAmode)
	{
		char j = '0' + vAmode;
		setVAmodeCMD_[setModeInd] = j;
		//std::cout << setVBcompCMD_ << std::endl;
		ibwrt(Dev_, setVAmodeCMD_, setModeArrSize);
		if (Ibsta() & ERR)
		{
			_GPIBCleanup("Unable to set VB compliance");
			hpStatus = 1;
			return hpStatus;
		}
		return 0;
	}

	int hpCmd::setVBmode(const int vBmode)
	{
		char j = '0' + vBmode;
		setVBmodeCMD_[setModeInd] = j;
		//std::cout << setVBcompCMD_ << std::endl;
		ibwrt(Dev_, setVBmodeCMD_, setModeArrSize);
		if (Ibsta() & ERR)
		{
			_GPIBCleanup("Unable to set VB compliance");
			hpStatus = 1;
			return hpStatus;
		}
		return 0;
	}






	int hpCmd::setLRange(const int lRange)
	{
		char j2 = '0' + (lRange % 10);
		char j = '0' + (lRange / 10) ;
		setLRangeCMD_[setLRangeRangeInd] = j;
		setLRangeCMD_[setLRangeRangeInd + 1] = j2;
		////std::cout << setLRangeCMD_ << std::endl;
		ibwrt(Dev_, setLRangeCMD_, setLRangeArrSize);
		if (Ibsta() & ERR)
		{
			_GPIBCleanup("Unable to set lowest range");
			hpStatus = 1;
			return hpStatus;
		}
		return 0;
	}

	int hpCmd::setRange(const int range)
	{
		char j;
		if (range == 1)
		{
			j = 'A';
		}
		else {
			j = '0' + (range / 10);
		}
		char j2 = '0' + (range % 10);
		setRangeCMD_[setRangeRangeInd] = j;
		setRangeCMD_[setRangeRangeInd + 1] = j2;
		//std::cout << setRangeCMD_ << std::endl;
		ibwrt(Dev_, setRangeCMD_, setRangeArrSize);
		if (Ibsta() & ERR)
		{
			_GPIBCleanup("Unable to set lowest range");
			hpStatus = 1;
			return hpStatus;
		}
		return 0;
	}

	int hpCmd::setIntTime(const int intTime)
	{
		char j = '0' + intTime;
		setIntTimeCMD_[setIntTimeInd] = j;
		//std::cout << setIntTimeCMD_ << std::endl;
		ibwrt(Dev_, setIntTimeCMD_, setIntTimeArrSize);
		if (Ibsta() & ERR)
		{
			_GPIBCleanup("Unable to set lowest range");
			hpStatus = 1;
			return hpStatus;
		}
		return 0;
	}

	int hpCmd::srcZeroAll() {
		//vForce('A', 0);
		//vForce('B', 0);
		ibwrt(Dev_, "W7", 2);
		return 0;
	}

	int hpCmd::vForce(double vA, double vB)
	{
		std::string strCMD = "PA" + std::to_string(vA)+",";
		std::cout << strCMD << std::endl;
		char* charCMD;
		charCMD = &strCMD[0];
		DWORD cmdSize = strlen(strCMD.c_str());
		ibwrt(Dev_, charCMD, cmdSize);
		strCMD = "PB" + std::to_string(vB) + ",";
		std::cout << strCMD << std::endl;
		charCMD = &strCMD[0];
		cmdSize = strlen(strCMD.c_str());
		ibwrt(Dev_, charCMD, cmdSize);

		//Trigger
		ibwrt(Dev_, "W1", 2);
		//std::cout<<vForceCMD_<<std::endl;
		//GPIBCleanup(Dev, "Unable to write to multimeter");
		if (Ibsta() & ERR)
		{
			_GPIBCleanup("Unable to write to multimeter");
			return 1;
		}
		return 0;
	}

	int hpCmd::iMeas(double &measVal)
	{
		char trigCmd[] = "E";
		ibwrt(Dev_, trigCmd, 1);
		ibrd(Dev_, ValueStr, ARRAYSIZE_RD);
		//std::cout << ValueStr << std::endl;
		if (Ibsta() & ERR)
		{
			_GPIBCleanup("Unable to read data from multimeter");
			return 1;
		}
		//measVal = 2.3;
		//sscanf_s(ValueStr, "%*[^0-9]%lf", &measVal);

		std::string strVal = ValueStr;
		measVal = std::stod(strVal.substr(strVal.find("NI")+2, strVal.find(",")-1));
		return 0;
	}

	int hpCmd::sendPersCmd(char cmd[], int len)
	{
		ibwrt(Dev_, cmd, len);
		if (Ibsta() & ERR)
		{
			_GPIBCleanup("Command issue");
			hpStatus = 1;
			return 1;
		}
		return 0;
	}
	hpCmd::~hpCmd() {

		// The device is taken offline.
		ibwrt(Dev_, "W7", 2);
		ibonl(Dev_, 0);
		std::cout << "Picoammeter has been shut down" << std::endl;
	}
}



