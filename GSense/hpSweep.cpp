//Source file for Keithley base sweep class

#include "hpCmd.h"
#include "hpSweep.h"
#include <string.h>
#include<Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include<iostream>
#include<chrono>
#include<thread>



namespace HP
{
	hpSweep::hpSweep(const sweepParameters& entries) {
		//Set all private member variables from the inputted parameters
		sweepSMU_ = entries.sweepSMU;
		constSMU_ = entries.constSMU;
		//measSMU_ = entries.measSMU;
		SR_ = entries.SR;
		startV_ = entries.startV;
		stopV_ = entries.stopV;
		constV_ = entries.constV;
		lRange_ = entries.lRange;
		range_ = entries.range;
		comp_ = entries.comp;
		intTime_ = entries.intTime;

		//Open handle to Keithley
		picoAm_ = new HP::hpCmd();

		//Set the machine parameters: Compliance, range, integration time 
		picoAm_->setVAcomp(comp_);
		picoAm_->setVBcomp(comp_);
		picoAm_->setLRange(lRange_);
		picoAm_->setRange(range_);
		picoAm_->setIntTime(intTime_);

		//Set the time of each measurement based on the
		//desired integration time
		if (intTime_ == 1) {
			dtMeas_ = 170; //ms
		}
		else if (intTime_ == 2) {
			dtMeas_ = 580; //ms
		}
		else if (intTime_ == 3) {
			dtMeas_ = 3400; //ms
		}
		else
		{
			std::cout << "Invalid time integration mode" << std::endl;

			return;
		}

		//Set the voltage step based on measurement time and SR
		stepV_ = SR_ * dtMeas_ * 1e-3;

		//Calculate the size of array needed to record measurements
		sizeArrayNeeded_ = (fabs(startV_ - stopV_) / (stepV_)) + 1.5;

		//Ensure stepV has the correct sign
		if (startV_ > stopV_) stepV_ = -1 * stepV_;

		//Force all SMUs to 0V
		picoAm_->srcZeroAll();
	}

	int hpSweep::runSweep(double vFs[], double iMs[], double tMs[], unsigned long dMs[], int sizeArray, int iStart) {
		//First ensure that the correct array size has been passed as argument
		if (sizeArrayNeeded_ != sizeArray) {
			std::cout << "Wrong array size" << std::endl;
			return 1;
		}

		//Apply constant bias if there is one:
		
		picoAm_->vForce(constSMU_, constV_);
		

		//Run the sweep. 

		//Force V, then measure I and time
		//Delay for the remainder of the measurment time
		//increment the voltage and repeat

		unsigned long delayT;

		//initalize voltage to start of sweep
		double v = startV_;

		//Initiate the timer
		auto clk = std::chrono::high_resolution_clock::now();

		//Force voltage
		picoAm_->vForce(sweepSMU_, v);

		//Record voltage
		vFs[iStart] = v;
		auto clk2 = std::chrono::high_resolution_clock::now();

		//Increment voltage
		v = v + stepV_;

		//Measure and store current
		picoAm_->iMeas(iMs[iStart]);

		//Record the time
		tMs[iStart] = std::chrono::duration_cast<std::chrono::milliseconds> (clk2 - clk).count();

		//Calculate amount of time to delay
		delayT = dtMeas_ - tMs[iStart];

		//Set delay to zero if negative
		if (dtMeas_ - tMs[iStart] < 0) { delayT = 0; }

		//Record delay
		dMs[iStart] = delayT;

		//Sleep for remainder of measurement period
		std::this_thread::sleep_for(std::chrono::milliseconds(delayT));

		//Repeat for length of array, since multiple sweeps may occur,
		//the indices of where to store data in array will differe by iStart
		for (int i = (iStart + 1); i < (iStart + sizeArray); i++) {

			picoAm_->vForce(sweepSMU_, v);
			vFs[i] = v;
			v = v + stepV_;
			picoAm_->iMeas(iMs[i]);
			clk2 = std::chrono::high_resolution_clock::now();
			tMs[i] = std::chrono::duration_cast<std::chrono::milliseconds> (clk2 - clk).count();
			delayT = dtMeas_ * (i + 1) - tMs[i];
			if ((dtMeas_ * (i + 1) - tMs[i]) < 0) { delayT = 0; }
			dMs[i] = delayT;
			std::this_thread::sleep_for(std::chrono::milliseconds(delayT));
		}

		return 0;
	}

	int hpSweep::arraySizeNeeded() {
		return sizeArrayNeeded_;
	}

	int hpSweep::reverseV() {
		double temp = startV_;
		startV_ = stopV_;
		stopV_ = temp;
		stepV_ = stepV_ * -1;
		return 0;
	}
	/*
	int hpSweep::constBias(double constV)
	{
		if (constSMU_ ==0) return 1;
		picoAm_->vForce(constSMU_,v);
		return 0;
	}*/

	hpSweep::~hpSweep() {
		picoAm_->srcZeroAll();
		delete picoAm_;
	}

}
