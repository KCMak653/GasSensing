#include "hpCmd.h"
#include "gmCmd.h"
#include "rcCmd.h"
#include "hpSweep.h"
#include "hpConst.h"
#include "hpGmConst.h"
#include "hpProgram.h"

#include <string.h>
#include<Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include<iostream>
#include<time.h>
#include<string.h>
#include<string>
#include <fstream>

namespace HPGM
{
	/*
	sweepVDS_IDS::sweepVDS_IDS(const sweepVDS_IDSParameters& entries)
	{
		//Set all private member variables to inputs
		sweepP_.sweepSMU = entries.sweepSMU;
		sweepP_.constSMU = entries.constSMU;
		sweepP_.SR = entries.SR;
		sweepP_.constV = entries.constV;
		sweepP_.startV = entries.startV;
		sweepP_.stopV = entries.stopV;
		sweepP_.lRange = entries.lRange;
		sweepP_.range = entries.range;
		sweepP_.comp = entries.comp;
		sweepP_.intTime = entries.intTime;

		fullCycle_ = entries.fullCycle;
		nCycles_ = entries.nCycles;


		//Initialize the sweep object
		swp_ = new HP::hpSweep(sweepP_);

		//Find the size of array needed to store values
		swpSize_ = swp_->arraySizeNeeded();
	}

	int sweepVDS_IDS::arraySizeNeeded()
	{
		int sizeArrayNeeded_ = (swpSize_ - 1) * (int(fullCycle_) + 1) * nCycles_
			+ (1 - int(fullCycle_)) * nCycles_ + 1;
		return sizeArrayNeeded_;
	}

	int sweepVDS_IDS::runProgram(double vFs[], double iMs[], double tMs[], unsigned long dMs[], int sizeArray)
	{
		//Set initial indice
		int iStart = 0;

		//Execute outer for loop for number of cycles requested
		//Execute inner loop only if full cycle 
		for (int j = 0; j < nCycles_; j++)
		{
			//Run the sweep
			swp_->runSweep(vFs, iMs, tMs, dMs, swpSize_, iStart);
			//Change the array indice
			iStart = iStart + swpSize_ - 1;
			if (fullCycle_)
			{
				//Reverse the direction of the sweep
				swp_->reverseV();
				swp_->runSweep(vFs, iMs, tMs, dMs, swpSize_, iStart);
				swp_->reverseV();
				iStart = iStart + swpSize_ - 1;
			}

		}
		return 0;
	}

	int sweepVDS_IDS::saveData(std::string fn, double vFs[], double iMs[], double tMs[], unsigned long dMs[], int sizeArray)
	{
		//Data directory
		std::string direc = "C:/Users/ECAN/Documents/KMData/Data/";
		//save as text file
		std::string frmt = ".csv";

		//Build file name for data and a second one to save test parameters
		std::string fp = direc + fn + frmt;
		std::string fpP = direc + fn + "_Param" + frmt;

		std::ofstream myfile;
		myfile.open(fp);
		myfile << "vForce [V], iMeas [A], time [ms], dTime [ms]\n";
		for (int i = 0; i < sizeArray; i++)
		{
			myfile << vFs[i] << ',' << iMs[i] << ',' << tMs[i] << ',' << dMs[i] << "\n";
		}
		myfile.close();

		//std::string fnP = "P_" + fn;
		std::ofstream myfile2;
		myfile2.open(fpP);
		myfile2 << "startV [V], stopV [V], SR [V/s], constV [V], lRange, range, comp, intTime, sweepSMU, constSMU, nCycles, fullCycle\n";
		myfile2 << sweepP_.startV << "," << sweepP_.stopV << ',' << sweepP_.SR << ',' << sweepP_.constV << ','
			<< sweepP_.lRange << ',' << sweepP_.range << ',' << sweepP_.comp << ','
			<< sweepP_.intTime << ',' << sweepP_.sweepSMU << ',' << sweepP_.constSMU << ',' 
			<< nCycles_ << ',' << fullCycle_ << "\n";
		myfile2.close();

		return 0;
	}

	sweepVDS_IDS::~sweepVDS_IDS() {
		delete swp_;
	}
	*/
	constVDS_IDS::constVDS_IDS(const constVDS_IDSParameters& entries) {

		//Set all private member variables to inputs
		constP_.dt = entries.dt;
		//constP_.constSMU1 = entries.constSMU1;
		//constP_.constSMU2 = entries.constSMU2;
		//constP_.measSMU = entries.measSMU;
		constP_.measTime = entries.measTime;
		constP_.constVA = entries.constVA;
		constP_.constVB = entries.constVB;
		constP_.lRange = entries.lRange;
		constP_.range = entries.range;
		constP_.comp = entries.comp;
		constP_.intTime = entries.intTime;

		//Initialize the const object
		cnst_ = new HP::hpConst(constP_);

		//Find the size of array needed to store values
		cnstSize_ = cnst_->arraySizeNeeded();
	}

