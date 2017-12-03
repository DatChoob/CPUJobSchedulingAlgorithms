class Job
{
public:
  int pid;
  int arrivalTime;
  int burstTime;
  bool operator==(const Job job)
  {
    return pid == job.pid;
  }
  bool operator!=(const Job job)
  {
    return pid != job.pid;
  }
};

class PCB : public Job
{
public:
  int remainingTime;
  int timePutInQueue;
};

class Stats
{
public:
  char algorithm[100];
  int totalJobs;
  int totalIdleTime;
  int totalWaitTime;
  int totalResponseTime;
  int totalTurnAroundTime;
  Stats();
};