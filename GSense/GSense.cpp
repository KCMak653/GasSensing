// GSense.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <chrono>
#include <time.h>
#include<string>
#include <iostream>
#include "gmCmd.h"
#include "hpCmd.h"
#include "hpConst.h"
#include "hpGmConst.h"
#include "hpSweep.h"
#include "hpProgram.h"
#include <thread>
#include <conio.h>

int main()
{
    std::cout << "Hello World!\n";
   /* 
   GM::gmCmd test1;
    
    
    test1.setFlowRate(2000);
    
    double val;
    Sleep(16000);
    //getch();
    test1.measFlowRate(val, TRUE);
    //test1.setCarrBase();
    
    /*
    test1.measFlowRate(val, TRUE);
    test1.setAnalyteGas(3);
    test1.setFlowRate(300);
    Sleep(2000);
    test1.measFlowRate(val, TRUE);
    test1.setAnalyteConc(50);
    for (int i = 0; i < 10; i++) {
        Sleep(2000);
        std::cout << "Conc: " << std::endl;
        test1.measAnalyteConc(val, TRUE);
        std::cout << "Flowrate: " << std::endl;
        test1.measFlowRate(val, TRUE);
    }
    test1.setAnalyteConc(0);
    test1.setFlowRate(200);
    Sleep(2000);
    */
    /*
    pulseGas_constVDS_IDSParameters constP;
    constP.constVA = -4;
    constP.constVB = .02;
    constP.measTime = 30*60;
    constP.dtGas = 10000;
    constP.dtHP = 1000;
    constP.lRange = 12;
    constP.range = 1;
    constP.intTime = 1;
    constP.comp = 1;

    constP.flowRate = 300;
    constP.gasConc = 300;

    HPGM::pulseGas_constVDS_IDS tst(constP);
    
    
    int arraySizeGas = tst.arraySizeNeededGas();
    int arraySizeHP = tst.arraySizeNeededHP();
    //double* vFs = new double[arraySize];
    double* iMs = new double[arraySizeHP];
    double* tMs = new double[arraySizeHP];
    unsigned long* dMs = new unsigned long[arraySizeHP];

    double* fRMs = new double[arraySizeGas];
    double* cMs = new double[arraySizeGas];

    tst.runProgram(fRMs, cMs, arraySizeGas, iMs, tMs, dMs, arraySizeHP);
    
    for (int i = 0; i < arraySizeHP; i++) {
        std::cout << "i: " << iMs[i] << std::endl;
        std::cout << "t: " << tMs[i] << std::endl;
        std::cout << "d: " << dMs[i] << std::endl;
    }

    for (int i = 0; i < arraySizeGas; i++) {
        std::cout << "FR: :" << fRMs[i] << std::endl;
        std::cout << "C: " << cMs[i] << std::endl;

    }
    
    std::string fn = "tst";
    tst.saveData(fn, fRMs, cMs, arraySizeGas, iMs, tMs, dMs, arraySizeHP);
    delete iMs;
    delete tMs;
    delete dMs;
    delete fRMs;
    delete cMs;

    */
    /*
    char TempChar[13] = "bnmkh";
    char SerialBuffer_[256];
    int bufferSize_ = 10;
    std::string strVal;
    double measVal;
    int i = 0;
    int j = 0;
    int nBytesRead = 13;
    do
    {
        if ((std::isdigit(TempChar[i])) || (TempChar[i] == '-') || (TempChar[i] == '+') || (TempChar[i] == '.'))
        {
            SerialBuffer_[j] = TempChar[i];
            std::cout << "SerialBuffer[i]: " << SerialBuffer_[j] << std::endl;
            j++;
        }
        i++;
        nBytesRead--;
    } while (nBytesRead > 0);
    /*
    for (unsigned int i = 0; i < bufferSize_; i++)
    {
        std::cout << "i: " << i << std::endl;
        std::cout << "SerialBuffer[i]: " << SerialBuffer_[i] << std::endl;
        strVal.push_back(SerialBuffer_[i]);
    }
    */
/*
    measVal = atof(SerialBuffer_);
    std::cout<<"strVal: "<< measVal <<std::endl;
    */
    //char* charArr;
    //charArr = &strVal[0];
    //sscanf_s(charArr, "%*[^0-9]%d%lf", &measVal, bufferSize_);
    //measVal = std::stod(strVal);
    
    int saveFreq = 5;
    int tMeas = 432000;
    constGas_constVDS_IDSParameters constP;
    constP.constVA = -8;
    constP.constVB = .02;
    constP.measTime = 86400;
    constP.dtGas = 5000;
    constP.dtHP = 1000;
    constP.lRange = 12;
    constP.range = 1;
    constP.intTime = 1;
    constP.comp = 1;

    constP.flowRate = 230;
    constP.gasConc = 0;

    HPGM::constGas_constVDS_IDS tst(constP);

    
    int arraySizeGas = tst.arraySizeNeededGas();
    int arraySizeHP = tst.arraySizeNeededHP();
    //double* vFs = new double[arraySize];
    double* iMs = new double[arraySizeHP];
    double* tMs = new double[arraySizeHP];
    unsigned long* dMs = new unsigned long[arraySizeHP];

    double* fRMs = new double[arraySizeGas];
    double* cMs = new double[arraySizeGas];
    std::string fn = "FRtest";
    //char a;
    //Pre -initial
    /*
    for (int i = 0; i < saveFreq; i++) {
        tst.runProgram(fRMs, cMs, arraySizeGas, iMs, tMs, dMs, arraySizeHP);
        fn = "DMMPquick_heatRecover_D";
        a = '0' + i;
        fn.push_back(a);
        tst.saveData(fn, fRMs, cMs, arraySizeGas, iMs, tMs, dMs, arraySizeHP);
    }
    */
    
    tst.runProgram(fRMs, cMs, arraySizeGas, iMs, tMs, dMs, arraySizeHP);
    //fn = "FRtest";
    tst.saveData(fn, fRMs, cMs, arraySizeGas, iMs, tMs, dMs, arraySizeHP);
    delete iMs;
    delete tMs;
    delete dMs;
    delete fRMs;
    delete cMs;
    
    
    double gasConc = 0;
    double flowRate = 300;
    char status = 'g';
    double mT1 = 40;
    double mT2 = 60;
    double mT3 = 20;
    
    int i = 0;
    char a;
    while (status == 'g') {
        std::cout << "gasConc: " << std::endl;
        std::cin >>gasConc;
        std::cout << "flowRate: " << std::endl;
        std::cin >> flowRate;
        std::cout << "mT1: " << std::endl;
        std::cin >> mT1;
        std::cout << "mT2: " << std::endl;
        std::cin >> mT2;
        std::cout << "mT3: " << std::endl;
        std::cin >> mT3;

        //Initial run
        tst.setMeasTime(mT1);
        arraySizeGas = tst.arraySizeNeededGas();
        arraySizeHP = tst.arraySizeNeededHP();
        //double* vFs = new double[arraySize];
        double* iMs = new double[arraySizeHP];
        double* tMs = new double[arraySizeHP];
        unsigned long* dMs = new unsigned long[arraySizeHP];

        double* fRMs = new double[arraySizeGas];
        double* cMs = new double[arraySizeGas];

        tst.runProgram(fRMs, cMs, arraySizeGas, iMs, tMs, dMs, arraySizeHP);
        a = '0' + i;
        fn = "FRtest_initial";
        fn.push_back(a);
        std::cout << fn << std::endl;
        tst.saveData(fn, fRMs, cMs, arraySizeGas, iMs, tMs, dMs, arraySizeHP);
        delete iMs;
        delete tMs;
        delete dMs;
        delete fRMs;
        delete cMs;
        

        //Pulse run
        tst.setFlowRate(flowRate);
        tst.setGasConc(gasConc);
        tst.setMeasTime(mT2);
        arraySizeGas = tst.arraySizeNeededGas();
        arraySizeHP = tst.arraySizeNeededHP();
        //double* vFs = new double[arraySize];
        double* iMs2 = new double[arraySizeHP];
        double* tMs2 = new double[arraySizeHP];
        unsigned long* dMs2 = new unsigned long[arraySizeHP];

        double* fRMs2 = new double[arraySizeGas];
        double* cMs2 = new double[arraySizeGas];
        //Pre -initial
        tst.runProgram(fRMs2, cMs2, arraySizeGas, iMs2, tMs2, dMs2, arraySizeHP);
        a = '0' + i;
        fn = "FRtest_pulse";
        fn.push_back(a);
        std::cout << fn << std::endl;
        tst.saveData(fn, fRMs2, cMs2, arraySizeGas, iMs2, tMs2, dMs2, arraySizeHP);
        delete iMs2;
        delete tMs2;
        delete dMs2;
        delete fRMs2;
        delete cMs2;

        //Settle
        tst.setFlowRate(300);
        tst.setGasConc(0);
        tst.setMeasTime(mT3);
        arraySizeGas = tst.arraySizeNeededGas();
        arraySizeHP = tst.arraySizeNeededHP();
        //double* vFs = new double[arraySize];
        double* iMs3 = new double[arraySizeHP];
        double* tMs3 = new double[arraySizeHP];
        unsigned long* dMs3 = new unsigned long[arraySizeHP];

        double* fRMs3 = new double[arraySizeGas];
        double* cMs3 = new double[arraySizeGas];
        //Pre -initial
        tst.runProgram(fRMs3, cMs3, arraySizeGas, iMs3, tMs3, dMs3, arraySizeHP);
        a = '0' + i;
        fn = "FRtest_settle";
        fn.push_back(a);
        std::cout << fn << std::endl;
        tst.saveData(fn, fRMs3, cMs3, arraySizeGas, iMs3, tMs3, dMs3, arraySizeHP);
        delete iMs3;
        delete tMs3;
        delete dMs3;
        delete fRMs3;
        delete cMs3;





        std::cout << "status: " << std::endl;
        std::cin >> status;
        

        i++;

    } 
    
    /*
    for (int i = 0; i < arraySizeHP; i++) {
        std::cout << "i: " << iMs[i] << std::endl;
        std::cout << "t: " << tMs[i] << std::endl;
        std::cout << "d: " << dMs[i] << std::endl;
    }

    for (int i = 0; i < arraySizeGas; i++) {
        std::cout << "FR: :" << fRMs[i] << std::endl;
        std::cout << "C: " << cMs[i] << std::endl;

    }
    
    std::string fn = "test";
    tst.saveData(fn, fRMs, cMs, arraySizeGas, iMs, tMs, dMs, arraySizeHP);
    delete iMs;
    delete tMs;
    delete dMs;
    delete fRMs;
    delete cMs;
    char c;

    std::cin >> c;
    */
    /*
    constVDS_IDSParameters constP;
    constP.constVA = .02;
    constP.constVB = .1;
    constP.measTime = 10;
    constP.dt = 1000;
    constP.lRange = 12;
    constP.range = 1;
    constP.intTime = 1;
    constP.comp = 1;
    //constP.constSMU1 = 'A';
    //constP.constSMU2 = 'B';
    
    HP::constVDS_IDS tst(constP);

    int arraySize = tst.arraySizeNeeded();
    //double* vFs = new double[arraySize];
    double* iMs = new double[arraySize];
    double* tMs = new double[arraySize];
    unsigned long* dMs = new unsigned long[arraySize];

    std::string fn = "constTest21";

    tst.runProgram(iMs, tMs, dMs, arraySize);
    tst.saveData(fn, iMs, tMs, dMs, arraySize);
    /*
    for (int i = 0; i < arraySize; i++) {
        std::cout << "F: :" << vFs[i] << std::endl;
        std::cout << "i: " << iMs[i] << std::endl;
        std::cout << "t: " << tMs[i] << std::endl;
        std::cout << "d: " << dMs[i] << std::endl;
    }
    

    delete iMs;
    delete tMs;
    delete dMs;
    
    /*
    sweepVDS_IDSParameters constP;
    constP.constV = 0.02;
    constP.startV = -2;
    constP.stopV = 4;
    constP.SR = 0.8;
    //constP.stepTime = 2;
    //constP.dt = 1000;
    //constP.measTime = 20;
    //constP.dt = 5000;
    constP.lRange = 12;
    constP.range = 1;
    constP.intTime = 1;
    constP.comp = 1;
    constP.constSMU = 'B';
    constP.sweepSMU = 'A';
    constP.nCycles = 1;
    constP.fullCycle = TRUE;

    HP::sweepVDS_IDS tst(constP);

    int arraySize = tst.arraySizeNeeded();
    std::cout << arraySize << std::endl;
    
    double* vFs = new double[arraySize];
    double* iMs = new double[arraySize];
    double* tMs = new double[arraySize];
    unsigned long* dMs = new unsigned long[arraySize];

    tst.runProgram(vFs, iMs, tMs, dMs, arraySize);
    for (int i = 0; i < arraySize; i++) {
        std::cout << "F: :" << vFs[i] << std::endl;
        std::cout << "i: " << iMs[i] << std::endl;
        std::cout << "t: " << tMs[i] << std::endl;
        std::cout << "d: " << dMs[i] << std::endl;
    }
    std::string fn = "test1";
    tst.saveData(fn, vFs, iMs, tMs, dMs, arraySize);
    delete vFs;
    delete iMs;
    delete tMs;
    delete dMs;
    
    /*
    HP::hpCmd test2;
    test2.setIntTime(1);
    test2.setRange(1);
    test2.setLRange(12);
    test2.srcZeroAll();

    
    /*
    double val = 0.000002545151351;
    std::string str = std::to_string(val);
    std::cout << str << std::endl;
    char cmd[] = "PA+00000.00000;";
    std::cout << strlen(cmd) << std::endl;
    std::cout << sizeof(cmd) << std::endl;
    int i;
    for (i = 0; i < strlen(str.c_str()); i++)
        cmd[i+2] = str[i];
    cmd[i + 2] = ';';
    for (i = i+1; i < (strlen(cmd)-2); i++)
        cmd[i+2] = ' ';

    std::cout <<i<< cmd << std::endl;
    */
    /*
    auto now = std::chrono::high_resolution_clock::now();
    //auto start2 = std::chrono::high_resolution_clock::now();
    
    //auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto now2 = std::chrono::high_resolution_clock::now();
    double dt = 50;
    double mt, dl;
    static unsigned long dely;
    for (int i = 0; i < 10; i++) {
        std::cout << i << std::endl;
        now2 = std::chrono::high_resolution_clock::now();
        mt = std::chrono::duration_cast<std::chrono::milliseconds> (now2 - now).count();
        std::cout << mt << std::endl;
        dely = dt - mt;
        std::this_thread::sleep_for(std::chrono::milliseconds(dely));
    }

    /*auto now2 = std::chrono::high_resolution_clock::now();
    //auto start2 = std::chrono::high_resolution_clock::now();

    auto now2_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now2);
    //auto value = now_ms.time_since_epoch();
    //long duration = value.count();
    //std::chrono::milliseconds dur(duration);
    //std::chrono::time_point<std::chrono::high_resolution_clock> dt(dur);
    double b = std::chrono::duration_cast<std::chrono::milliseconds>(now2_ms - now_ms).count();
    std::cout << b << std::endl;
    
    */

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