	int constVDS_IDS::arraySizeNeeded()
	{
		return cnstSize_;
	}

	int constVDS_IDS::runProgram(double iMs[], double tMs[], unsigned long dMs[], int sizeArray)
	{
		int iStart = 0;
		cnst_->runTest(iMs, tMs, dMs, cnstSize_, iStart);
		return 0;
	}

	int constVDS_IDS::saveData(std::string fn, double iMs[], double tMs[], unsigned long dMs[], int sizeArray)
	{

		//Data directory
		std::string direc = "C:/Users/ECAN/Documents/KMData/Data/";
		//save as text file
		std::string frmt = ".csv";

		//Build file name for data and a second one to save test parameters
		std::string fp = direc + fn + frmt;
		std::string fpP = direc + fn + "_Param" + frmt;

		std::ofstream myfile;
		myfile.open(fp);
		myfile << "iMeas [A], time [ms], dTime [ms]\n";
		for (int i = 0; i < sizeArray; i++)
		{
			myfile << iMs[i] << ',' << tMs[i] << ',' << dMs[i] << "\n";
		}
		myfile.close();

		//std::string fnP = "P_" + fn;
		std::ofstream myfile2;
		myfile2.open(fpP);
		myfile2 << "dt [ms], measTime [s], constVA [V], constVB [V], lRange, range, comp, intTime\n";
		myfile2 << constP_.dt << "," << constP_.measTime << ',' << constP_.constVA << ',' << constP_.constVB << ','
			<< constP_.lRange << ',' << constP_.range << ',' << constP_.comp << ','
			<< constP_.intTime << "\n";
		myfile2.close();

		return 0;
	}

	constVDS_IDS::~constVDS_IDS() {
		delete cnst_;
	}

	pulseGas_constVDS_IDS::pulseGas_constVDS_IDS(const pulseGas_constVDS_IDSParameters& entries)
	{

		//Set all private member variables to inputs
		constP_.dtGas = entries.dtGas;
		constP_.dtHP = entries.dtHP;

		constP_.measTime = entries.initTime;
		constP_.constVA = entries.constVA;
		constP_.constVB = entries.constVB;
		constP_.lRange = entries.lRange;
		constP_.range = entries.range;
		constP_.comp = entries.comp;
		constP_.intTime = entries.intTime;

		constP_.flowRate = entries.flowRate;
		constP_.gasConc = 0;

		initTime_ = entries.initTime;
		stepTime_ = entries.stepTime;
		pulseTime_ = entries.pulseTime;
		endTime_ = entries.endTime;
		nPulses_ = entries.nPulses;

		gasConc_ = entries.gasConc;

		cnst_ = new HPGM::hpGmConst(constP_);

		initSizeGas_ = cnst_->arraySizeNeededGas();
		initSizeHP_ = cnst_->arraySizeNeededHP();

		cnst_->setMeasTime(stepTime_);
		stepSizeGas_ = cnst_->arraySizeNeededGas();
		stepSizeHP_ = cnst_->arraySizeNeededHP();

		cnst_->setMeasTime(pulseTime_);
		pulseSizeGas_ = cnst_->arraySizeNeededGas();
		pulseSizeHP_ = cnst_->arraySizeNeededHP();

		cnst_->setMeasTime(endTime_);
		endSizeGas_ = cnst_->arraySizeNeededGas();
		endSizeHP_ = cnst_->arraySizeNeededHP();

		cnst_->setMeasTime(initTime_);

		sizeArrayNeededGas_ = initSizeGas_ = (stepSizeGas_ + pulseSizeGas_ - 1) * nPulses_ + endSizeGas_ - 1;
		sizeArrayNeededHP_ = initSizeHP_ = (stepSizeHP_ + pulseSizeHP_ - 1) * nPulses_ + endSizeHP_ - 1;
	}

