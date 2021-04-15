#include "hpCmd.h"
#include "hpConst.h"
#include <string.h>
#include<Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <conio.h>
#include<iostream>
#include<chrono>
#include <thread>

namespace HP
{
	hpConst::hpConst(const constParameters& entries)
	{
		dt_ = entries.dt;
		measTime_ = entries.measTime;
		constVA_ = entries.constVA;
		constVB_ = entries.constVB;
		//constSMU1_ = entries.constSMU1;
		//constSMU2_ = entries.constSMU2;
		//measSMU_ = entries.measSMU;
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
		picoAm_->setVAmode(5);
		picoAm_->setVBmode(1);


		//Set the minimum time of each measurement based on the
		//desired integration time
		if (intTime_ == 1) {
			dtMin_ = 170; //ms
		}
		else if (intTime_ == 2) {
			dtMin_ = 580; //ms
		}
		else if (intTime_ == 3) {
			dtMin_ = 3400; //ms
		}
		else
		{
			std::cout << "Invalid time integration mode" << std::endl;

			return;
		}

		if (dtMin_ > dt_) {
			std::cout << "Measurement frequency too high for integration time" << std::endl;
			return;
		}

		//Calculate the size of array needed to record measurements
		sizeArrayNeeded_ = measTime_ * 1e3 / dt_ + 1.5;

		//Force all SMUs to 0V
		picoAm_->srcZeroAll();
	}

	int hpConst::setV(double vA, double vB) {
		constVA_ = vA;
		constVB_ = vB;
		return 0;
	}

	int hpConst::setMeasTime(double t) {
		measTime_ = t;
		sizeArrayNeeded_ = measTime_ * 1e3 / dt_ + 1.5;
		return 0;
	}

	int hpConst::runTest(double iMs[], double tMs[], unsigned long dMs[], int sizeArray, int iStart)
	{
		//First ensure that the correct array size has been passed as argument
		if (sizeArrayNeeded_ != sizeArray) {
			std::cout << "Wrong array size" << std::endl;
			return 1;
		}
		unsigned long delayT;

		//Initiate the timer
		//auto clk = std::chrono::high_resolution_clock::now();
		//auto clk2 = std::chrono::high_resolution_clock::now();
		auto clk = std::chrono::high_resolution_clock::now();

		//Apply constant bias
		//std::cout << "V: " << constV1_ << std::endl;

		//picoAm_->vForce(constSMU2_, v2);
		//_getch();
		picoAm_->vForce(constVA_, constVB_);

		

		
		

		//Run the sweep. 

		//Measure I and time
		//Delay for the remainder of the measurment time

		//vFs[iStart] = v1;
		auto clk2 = std::chrono::high_resolution_clock::now();
		//Measure and store current
		picoAm_->iMeas(iMs[iStart]);

		//Record the time
		tMs[iStart] = std::chrono::duration_cast<std::chrono::milliseconds> (clk2 - clk).count();

		//Calculate amount of time to delay
		delayT = dt_ - tMs[iStart];

		//Set delay to zero if negative
		if (dt_ - tMs[iStart] < 0) { delayT = 0; }

		//Record delay
		dMs[iStart] = delayT;

		//Sleep for remainder of measurement period

		std::this_thread::sleep_for(std::chrono::milliseconds(delayT));

		//Repeat for length of array, since multiple sweeps may occur,
		//the indices of where to store data in array will differe by iStart
		for (int i = (iStart + 1); i < (iStart + sizeArray); i++) {
			//vFs[i] = v1;
			picoAm_->iMeas(iMs[i]);
			clk2 = std::chrono::high_resolution_clock::now();
			tMs[i] = std::chrono::duration_cast<std::chrono::milliseconds> (clk2 - clk).count();
			delayT = dt_ * (i + 1) - tMs[i];
			if ((dt_ * (i + 1) - tMs[i]) < 0) { delayT = 0; }
			dMs[i] = delayT;
			std::this_thread::sleep_for(std::chrono::milliseconds(delayT));
		}

		return 0;
	}

	int hpConst::arraySizeNeeded() {
		std::cout << "Array size" << sizeArrayNeeded_ << std::endl;
		return sizeArrayNeeded_;
	}
	/*
	int hpConst::setV1(double v)
	{
		constV1_ = v;
		return 0;
	}
	*/
	hpConst::~hpConst() {
		picoAm_->srcZeroAll();
		delete picoAm_;
	}
}

