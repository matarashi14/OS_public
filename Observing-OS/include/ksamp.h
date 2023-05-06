//Author: Motoki Atarashi
//Date: Jan 28, 2023
//Description: ksamp.h


#ifndef KSAMP_H
#define KSAMP_H

//Define all global variables and function definitions here




void getProcessorType();
void getKernalVer();
void getTimeSinceLastBoot();
void getTimeEachMode(); // The amount of time the processor has spend in user mode, system mode and idle time
void getTimeEachModeAvg(int rate, int period);
void getNumOfDisk(); // The number of disk read/write requests made on the system
void getNumOfDiskAvg(int rate, int period);
void getNumOfSwitches();
void getNumOfSwitchesAvg(int rate, int period);
void getNumOfProcess();
void getNumOfProcessAvg(int rate, int period);

void getMemory(); // The amount of memory configured for this computer
void getMemoryAvg(int rate, int period);
void getMemoryAvailable();
void getMemoryAvailableAvg(int rate, int period);



#endif 
