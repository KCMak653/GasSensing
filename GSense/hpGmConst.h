#pragma once



//Keithley apply constant bias class, base for vStep class

//Class to create constant bias object



//Input parameters
struct constHPGMParameters
{
	double constVA; //Constant bias to apply [V]
	double constVB; //
	double measTime; //Total measurement time [s]
	int dtGas; //Measurement frequency for gas mixer [ms]
	int lRange; //Order of mag of lowest range [A]
	int range; //Order of mag of I range [A]
	int comp; //Compliance, max I value [A]
	int intTime; //Integration time (1,2,3)(Fast, Normal, Long)
	//bool setV; //Whether to set V
	int dtHP; //Measurement frequency for HP [ms]

	double flowRate;
	double gasConc;
	//char constSMU1; //SMU to keep constant
	//char constSMU2;
};

namespace HPGM
{
	class hpGmConst
	{
	public:
		//Constructor to initialzie keithley and set member variables
		hpGmConst(const constHPGMParameters& entries);

		int runTest(double fRMs[], double cMs[], int sizeArrayGas, int iStartGas, double iMs[], double tMs[], unsigned long dMs[], int sizeArrayHP, int iStartHP);

		int arraySizeNeededGas();

		int arraySizeNeededHP();

		int setMeasTime(double measTime);

		int setGasConc(double gasConc);

		int setFlowRate(double flowRate);

		int setConstVA(double v);

		int setConstVB(double v);

		~hpGmConst();

	private:
		int dtGas_; //Measurement frequency [ms]
		int dtHP_; 
		double measTime_; //Total measurement time [s]
		double constVA_; //Constant bias [V]
		double constVB_;
		int lRange_; //Order of mag of lowest range [A]
		int range_; //Order of mag of I range [A]
		int comp_; //Compliance, max I value [A}
		int intTime_; //Integration time (1,2,3)(Fast, Normal, Long)
		HP::hpCmd* picoAm_; //Ptr to Keithley command
		GM::gmCmd* gasM_; //Ptr to gas mixer
		double flowRate_;
		double gasConc_;
		int dtMin_; //Minimum time of each measurement [ms]
		int sizeArrayNeededHP_; //Size of array to store measurements
		int sizeArrayNeededGas_;
		int sizeArrayNeededStepHP_;
		int sizeArrayNeededStepGas_;
		int relFreqHPGas_;
		bool setV_; //Whether to set V, if true will set to new values (restarts system)
		//int constSMU1_; //SMU to keep const
		//int constSMU2_; //SMU to measure
	};
}





