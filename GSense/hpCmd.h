#pragma once


namespace HP
{
	class hpCmd
	{
	public:
		hpCmd();
		int hpStatus;
		
		int srcZeroAll();
		int setVAcomp(const int vAcomp);
		int setVAmode(const int vAmode);
		int setVBmode(const int vBmode);
		int setVBcomp(const int vBComp);
		int setLRange(const int lRange);
		int setRange(const int range);
		int setIntTime(const int intTime);
		int vForce(double vA, double vB);
		int sendPersCmd(char cmd[], int len);
		int iMeas(double& measVal);
		
		~hpCmd();
		
	private:
		//vForceCMD_[];
		char setLRangeCMD_[4];
		char setRangeCMD_[4];
		char setIntTimeCMD_[3];
		char setVAcompCMD_[3];
		char setVBcompCMD_[3];
		char setVBmodeCMD_[3];
		char setVAmodeCMD_[3];
		void _GPIBCleanup(const char* ErrorMsg);
		//int _updateCMD(double val);
		int Dev_;
	};
}
