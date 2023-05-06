// Date: Jan 28, 2023
// Description: ksamp.c

#include "../include/ksamp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

void getProcessorType()
{
	char str[256];
	FILE *file = fopen("/proc/cpuinfo", "r");
	while (fgets(str, 256, file))
	{
		if (strstr(str, "model name"))
		{
			char model[1024];
			sscanf(str, "model name : %[^\n]", model);
			printf("Processor Type: %s\n", model);
			break;
		}
	}

	fclose(file);
}

void getKernalVer()
{
	char str[255];
	FILE *file = fopen("/proc/version", "r");

	while (fgets(str, 256, file))
	{
		if (strstr(str, "Linux version"))
		{
			char ver[256];
			sscanf(str, "Linux version %20s", ver);
			printf("Kernel version: Linux version %s\n", ver);
			break;
		}
	}

	fclose(file);
}

// The amount of time since it was last booted
void getTimeSinceLastBoot()
{
	char str[256];
	FILE *file = fopen("/proc/uptime", "r");
	while (fgets(str, 256, file))
	{
		int uptime, idle_time;
		sscanf(str, "%d %d", &uptime, &idle_time);
		printf("Time since last boot: %d s\n", uptime);
		break;
	}

	fclose(file);
}

// The amount of time the processor has spend in user mode, system mode and idle time
void getTimeEachMode()
{
	char str[256];
	FILE *file = fopen("/proc/stat", "r");
	while (fgets(str, 256, file))
	{
		if (strstr(str, "cpu"))
		{
			int user, nice, system, idle, iowait, irq, softirq;
			sscanf(str, "cpu %d %d %d %d %d %d %d", &user, &nice, &system, &idle, &iowait, &irq, &softirq);
			printf("Processor time spent: user: %d cs, ", user);
			printf("system: %d cs, ", system);
			printf("idle: %d cs \n", idle);
			break;
		}
	}
	fclose(file);
}

void getTimeEachModeAvg(int rate, int period)
{
	char str[256];
	int userTotal = 0;
	int systemTotal = 0;
	int idleTotal = 0;
	int counter = 0;
	time_t start = time(NULL);

	while (1)
	{
		time_t curr = time(NULL);
		if (curr - start > period)
		{
			break;
		}

		FILE *file = fopen("/proc/stat", "r");
		while (fgets(str, 256, file))
		{
			if (strstr(str, "cpu"))
			{
				int user, nice, system, idle, iowait, irq, softirq;
				sscanf(str, "cpu %d %d %d %d %d %d %d", &user, &nice, &system, &idle, &iowait, &irq, &softirq);
				userTotal += user;
				systemTotal += system;
				idleTotal += idle;
				counter++;
				break;
			}
		}
		fclose(file);
		sleep(rate);
	}
	printf("Processor time Ave: user: %d cs, ", userTotal * counter);
	printf("System: %d cs, ", systemTotal / counter);
	printf("Idle: %d cs \n", idleTotal / counter);
}

// The number of disk read/write requests made on the system
void getNumOfDisk()
{
	char str[256];
	FILE *file = fopen("/proc/diskstats", "r");
	fgets(str, 256, file);
	int major, minor, read, readMerge, secRead, readTime, write, writeMerge, secWrite, writeTime, io, timeIO, weightedTimeIO;
	char device[10];
	sscanf(str, "%d %d %s %d %d %d %d %d %d %d %d %d %d %d", &major, &minor, device, &read, &readMerge, &secRead, &readTime, &write, &writeMerge, &secWrite, &writeTime, &io, &timeIO, &weightedTimeIO);
	printf("Disk read completed: %d\n", read);
	printf("Disk write completed: %d\n", write);
	fclose(file);
}

void getNumOfDiskAvg(int rate, int period)
{
	int readTotal = 0;
	int writeTotal = 0;
	char str[256];
	int counter = 0;
	time_t start = time(NULL);

	while (1)
	{
		time_t curr = time(NULL);
		if (curr - start > period)
		{
			break;
		}

		FILE *file = fopen("/proc/diskstats", "r");
		fgets(str, 256, file);
		int major, minor, read, readMerge, secRead, readTime, write, writeMerge, secWrite, writeTime, io, timeIO, weightedTimeIO;
		char device[10];
		sscanf(str, "%d %d %s %d %d %d %d %d %d %d %d %d %d %d", &major, &minor, device, &read, &readMerge, &secRead, &readTime, &write, &writeMerge, &secWrite, &writeTime, &io, &timeIO, &weightedTimeIO);
		printf("Disk read completed: %d\n", read);
		printf("Disk write completed: %d\n", write);
		readTotal += read;
		writeTotal += write;
		counter++;
		fclose(file);
		sleep(rate);
	}
	printf("Disk read completed Ave: %d\n", readTotal / counter);
	printf("Disk write completed Ave: %d\n", writeTotal / counter);
}

