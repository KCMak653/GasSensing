// GSense.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <chrono>
#include <time.h>
#include<string>
#include <iostream>
#include "gmCmd.h"
#include "hpCmd.h"
#include "hpConst.h"
#include "hpSweep.h"
#include "hpProgram.h"
#include <thread>

int main()
{
    std::cout << "Hello World!\n";
    
    GM::gmCmd test1;
    //test1.setAnalyteGas(2);
    //test1.setFlowRate(2000);
    //double val;
    //Sleep(2000);
    //test1.measFlowRate(val, TRUE);
    test1.setCarrBase();
    //test1.measFlowRate(val, TRUE);
    
    
    constVDS_IDSParameters constP;
    constP.constV1 = -8;
    constP.constV2 = 0.02;
    constP.measTime = 64800;
    constP.dt = 10000;
    constP.lRange = 12;
    constP.range = 1;
    constP.intTime = 1;
    constP.comp = 1;
    constP.constSMU1 = 'A';
    constP.constSMU2 = 'B';
    
    HP::constVDS_IDS tst(constP);

    int arraySize = tst.arraySizeNeeded();
    double* vFs = new double[arraySize];
    double* iMs = new double[arraySize];
    double* tMs = new double[arraySize];
    unsigned long* dMs = new unsigned long[arraySize];

    tst.runProgram(vFs, iMs, tMs, dMs, arraySize);
    /*
    for (int i = 0; i < arraySize; i++) {
        std::cout << "F: :" << vFs[i] << std::endl;
        std::cout << "i: " << iMs[i] << std::endl;
        std::cout << "t: " << tMs[i] << std::endl;
        std::cout << "d: " << dMs[i] << std::endl;
    }
    */
    delete vFs;
    delete iMs;
    delete tMs;
    delete dMs;
    
    /*
    stepVDS_IDSParameters constP;
    constP.constV = 0.02;
    constP.startV = 0;
    constP.stopV = -2;
    constP.stepV = 1;
    constP.stepTime = 2;
    constP.dt = 1000;
    //constP.measTime = 20;
    //constP.dt = 5000;
    constP.lRange = 12;
    constP.range = 1;
    constP.intTime = 1;
    constP.comp = 1;
    constP.constSMU = 'B';
    constP.stepSMU = 'A';
    constP.nCycles = 1;
    constP.fullCycle = TRUE;

    HP::stepVDS_IDS tst(constP);

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