	int pulseGas_constVDS_IDS::arraySizeNeededGas()
	{
		return sizeArrayNeededGas_;
	}

	int pulseGas_constVDS_IDS::arraySizeNeededHP()
	{
		return sizeArrayNeededHP_;
	}
	int pulseGas_constVDS_IDS::runProgram(double fRMs[], double cMs[], int sizeArrayGas, double iMs[], double tMs[], unsigned long dMs[], int sizeArrayHP)
	{
		if (sizeArrayGas != sizeArrayNeededGas_) {
			std::cout << "Incorrect Gas Array size" << std::endl;
			return 1;
		}

		if (sizeArrayHP != sizeArrayNeededHP_) {
			std::cout << "Incorrect HP Array size" << std::endl;
			return 1;
		}

		int iStartGas = 0;
		int iStartHP = 0;
		cnst_->runTest(fRMs, cMs, initSizeGas_, iStartGas, iMs, tMs, dMs, initSizeHP_, iStartHP);
		iStartGas = iStartGas + initSizeGas_ - 1;
		iStartHP = iStartHP + initSizeHP_ - 1;

		for (int i = 0; i < nPulses_; i++) {
			cnst_->setMeasTime(pulseTime_);
			cnst_->setGasConc(gasConc_);
			cnst_->runTest(fRMs, cMs, pulseSizeGas_, iStartGas, iMs, tMs, dMs, pulseSizeHP_, iStartHP);
			cnst_->setMeasTime(stepTime_);
			cnst_->setGasConc(0);
			iStartGas = iStartGas + pulseSizeGas_ - 1;
			iStartHP = iStartHP + pulseSizeHP_ - 1;
			cnst_->runTest(fRMs, cMs, stepSizeGas_, iStartGas, iMs, tMs, dMs, stepSizeHP_, iStartHP);
			iStartGas = iStartGas + stepSizeGas_;
			iStartHP = iStartHP + stepSizeHP_;
		}

		cnst_->setMeasTime(endTime_);
		cnst_->runTest(fRMs, cMs, endSizeGas_, iStartGas, iMs, tMs, dMs, endSizeHP_, iStartHP);
		return 0;
	}
	int pulseGas_constVDS_IDS::saveData(std::string fn, double fRMs[], double cMs[], int sizeArrayGas, double iMs[], double tMs[], unsigned long dMs[], int sizeArrayHP)
	{

		//Data directory
		std::string direc = "C:/Users/ECAN/Documents/KMData/Data/";
		//save as text file
		std::string frmt = ".csv";

		//Build file name for data and a second one to save test parameters
		std::string fp = direc + fn + frmt;
		std::string fpG = direc + fn + "_Gas" + frmt;
		std::string fpP = direc + fn + "_Param" + frmt;

		std::ofstream myfile;
		myfile.open(fp);
		myfile << "iMeas [A], time [ms], dTime [ms]\n";
		for (int i = 0; i < sizeArrayHP; i++)
		{
			myfile << iMs[i] << ',' << tMs[i] << ',' << dMs[i] << "\n";
		}
		myfile.close();
		std::ofstream myfile3;
		myfile3.open(fpG);
		myfile3 << "flowRate [ccm], gasConc [ppm]\n";
		for (int i = 0; i < sizeArrayGas; i++) {
			myfile3 << fRMs[i] << ',' << cMs[i] << "\n";
		}
		//std::string fnP = "P_" + fn;
		std::ofstream myfile2;
		myfile2.open(fpP);
		myfile2 << "dtHP [ms],dtGas [ms], initTime [s], pulseTime [s], stepTime [s], endTime [s], nPulses, constVA [V], constVB [V], flowRate [ccm], gasConc [ppm], lRange, range, comp, intTime\n";
		myfile2 << constP_.dtHP << ','<<constP_.dtGas << "," << initTime_ << ',' << pulseTime_ << ',' << stepTime_ << ','
			<<endTime_ << ','<< nPulses_ << ',' << constP_.constVA << ',' << constP_.constVB << ','
			<<constP_.flowRate<< ',' <<constP_.gasConc<< ',' << constP_.lRange << ',' << constP_.range << ',' << constP_.comp << ','
			<< constP_.intTime << "\n";
		myfile2.close();

		return 0;
	}

