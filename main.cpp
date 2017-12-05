#include <iostream>
#include "LinkedList.h"
#include "main.h"
#include <unistd.h>
#include <stdlib.h> /* atoi */
using namespace std;
#include <fstream>
#include <cstdio>
#include <string.h>

#define FCFS "FCFS"
#define SRTF "SRTF"
#define RR "RR"
void printStatInfo(Stats statInfo, int currentTime)
{
    printf("Average cpu usage : %.2f%%\n", 100 * (float)(currentTime - statInfo.totalIdleTime) / (float)currentTime);
    printf("Average waiting time : %.2f\n", (float)statInfo.totalWaitTime / (float)statInfo.totalJobs);
    printf("Average response time : %.2f\n", (float)statInfo.totalResponseTime / (float)statInfo.totalJobs);
    printf("Average turnaround time: %.2f\n", (float)statInfo.totalTurnAroundTime / (float)statInfo.totalJobs);
}

bool addNewJobsToPCBArray(List<Job> jobList, List<PCB> pcbList, int currentTime)
{
    bool itemsAdded = false;
    ListIterator<Job> jobIterator = jobList.first();
    while (!jobIterator.isPastEnd())
    {
        Job job = jobIterator.retrieve();
        if (job.arrivalTime == currentTime)
        {
            jobList.remove(job);
            PCB pcbItem;
            pcbItem.pid = job.pid;
            pcbItem.arrivalTime = job.arrivalTime;
            pcbItem.remainingTime = job.burstTime;
            pcbItem.burstTime = job.burstTime;
            pcbItem.timePutInQueue = currentTime;
            pcbList.insertToEnd(pcbItem);
            itemsAdded = true;
        }
        jobIterator.advance();
    }
    return itemsAdded;
}
PCB getMinRemainingTimePCB(List<PCB> pcbList)
{
    PCB minRemainingPCB = pcbList.first().retrieve();
    ListIterator<PCB> pcbIterator = pcbList.first();

    while (!pcbIterator.isPastEnd())
    {
        PCB pcb = pcbIterator.retrieve();
        if (pcb.remainingTime < minRemainingPCB.remainingTime)
            minRemainingPCB = pcb;
        pcbIterator.advance();
    }
    pcbList.remove(minRemainingPCB);
    return minRemainingPCB;
}

PCB getNextSRTFJob(PCB currentJob, List<PCB> pcbList, int currentTime)
{
    if (pcbList.isEmpty())
        return *(new PCB());
    if (currentJob.burstTime == 0)
    {
        return getMinRemainingTimePCB(pcbList);
    }
    PCB minRemainingPCB = currentJob;
    ListIterator<PCB> pcbIterator = pcbList.first();

    while (!pcbIterator.isPastEnd())
    {
        PCB pcb = pcbIterator.retrieve();
        if (pcb.arrivalTime == currentTime && (pcb.remainingTime < minRemainingPCB.remainingTime))
            minRemainingPCB = pcb;
        pcbIterator.advance();
    }
    pcbList.remove(minRemainingPCB);
    return minRemainingPCB;
}

void performSRTF(List<Job> jobList, Stats statInfo)
{
    PCB badPCB;
    badPCB.burstTime = 0;
    int currentTime = 0;
    List<PCB> pcbList;
    PCB currentJob;
    PCB newJob;
    bool newJobAdded = false;
    addNewJobsToPCBArray(jobList, pcbList, currentTime);

    while (!jobList.isEmpty() || !pcbList.isEmpty())
    {
        if (pcbList.isEmpty())
        {
            usleep(1 * 1000);
            currentTime++;
            statInfo.totalIdleTime++;
            continue;
        }
        if (newJobAdded)
        {
            newJobAdded = false;
            currentJob = newJob;
        }
        else
            currentJob = getNextSRTFJob(badPCB, pcbList, currentTime);
        statInfo.totalWaitTime += currentTime - currentJob.timePutInQueue;
        //first time being processed
        if (currentJob.burstTime == currentJob.remainingTime)
            statInfo.totalResponseTime += currentTime - currentJob.arrivalTime;
        while (currentJob.remainingTime-- != 0)
        {
            printf("<system time %d> process %d is running\n", currentTime, currentJob.pid);
            usleep(1 * 1000);
            currentTime++;
            if (addNewJobsToPCBArray(jobList, pcbList, currentTime))
            {
                newJob = getNextSRTFJob(currentJob, pcbList, currentTime);
                if (newJob.pid != currentJob.pid)
                {
                    newJobAdded = true;
                    break;
                }
            }
        }
        if (currentJob.remainingTime > 0)
        {
            currentJob.timePutInQueue = currentTime;
            pcbList.insertToEnd(currentJob);
        }
        else
        {
            printf("<system time %d> process %d is finished.......\n", currentTime, currentJob.pid);
            statInfo.totalTurnAroundTime += currentTime - currentJob.arrivalTime;
        }
    }
    printStatInfo(statInfo, currentTime);
}
void performFCFS(List<Job> jobList, Stats statInfo)
{
    int currentTime = 0;
    List<PCB> pcbList;
    while (!jobList.isEmpty() || !pcbList.isEmpty())
    {
        addNewJobsToPCBArray(jobList, pcbList, currentTime);
        if (pcbList.isEmpty())
        {
            usleep(1 * 1000);
            currentTime++;
            statInfo.totalIdleTime++;
            continue;
        }
        PCB currentJob = pcbList.removeFirst();
        statInfo.totalWaitTime += currentTime - currentJob.timePutInQueue;
        //first time being processed
        if (currentJob.burstTime == currentJob.remainingTime)
            statInfo.totalResponseTime += currentTime - currentJob.arrivalTime;

        while (currentJob.remainingTime-- != 0)
        {
            addNewJobsToPCBArray(jobList, pcbList, currentTime);
            printf("<system time %d> process %d is running\n", currentTime, currentJob.pid);
            usleep(1 * 1000);
            currentTime++;
        }

        statInfo.totalTurnAroundTime += currentTime - currentJob.arrivalTime;
        printf("<system time %d> process %d is finished.......\n", currentTime, currentJob.pid);
    }
    printf("<system time %d> All processes finished....................\n", currentTime);

    printStatInfo(statInfo, currentTime);
}

