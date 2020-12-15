//HP base sweep class

//Class to create voltage sweep object

#pragma once


//Input parameters
struct sweepParameters
{
	double startV; //Start of sweep [V]
	double stopV; //End of sweep [V]
	double SR; //Scan rate [V/s]
	double constV; //Constant bias to apply
	int lRange; //Order of mag of lowest range [A]
	int range; //Order of mag of I range [A]
	int comp; //Compliance, max I value [A}
	int intTime; //Integration time (1,2,3)(Fast, Normal, Long)
	char sweepSMU; //SMU to sweep (1 -> 4)
	char constSMU; //SMU to keep constant
	//int measSMU; //SMU to measure
};

namespace HP
{
	class hpSweep
	{
	public:
		//Constructor initializes the Keithley and applies
		//the machine settings
		hpSweep(const sweepParameters& entries);

		//Run the sweep
		int runSweep(double vFs[], double iMs[], double tMs[], unsigned long dMs[], int sizeArray, int iStart);

		//Returns the size of the array needed based on startV, stopV, SR and int time
		int arraySizeNeeded();

		//Reverse the stopV and stopV - Change direction of sweep
		int reverseV();

		//Apply a constant bias
		//int constBias(double constV);

		~hpSweep();

	private:
		double startV_; //Start of sweep [V]
		double stopV_; //End of sweep [V]
		double SR_; //Scan rate [V/s]
		double constV_; //Constant bias [V]
		int lRange_; //Order of mag of lowest range [A]
		int range_; //Order of mag of I range [A]
		int comp_; //Compliance, max I value [A}
		int intTime_; //Integration time (1,2,3)(Fast, Normal, Long)
		HP::hpCmd* picoAm_; //Ptr to Keithley command
		int dtMeas_; //Time of each measurement [ms]
		double stepV_; //Voltage step between measurements [V]
		int sizeArrayNeeded_; //Size of array to store measurements
		char sweepSMU_; //SMU to sweep (1 -> 4)
		char constSMU_; //SMU to keep const
		//int measSMU_; //SMU to measure

	};
}