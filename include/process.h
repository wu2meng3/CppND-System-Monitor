#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  //Process(int pid_in, const std::string& user_in, const std::string& cmd_in, const std::string& ram_, long int up_time_in);
  Process(int pid_in, const std::string& user_in);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
   int pid_ = 0;
   std::string user_ = "mwu";
//   std::string cmd_ = "cd";
//   std::string ram_ = "1 KB";
//   long int up_time_ = 1;
};

#endif