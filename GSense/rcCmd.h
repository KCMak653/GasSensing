//Channel relay command class

#pragma once

//This class creates relay channel object, communicates to it over USB via a virtual COM port


namespace RC
{
	class rcCmd
	{
	public:
		//Constructor opens port and establishes communication
		rcCmd();

		//Turns a specified device on
		int chOn(int dev);

		//Turns a specified dev off
		int chOff(int dev);

		//Switch one dev on and the other off
		int chSwitch(int devOn, int devOff);

		//Turns all devices on
		int chAllOn();

		//Turns all devices off
		int chAllOff();


		//Deconstructor closes handle to device
		~rcCmd();

	private:
		bool Status;
		HANDLE hComm_;
		std::string ComPortName_{};
		std::string CMD_{};
		int sendCmd_();
		int purge();
		//char devCmd[]_;
		//DCB dcbSerialParams_;
	};
}