//fix with time quantum
void performRR(List<Job> jobList, Stats statInfo, int timeQuantum)
{
    int currentTime = 0;
    List<PCB> pcbList;
    int i = 0;
    while (!jobList.isEmpty() || !pcbList.isEmpty())
    {
        addNewJobsToPCBArray(jobList, pcbList, currentTime);
        if (pcbList.isEmpty())
        {
            for (i = 0; i < timeQuantum; i++)
            {
                usleep(1 * 1000);
                currentTime++;
                statInfo.totalIdleTime++;
                addNewJobsToPCBArray(jobList, pcbList, currentTime);
            }
            continue;
        }
        PCB currentJob = pcbList.removeFirst();
        statInfo.totalWaitTime += currentTime - currentJob.timePutInQueue;
        //first time being processed
        if (currentJob.burstTime == currentJob.remainingTime)
        {
            statInfo.totalResponseTime += currentTime - currentJob.arrivalTime;
        }
        for (i = 0; i < timeQuantum; i++)
        {
            printf("<system time %d> process %d is running\n", currentTime, currentJob.pid);
            usleep(1 * 1000);
            currentTime++;
            addNewJobsToPCBArray(jobList, pcbList, currentTime);
            if (--currentJob.remainingTime == 0)
            {
                statInfo.totalTurnAroundTime += currentTime - currentJob.arrivalTime;
                printf("<system time %d> process %d is finished.......\n", currentTime, currentJob.pid);
                break;
            }
        }

        if (currentJob.remainingTime > 0)
        {
            currentJob.timePutInQueue = currentTime;
            pcbList.insertToEnd(currentJob);
        }
    }
    printf("<system time %d> All processes finished....................\n", currentTime);

    printStatInfo(statInfo, currentTime);
}
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Incorrect number of arguments\n");
        return 1;
    }
    char *inputFile = argv[1];

    char selectedAlgorithm[100];
    int rrTimeQuantum = 1;
    if (strcmp(argv[2], FCFS) == 0)
    {
        strcpy(selectedAlgorithm, FCFS);
    }
    else if (strcmp(argv[2], SRTF) == 0)
    {
        strcpy(selectedAlgorithm, SRTF);
    }
    else if (strcmp(argv[2], RR) == 0)
    {
        strcpy(selectedAlgorithm, RR);
        if (argc != 4)
        {
            printf("Provide a time quantum for RR\n");
            return -1;
        }
        rrTimeQuantum = atoi(argv[3]);
    }
    else
    {
        printf("Please select a valid algorithm\n");
        return -1;
    }
    List<Job> jobList;
    Stats statInfo;
    strcpy(statInfo.algorithm, selectedAlgorithm);

    // open a file in read mode.
    ifstream infile;
    infile.open(inputFile);
    int tempPid, tempArrivalTime, tempBurstTime;
    while (infile >> tempPid >> tempArrivalTime >> tempBurstTime)
    {
        //printf("Job found: %d %d %d\n", tempPid, tempArrivalTime, tempBurstTime);
        Job job;
        job.pid = tempPid;
        job.arrivalTime = tempArrivalTime;
        job.burstTime = tempBurstTime;
        jobList.insertToEnd(job);
        statInfo.totalJobs++;
    }
    infile.close();

    printf("Selected Algorithm: %s\n", &statInfo.algorithm);
    printf("Total %d tasks are read from \"%s\". press 'enter' to start...\n",statInfo.totalJobs,inputFile); 
    printf("==================================================================\n");
    if (strcmp(selectedAlgorithm, FCFS) == 0)
    {
        performFCFS(jobList, statInfo);
    }
    else if (strcmp(selectedAlgorithm, RR) == 0)
    {
        performRR(jobList, statInfo, rrTimeQuantum);
    }
    else if (strcmp(selectedAlgorithm, SRTF) == 0)
    {
        performSRTF(jobList, statInfo);
    }
    return 0;
}

Stats::Stats()
{
    totalJobs = 0;
    totalIdleTime = 0;
    totalWaitTime = 0;
    totalResponseTime = 0;
    totalTurnAroundTime = 0;
}