// The number of context switches the kernel has performed
void getNumOfSwitches()
{
	char str[256];
	FILE *file = fopen("/proc/stat", "r");
	while (fgets(str, 256, file))
	{
		if (strstr(str, "ctxt"))
		{
			int sw;
			sscanf(str, "ctxt %d", &sw);
			printf("context switches: %d\n", sw);
		}
	}
	fclose(file);
}

void getNumOfSwitchesAvg(int rate, int period)
{
	int switchTotal = 0;
	char str[256];
	int counter = 0;
	time_t start = time(NULL);

	while (1)
	{
		time_t curr = time(NULL);
		if (curr - start > period)
		{
			break;
		}
		FILE *file = fopen("/proc/stat", "r");
		while (fgets(str, 256, file))
		{
			if (strstr(str, "ctxt"))
			{
				int sw;
				sscanf(str, "ctxt %d", &sw);
				printf("context switches: %d\n", sw);
				switchTotal += sw;
				counter++;
			}
		}
		fclose(file);
		sleep(rate);
	}

	printf("Context switches ave: %d\n", switchTotal / counter);
}

// The number of processes that have been created since the last boot
void getNumOfProcess()
{
	char str[256];
	FILE *file = fopen("/proc/stat", "r");
	while (fgets(str, 256, file))
	{
		if (strstr(str, "processes"))
		{
			// char p[256];
			int p;
			sscanf(str, "processes %d", &p);
			printf("Processes created: %d\n", p);
		}
	}
	fclose(file);
}

void getNumOfProcessAvg(int rate, int period)
{
	char str[256];
	int processTotal = 0;
	int counter = 0;
	time_t start = time(NULL);

	while (1)
	{
		time_t curr = time(NULL);
		if (curr - start > period)
		{
			break;
		}

		FILE *file = fopen("/proc/stat", "r");
		while (fgets(str, 256, file))
		{
			if (strstr(str, "processes"))
			{
				int p;
				sscanf(str, "processes %d", &p);
				printf("Processes created: %d\n", p);
				processTotal += p;
				counter++;
			}
		}
		fclose(file);
		sleep(rate);
	}

	printf("Process created ave: %d\n", processTotal / counter);
}

// The amount of memory configured for this computer
void getMemory()
{
	char str[256];
	FILE *file = fopen("/proc/meminfo", "r");
	while (fgets(str, 256, file))
	{
		if (strstr(str, "MemTotal"))
		{
			int mem;
			sscanf(str, "MemTotal %d", &mem);
			printf("Total memory: %d kB\n", mem);
		}
	}
	fclose(file);
}

void getMemoryAvg(int rate, int period)
{

	char str[256];
	int memoryTotal = 0;
	int counter = 0;
	time_t start = time(NULL);

	while (1)
	{
		time_t curr = time(NULL);
		if (curr - start > period)
		{
			break;
		}
		FILE *file = fopen("/proc/meminfo", "r");
		while (fgets(str, 256, file))
		{
			if (strstr(str, "MemTotal"))
			{
				int mem;
				sscanf(str, "MemTotal %d", &mem);
				printf("Total memory: %d kB\n", mem);
				memoryTotal += mem;
				counter++;
			}
		}
		fclose(file);
		sleep(rate);
	}

	printf("total memory avg: %d", memoryTotal / counter);
}

// The amount of memory currently available
void getMemoryAvailable()
{

	FILE *fp = fopen("/proc/meminfo", "r");
	char str[256];

	while (fgets(str, 256, fp))
	{
		if (strstr(str, "MemAvailable"))
		{
			int memAvailable;
			sscanf(str, "MemAvailable: %d", &memAvailable);
			printf("Memory available: %d kB\n", memAvailable);
			break;
		}
	}
	fclose(fp);
}

void getMemoryAvailableAvg(int rate, int period)
{

	char str[256];
	int memTotal = 0;
	int counter = 0;
	time_t start = time(NULL);
	while (1)
	{
		time_t curr = time(NULL);
		if (curr - start > period)
		{
			break;
		}
		FILE *fp = fopen("/proc/meminfo", "r");
		while (fgets(str, 256, fp))
		{
			if (strstr(str, "MemAvailable"))
			{
				int memAvailable;
				sscanf(str, "MemAvailable: %d", &memAvailable);
				printf("Memory available: %d kB\n", memAvailable);
				memTotal += memAvailable;
				counter++;
				break;
			}
		}
		fclose(fp);
		sleep(rate);
	}

	printf("Memory Available avg: %d\n", memTotal / counter);
}

// Function code goes in this file
