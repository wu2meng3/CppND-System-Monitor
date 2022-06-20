#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid_in, const string& user_in, const string& cmd_in)
: pid_(pid_in), user_(user_in), cmd_(cmd_in)
{
}

// Process::Process(int pid_in, const string& user_in, const string& cmd_in, const string& ram_in, long int up_time_in)
// : pid_(pid_in), user_(user_in), cmd_(cmd_in), ram_(ram_in), up_time_(up_time_in)
// {
// }

// TODO: Return this process's ID
int Process::Pid()
{ 
    return pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

// TODO: Return the command that generated this process
string Process::Command()
{ 
    return cmd_;
}

// TODO: Return this process's memory utilization
string Process::Ram()
{ 
    return LinuxParser::Ram(this->pid_);
}

// TODO: Return the user (name) that generated this process
string Process::User()
{ 
    return user_;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime()
{ 
    //return up_time_;
    return 0;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const 
{ 
    //return this->CpuUtilization() < a.CpuUtilization();
    return true;
}