	pulseGas_constVDS_IDS::~pulseGas_constVDS_IDS() {
		delete cnst_;
	}

	constGas_constVDS_IDS::constGas_constVDS_IDS(const constGas_constVDS_IDSParameters& entries)
	{
		//Set all private member variables to inputs
		constP_.dtGas = entries.dtGas;
		constP_.dtHP = entries.dtHP;

		constP_.measTime = entries.measTime;
		constP_.constVA = entries.constVA;
		constP_.constVB = entries.constVB;
		constP_.lRange = entries.lRange;
		constP_.range = entries.range;
		constP_.comp = entries.comp;
		constP_.intTime = entries.intTime;

		constP_.flowRate = entries.flowRate;
		constP_.gasConc = entries.gasConc;

		for (int i = 0; i < 7; i++) {
			chMeas_[i] = entries.chMeas[i];
			constP_.chMeas[i] = entries.chMeas[i];
		}
		nChannels_ = entries.nChannels;
		constP_.nChannels = entries.nChannels;
		cnst_ = new HPGM::hpGmConst(constP_);

		sizeArrayNeededGas_ = cnst_->arraySizeNeededGas();
		sizeArrayNeededHP_ = cnst_->arraySizeNeededHP();

		
	}

	int constGas_constVDS_IDS::arraySizeNeededGas()
	{
		return sizeArrayNeededGas_;
	}

	int constGas_constVDS_IDS::arraySizeNeededHP()
	{
		return sizeArrayNeededHP_;
	}

	int constGas_constVDS_IDS::setV(double vA, double vB) {
		cnst_->setConstVA(vA);
		cnst_->setConstVB(vB);
		return 0;
	}

	int constGas_constVDS_IDS::setMeasTime(double t) {
		cnst_->setMeasTime(t);
		sizeArrayNeededGas_ = cnst_->arraySizeNeededGas();
		sizeArrayNeededHP_ = cnst_->arraySizeNeededHP();
		return 0;
	}

	int constGas_constVDS_IDS::setGasConc(double gasConc) {
		cnst_->setGasConc(gasConc);
		return 0;
	}

	int constGas_constVDS_IDS::setFlowRate(double flowRate) {
		cnst_->setFlowRate(flowRate);
		return 0;
	}

	int constGas_constVDS_IDS::runProgram(double fRMs[], double cMs[], int sizeArrayGas, double iMs[], double tMs[], unsigned long dMs[], int sizeArrayHP)
	{
		if (sizeArrayGas != sizeArrayNeededGas_) {
			std::cout << "Incorrect Gas Array size" << std::endl;
			return 1;
		}

		if (sizeArrayHP != sizeArrayNeededHP_) {
			std::cout << "Incorrect HP Array size" << std::endl;
			return 1;
		}

		int iStartGas = 0;
		int iStartHP = 0;

		cnst_->runTest(fRMs, cMs, sizeArrayGas, iStartGas, iMs, tMs, dMs, sizeArrayHP, iStartHP);
		//cnst_->setGasConc(0);
		//cnst_->setFlowRate(200);
		Sleep(7000);
		return 0;
	}

