#include "hpCmd.h"
#include "gmCmd.h"
#include "hpGmConst.h"
#include <string.h>
#include<Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <conio.h>
#include<iostream>
#include<chrono>
#include <thread>

namespace HPGM
{
	hpGmConst::hpGmConst(const constHPGMParameters& entries)
	{
		dtGas_ = entries.dtGas;
		dtHP_ = entries.dtHP;
		measTime_ = entries.measTime;
		constVA_ = entries.constVA;
		constVB_ = entries.constVB;
		//setV_ = entries.setV;
		setV_ = TRUE;
		//constSMU1_ = entries.constSMU1;
		//constSMU2_ = entries.constSMU2;
		//measSMU_ = entries.measSMU;
		lRange_ = entries.lRange;
		range_ = entries.range;
		comp_ = entries.comp;
		intTime_ = entries.intTime;

		gasConc_ = entries.gasConc;
		flowRate_ = entries.flowRate;

		//Open handle to Keithley
		picoAm_ = new HP::hpCmd();
		gasM_ = new GM::gmCmd();

		//Set the machine parameters: Compliance, range, integration time 
		picoAm_->setVAcomp(comp_);
		picoAm_->setVBcomp(comp_);
		picoAm_->setLRange(lRange_);
		picoAm_->setRange(range_);
		picoAm_->setIntTime(intTime_);
		picoAm_->setVAmode(5);
		picoAm_->setVBmode(1);

		gasM_->setCarrBase();

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

		if (dtMin_ > dtHP_) {
			std::cout << "Measurement frequency too high for integration time" << std::endl;
			return;
		}

		if ((dtGas_ % dtHP_) != 0) {
			std::cout << "Meas frequency of gas needs to be multiple of HP" << std::endl;
			return;
		}

		//Calculate the size of array needed to record measurements
		sizeArrayNeededHP_ = (measTime_ * 1e3 / dtHP_ + 0.5);
		std::cout << "HP array: " << sizeArrayNeededHP_ << std::endl;
		sizeArrayNeededGas_ = (measTime_ * 1e3 / dtGas_ + 0.5);
		std::cout << "Gas array: " << sizeArrayNeededGas_ << std::endl;
		relFreqHPGas_ = (sizeArrayNeededHP_ - 1) / (sizeArrayNeededGas_ - 1);
		std::cout << "relFreq: " << relFreqHPGas_ << std::endl;
		//Force all SMUs to 0V
		picoAm_->srcZeroAll();
	}

	int hpGmConst::runTest(double fRMs[], double cMs[], int sizeArrayGas, int iStartGas, double iMs[], double tMs[], unsigned long dMs[], int sizeArrayHP, int iStartHP)
	{
		//First ensure that the correct array size has been passed as argument
		if (sizeArrayNeededGas_ != sizeArrayGas) {
			std::cout << "Wrong array size - Gas" << std::endl;
			return 1;
		}

		if (sizeArrayNeededHP_ != sizeArrayHP) {
			std::cout << "Wrong array size - HP" << std::endl;
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
		//Only force V if setV is true
		if (setV_) {
			picoAm_->vForce(constVA_, constVB_);
			setV_ = FALSE;
		}

		gasM_->setAnalyteGas(3);
		gasM_->setFlowRate(flowRate_);
		gasM_->setAnalyteConc(gasConc_);


		//Run the sweep. 

		//Measure I and time
		//Delay for the remainder of the measurment time

		//vFs[iStart] = v1;
		auto clk2 = std::chrono::high_resolution_clock::now();
		//Measure and store current
		picoAm_->iMeas(iMs[iStartHP]);
		gasM_->measFlowRate(fRMs[iStartGas], FALSE);
		gasM_->measAnalyteConc(cMs[iStartGas], FALSE);

		//Record the time
		tMs[iStartHP] = std::chrono::duration_cast<std::chrono::milliseconds> (clk2 - clk).count();

		//Calculate amount of time to delay
		delayT = dtHP_ - tMs[iStartHP];

		//Set delay to zero if negative
		if (dtHP_ - tMs[iStartHP] < 0) { delayT = 0; }

		//Record delay
		dMs[iStartHP] = delayT;

		//Sleep for remainder of measurement period

		std::this_thread::sleep_for(std::chrono::milliseconds(delayT));

		//Repeat for length of array, since multiple sweeps may occur,
		//the indices of where to store data in array will differe by iStart
		iStartHP = iStartHP + 1;
		iStartGas = iStartGas + 1;

		for (int i = (iStartGas); i < (iStartGas + sizeArrayNeededGas_); i++) {

			for (int j = (iStartHP); j < (iStartHP + relFreqHPGas_ ); j++) {

				picoAm_->iMeas(iMs[j]);

				clk2 = std::chrono::high_resolution_clock::now();
				tMs[j] = std::chrono::duration_cast<std::chrono::milliseconds> (clk2 - clk).count();

				delayT = dtHP_ * (j + 1) - tMs[j];
				if ((dtHP_ * (j + 1) - tMs[j]) < 0) { delayT = 0; }
				dMs[j] = delayT;

				std::this_thread::sleep_for(std::chrono::milliseconds(delayT));
			}

			gasM_->measFlowRate(fRMs[i], FALSE);
			gasM_->measAnalyteConc(cMs[i], FALSE);
			iStartHP = iStartHP + relFreqHPGas_;
			std::cout << "\n Number of seconds remaining: " << (sizeArrayNeededHP_ - iStartHP+1)*dtHP_ << std::endl;

			}


		return 0;
	}

	int hpGmConst::arraySizeNeededGas() {
		std::cout << "Array size Gas" << sizeArrayNeededGas_ << std::endl;
		return sizeArrayNeededGas_;
	}

	int hpGmConst::arraySizeNeededHP(){
		std::cout << "Array size HP" << sizeArrayNeededHP_ << std::endl;
		return sizeArrayNeededHP_;
	}

	int hpGmConst::setMeasTime(double measTime) {
		measTime_ = measTime;
		sizeArrayNeededHP_ = (measTime_ * 1e3 / dtHP_ + 0.5);
		std::cout << "HP array: " << sizeArrayNeededHP_ << std::endl;
		sizeArrayNeededGas_ = (measTime_ * 1e3 / dtGas_ + 0.5);
		std::cout << "Gas array: " << sizeArrayNeededGas_ << std::endl;
		relFreqHPGas_ = (sizeArrayNeededHP_ - 1) / (sizeArrayNeededGas_ - 1);
		std::cout << "relFreq: " << relFreqHPGas_ << std::endl;
		return 0;
	}

	int hpGmConst::setGasConc(double gasConc) {
		gasConc_ = gasConc;
		return 0;
	}

	int hpGmConst::setFlowRate(double flowRate) {
		flowRate_ = flowRate;
		return 0;
	}

	int hpGmConst::setConstVA(double v) {
		constVA_ = v;
		setV_ = TRUE;
		return 0;
	}

	int hpGmConst::setConstVB(double v) {
		constVB_ = v;
		setV_ = TRUE;
		return 0;
	}
	/*
	int hpConst::setV1(double v)
	{
		constV1_ = v;
		return 0;
	}
	*/
	hpGmConst::~hpGmConst() {
		picoAm_->srcZeroAll();
		delete picoAm_;
		delete gasM_;
	}
}

