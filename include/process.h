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
  ~Process() = default;

  int Pid() const;
  std::string User() const;
  std::string Command() const;
  float CpuUtilization();
  float GetCpuUtilization() const;
  std::string Ram();
  long int UpTime();
  bool operator < (Process const& a) const;
  bool operator == (Process const& a) const;
  bool operator != (Process const& a) const;

 private:
   int pid_ = 0;
   std::string user_;
   std::string cmd_;

   long prev_process_jiffies_so_far_ = 0;
   long prev_total_jiffies_so_far_ = 0;
   float cpu_utilization_ = 0.0;
};

#endif