	int constGas_constVDS_IDS::saveData(std::string fn, double fRMs[], double cMs[], int sizeArrayGas, double iMs[], double tMs[], unsigned long dMs[], int sizeArrayHP)
	{

		//Data directory
		std::string direc = "C:/Users/ECAN/Documents/KMData/Data/";
		//save as text file
		std::string frmt = ".csv";

		//Build file name for data and a second one to save test parameters
		std::string fp = direc + fn + frmt;
		std::string fpG = direc + fn + "_Gas" + frmt;
		std::string fpP = direc + fn + "_Param" + frmt;

		std::ofstream myfile;
		myfile.open(fp);
		for (int n = 0; n < nChannels_; n++) {
			myfile << "iMeas [A] - Dev " << chMeas_[n] << ", ";
		}
		myfile <<" time [ms], dTime [ms]\n";
		for (int i = 0; i < sizeArrayHP; i++)
		{
			for (int n = 0; n < nChannels_; n++) {
				myfile << iMs[i * nChannels_ + n] << ',';
			}
				
			myfile << tMs[i] << ',' << dMs[i] << "\n";
		}
		myfile.close();
		std::ofstream myfile3;
		myfile3.open(fpG);
		myfile3 << "flowRate [ccm], gasConc [ppm]\n";
		for (int i = 0; i < sizeArrayGas; i++) {
			myfile3 << fRMs[i] << ',' << cMs[i] << "\n";
		}
		//std::string fnP = "P_" + fn;
		std::ofstream myfile2;
		myfile2.open(fpP);
		myfile2 << "dtHP [ms],dtGas [ms], initTime [s], pulseTime [s], stepTime [s], endTime [s], nPulses, constVA [V], constVB [V], flowRate [ccm], gasConc [ppm], lRange, range, comp, intTime\n";
		myfile2 << constP_.dtHP << ',' << constP_.dtGas << "," << constP_.measTime 
			<< ',' << constP_.constVA << ',' << constP_.constVB << ','
			<< constP_.flowRate << ',' << constP_.gasConc << ',' << constP_.lRange << ',' << constP_.range << ',' << constP_.comp << ','
			<< constP_.intTime << "\n";
		myfile2.close();

		return 0;
	}

