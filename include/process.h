#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid_in, const std::string& user_in, const std::string& cmd_in);
  int Pid() const;
  std::string User() const;
  std::string Command() const;
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;

 private:
   int pid_ = 0;
   std::string user_;
   std::string cmd_;
   long up_time_;

   long prev_process_jiffies_so_far = 0;
   long prev_total_jiffies_so_far = 0;
   float cpu_utilization = 0.0;
};

#endif