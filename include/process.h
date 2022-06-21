#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid_in);
  ~Process() = default;

  int Pid() const;
  float CpuUtilization() const;
  std::string User() const;
  std::string Command() const;
  std::string Ram() const;
  long UpTime() const;
  bool operator < (Process const& a) const;
  bool operator == (Process const& a) const;
  bool operator != (Process const& a) const;

  void ResetCpuUtilization();

 private:
   int pid_ = 0;
   long up_time_ = 0;
   std::string user_;
   std::string cmd_;
   std::string ram_;

   long prev_process_jiffies_so_far_ = 0;
   long prev_total_jiffies_so_far_ = 0;
   float cpu_utilization_ = 0.0;
};

#endif