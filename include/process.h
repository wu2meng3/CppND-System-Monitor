#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid_in, const std::string& user_in, const std::string& cmd_in, long up_time_in);
  int Pid() const;
  std::string User() const;
  std::string Command() const;
  float CpuUtilization() const;
  std::string Ram();
  long int UpTime() const;
  bool operator<(Process const& a) const;

 private:
   int pid_ = 0;
   std::string user_;
   std::string cmd_;
   long up_time_;
};

#endif