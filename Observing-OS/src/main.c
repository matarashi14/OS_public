//Date: Jan 28, 2023
//Description: kemp.c
//Parameters: {rate} {period}

#include "../include/ksamp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


void partOne(){
	getProcessorType();
	getKernalVer();
	getTimeSinceLastBoot();
	getTimeEachMode();
	getNumOfDisk();
	getNumOfSwitches();
	getNumOfProcess();
	getMemory();
	getMemoryAvailable();
}

int main(int argc, char **argv){

	

	if(argc == 3){
		int rate, period;
		time_t start = time(NULL);
		rate = atoi(argv[1]);
		period = atoi(argv[2]);
		getTimeEachModeAvg(rate, period);
		getNumOfDiskAvg(rate, period);
		getNumOfSwitchesAvg(rate, period);
		getNumOfProcessAvg(rate, period);
		getMemoryAvg(rate, period);
		getMemoryAvailableAvg(rate, period);


	}else if (argc == 1){
		partOne();
	}else{
		printf("invalid input\n");
	}


	return 0;
}