	constGas_constVDS_IDS::~constGas_constVDS_IDS() {
		delete cnst_;
	}
	/*
		stepVDS_IDS::stepVDS_IDS(const stepVDS_IDSParameters& entries) {

		//Set all private member variables to inputs
		stepP_.dt = entries.dt;
		stepP_.constSMU1 = entries.stepSMU;
		stepP_.constSMU2 = entries.constSMU;
		stepP_.measTime = entries.stepTime;
		stepP_.constV1 = entries.startV;
		stepP_.constV2 = entries.constV;
		stepP_.lRange = entries.lRange;
		stepP_.range = entries.range;
		stepP_.comp = entries.comp;
		stepP_.intTime = entries.intTime;

		startV_ = entries.startV;
		stopV_ = entries.stopV;
		stepV_ = entries.stepV;
		nCycles_ = entries.nCycles;
		fullCycle_ = entries.fullCycle;

		step_ = new HP::hpConst(stepP_);

		stepSize_ = step_->arraySizeNeeded();
		std::cout << "stepSize: " << stepSize_ << std::endl;
		//Calculate number of steps for single climb and total number of steps
		std::cout << "fC: " << int(fullCycle_) << std::endl;
		nSteps_ = (fabs(startV_ - stopV_) / (stepV_)) + 1.5;
		//std::cout<<(nSteps_ - 1)*((int(fullCycle_)+1)*nCycles_)<<std::endl;
		nStepsTot_ = (nSteps_ - 1) * ((int(fullCycle_) + 1) * nCycles_)
			+ (1 - int(fullCycle_)) * nCycles_ + fullCycle_;
		std::cout << "nStep: " << nSteps_ << std::endl;
		std::cout << "nSteptot: " << nStepsTot_ << std::endl;
		sizeArrayNeeded_ = nStepsTot_ * (stepSize_ - 1) + 1;
		if (startV_ > stopV_) stepV_ = abs(stepV_) * -1;
		std::cout << "StepV: " << stepV_ << std::endl;

	}

	int stepVDS_IDS::arraySizeNeeded() {
		return sizeArrayNeeded_;
	}

	int stepVDS_IDS::runProgram(double vFs[], double iMs[], double tMs[], unsigned long dMs[], int sizeArray)
	{
		if (sizeArray != sizeArrayNeeded_) {
			std::cout << "Incorrect Array size" << std::endl;
			return 1;
		}
		//Set initial indice
		int iStart = 0;
		int nS = nSteps_;
		double iV = startV_;
		//Execute outer for loop for number of cycles requested
		//Execute inner loop only if full cycle 
		for (int j = 0; j < nCycles_; j++)
		{
			//Run the sweep
			iStart = runFlight(vFs, iMs, tMs, dMs, iStart, nS, iV);
			nS = nSteps_ - fullCycle_;
			//Change the array indice
			std::cout << iStart << std::endl;
			if (iStart > sizeArrayNeeded_) {
				std::cout << "gone wild" << std::endl;
				return 5;
			}
			//iStart = iStart + (stepSize_ - 1)*(nSteps_-int(fullCycle_)) - 1;
			if (fullCycle_)
			{
				//Reverse the direction of the sweep
				reverseV();
				iV = startV_ + stepV_;
				iStart = runFlight(vFs, iMs, tMs, dMs, iStart, nS, iV);
				reverseV();
				//iStart = iStart + (stepSize_ - 1) * (nSteps_ -1) - 1;
			}
			iV = startV_ + stepV_ * (int(fullCycle_));

		}
		return 0;
	}

	int stepVDS_IDS::runFlight(double vFs[], double iMs[], double tMs[], unsigned long dMs[], int iStart, int nS, double v)
	{
		step_->setV1(v);
		for (int i = 0; i < nS; i++) {
			std::cout << iStart << std::endl;
			step_->runTest(vFs, iMs, tMs, dMs, stepSize_, iStart);
			v = v + stepV_;
			step_->setV1(v);
			iStart = iStart + stepSize_ - 1;
		}
		return iStart;
	}

	void stepVDS_IDS::reverseV()
	{
		double temp = startV_;
		startV_ = stopV_;
		stopV_ = temp;
		stepV_ = -1 * stepV_;
	}

	int stepVDS_IDS::saveData(std::string fn, double vFs[], double iMs[], double tMs[], unsigned long dMs[], int sizeArray)
	{

		//Data directory
		std::string direc = "C:/Users/ECAN/Documents/KMData/Data/";
		//save as text file
		std::string frmt = ".csv";

		//Build file name for data and a second one to save test parameters
		std::string fp = direc + fn + frmt;
		std::string fpP = direc + fn + "_Param" + frmt;

		std::ofstream myfile;
		myfile.open(fp);
		myfile << "vForce [V], iMeas [A], time [ms], dTime [ms]\n";
		for (int i = 0; i < sizeArray; i++)
		{
			myfile << vFs[i] << ',' << iMs[i] << ',' << tMs[i] << ',' << dMs[i] << "\n";
		}
		myfile.close();

		//std::string fnP = "P_" + fn;
		std::ofstream myfile2;
		myfile2.open(fpP);
		myfile2 << "stepVDS_IDS: Parameters\n";
		myfile2 << "dt [ms], stepTime [s], startV [V], stopV [V], stepV [V], "
			"constV [V], nCycles, fullCycle, lRange, range, comp, "
			"intTime, stepSMU, constSMU\n";
		myfile2 << stepP_.dt << "," << stepP_.measTime << ',' << startV_ << ',' << stopV_ << ',' << stepV_ << ','
			<< constV_ << ',' << nCycles_ << ',' << fullCycle_ << ','
			<< stepP_.lRange << ',' << stepP_.range << ',' << stepP_.comp << ','
			<< stepP_.intTime << ',' << stepP_.constSMU1 << ',' << stepP_.constSMU2 << "\n";
		myfile2.close();

		return 0;
	}
	stepVDS_IDS::~stepVDS_IDS() {
		delete step_;
	}
*/
}