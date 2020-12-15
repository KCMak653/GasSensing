#pragma once

//Keithley apply constant bias class, base for vStep class

//Class to create constant bias object

#pragma once


//Input parameters
struct constParameters
{
	double constV1; //Constant bias to apply [V]
	double constV2; //
	double measTime; //Total measurement time [s]
	double dt; //Measurement frequency [ms]
	int lRange; //Order of mag of lowest range [A]
	int range; //Order of mag of I range [A]
	int comp; //Compliance, max I value [A]
	int intTime; //Integration time (1,2,3)(Fast, Normal, Long)
	char constSMU1; //SMU to keep constant
	char constSMU2;
};

namespace HP
{
	class hpConst
	{
	public:
		//Constructor to initialzie keithley and set member variables
		hpConst(const constParameters& entries);

		int runTest(double vFs[], double iMs[], double tMs[], unsigned long dMs[], int sizeArray, int iStart);

		int setV1(double v);

		int arraySizeNeeded();

		~hpConst();

	private:
		double dt_; //Measurement frequency [ms]
		double measTime_; //Total measurement time [s]
		double constV1_; //Constant bias [V]
		double constV2_;
		int lRange_; //Order of mag of lowest range [A]
		int range_; //Order of mag of I range [A]
		int comp_; //Compliance, max I value [A}
		int intTime_; //Integration time (1,2,3)(Fast, Normal, Long)
		HP::hpCmd* picoAm_; //Ptr to Keithley command
		int dtMin_; //Minimum time of each measurement [ms]
		int sizeArrayNeeded_; //Size of array to store measurements
		int constSMU1_; //SMU to keep const
		int constSMU2_; //SMU to measure
	};
}





