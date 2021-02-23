//Keithley programs
//sweepVDS_IDS: Sweep V, measure I - Any 3 electrode configuration possible
	//i.e Apply const Bias to SMU, sweep other, measure another

#pragma once
#include<string.h>
#include<string>
/*
//Sweep parameters
struct sweepVDS_IDSParameters
{
	double startV; //Start of sweep [V]
	double stopV; //End of sweep [V]
	double SR; //Scan rate [V/s]
	double constV;
	int lRange; //Order of mag of lowest range [A]
	int range; //Order of mag of I range [A]
	int comp; //Compliance, max I value [A}
	int intTime; //Integration time (1,2,3)(Fast, Normal, Long)
	char sweepSMU; //SMU to force and measure (1 -> 4)
	//int measSMU; //SMU to measure
	char constSMU; //Applied constant bias 
	int nCycles; //Number of cycles
	bool fullCycle; //Return to startV
};
*/

struct pulseGas_constVDS_IDSParameters
{
	double constVA; //Constant bias to apply [V]
	double constVB;
	double initTime; //Initial settle time [s]
	double stepTime; //Time between pulses [s]
	double pulseTime; //Pulse time [s]
	double endTime; //End time [s]
	int nPulses;
	double dtGas; //Measurement frequency [ms]
	double dtHP;
	double flowRate;
	double gasConc;
	int lRange; //Order of mag of lowest range [A]
	int range; //Order of mag of I range [A]
	int comp; //Compliance, max I value [A}
	int intTime; //Integration time (1,2,3)(Fast, Normal, Long)
};

struct constVDS_IDSParameters
{
	double constVA; //Constant bias to apply [V]
	double constVB;
	double measTime; //Total measurement time [s]
	double dt; //Measurement frequency [ms]
	int lRange; //Order of mag of lowest range [A]
	int range; //Order of mag of I range [A]
	int comp; //Compliance, max I value [A}
	int intTime; //Integration time (1,2,3)(Fast, Normal, Long)
	//char constSMU1; //SMU to keep constant
	//char constSMU2;
	//int measSMU; //SMU to measure
};
/*
struct stepVDS_IDSParameters
{
	double startV; //Start step voltage [V]
	double stopV; //Max voltage to apply [V]
	double stepV; //Size of voltage step [V]
	double constV; //Constant bias to apply [V]
	double stepTime; //Step measurement time [s]
	double dt; //Measurement frequency [ms]
	int lRange; //Order of mag of lowest range [A]
	int range; //Order of mag of I range [A]
	int comp; //Compliance, max I value [A}
	int intTime; //Integration time (1,2,3)(Fast, Normal, Long)
	int nCycles; //Number of cycles
	bool fullCycle;
	char constSMU; //SMU to keep constant
	char stepSMU; //SMU to step
	//int measSMU; //SMU to measure
};
*/
namespace HPGM
{
	/*
	class sweepVDS_IDS
	{
	public:
		//Constructor creates sweep object and applies all machine settings
		sweepVDS_IDS(const sweepVDS_IDSParameters& entries);

		//Runs the program, stores results in passed arrays
		int runProgram(double vFs[], double iMs[], double tMs[], unsigned long dMs[], int sizeArray);

		//Returns the size of array needed to store measurements
		int arraySizeNeeded();

		//Save the data
		int saveData(std::string fn, double vFs[], double iMs[], double tMs[], unsigned long dMs[], int sizeArray);

		~sweepVDS_IDS();
	private:
		sweepParameters sweepP_; //Parameters for sweep class
		HP::hpSweep* swp_; //Ptr to sweep object
		int swpSize_; // Size of a single sweep
		int nCycles_; //Number of cycles
		bool fullCycle_; //Return to startV
		int sizeArrayNeeded; //Size of array to hold values
	};
	*/
	class constVDS_IDS
	{
	public:
		//Constructor creates sweep object and applies all machine settings
		constVDS_IDS(const constVDS_IDSParameters& entries);

		//Runs the program, stores results in passed arrays
		int runProgram(double iMs[], double tMs[], unsigned long dMs[], int sizeArray);

		//Returns the size of array needed to store measurements
		int arraySizeNeeded();


		//Save the data
		int saveData(std::string fn,  double iMs[], double tMs[], unsigned long dMs[], int sizeArray);

		~constVDS_IDS();
	private:
		constParameters constP_; //Parameters for const class
		HP::hpConst* cnst_; //Ptr to sweep object
		int cnstSize_; // Size of a single sweep
		int sizeArrayNeeded; //Size of array to hold values

	};

	class pulseGas_constVDS_IDS
	{
	public:
		pulseGas_constVDS_IDS(const pulseGas_constVDS_IDSParameters& entries);
		int runProgram(double fRMs[], double cMs[], int sizeArrayGas, double iMs[], double tMs[], unsigned long dMs[], int sizeArrayHP);
		int arraySizeNeededGas();
		int arraySizeNeededHP();
		int saveData(std::string fn, double fRMs[], double cMs[], int sizeArrayGas, double iMs[], double tMs[], unsigned long dMs[], int sizeArrayHP);
		~pulseGas_constVDS_IDS();
	private:
		constHPGMParameters constP_;
		HPGM::hpGmConst* cnst_;
		int sizeArrayNeededGas_;
		int sizeArrayNeededHP_;
		int initSizeGas_;
		int endSizeGas_;
		int stepSizeGas_;
		int pulseSizeGas_;
		int initSizeHP_;
		int endSizeHP_;
		int stepSizeHP_;
		int pulseSizeHP_;
		double gasConc_;
		double initTime_;
		double pulseTime_;
		double stepTime_;
		double endTime_;
		int nPulses_;
	};
	/*
	class stepVDS_IDS
	{
	public:
		stepVDS_IDS(const stepVDS_IDSParameters& entries);
		int runProgram(double vFs[], double iMs[], double tMs[], unsigned long dMs[], int sizeArray);
		int arraySizeNeeded();
		//int saveData(std::string fn, double vFs[], int sizeArray, double iMs[], double tMs[]);
		int runFlight(double vFs[], double iMs[], double tMs[], unsigned long dMs[], int iStart, int nS, double v);
		int saveData(std::string fn, double vFs[], double iMs[], double tMs[], unsigned long dMs[], int sizeArray);
		~stepVDS_IDS();
	private:
		constParameters stepP_;
		HP::hpConst* step_;
		int stepSize_;
		int nCycles_;
		bool fullCycle_;
		int nSteps_;
		int nStepsTot_;
		double stopV_;
		double startV_;
		double stepV_;
		double constV_;
		double constSMU_;
		int sizeArrayNeeded_;
		void reverseV();

	};
	*/
}