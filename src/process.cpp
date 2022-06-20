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

Process::Process(int pid_in, const string& user_in, const string& cmd_in, long up_time_in)
: pid_(pid_in), user_(user_in), cmd_(cmd_in), up_time_(up_time_in)
{
}

// Return this process's ID
int Process::Pid() const
{ 
    return pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

// Return the command that generated this process
string Process::Command() const
{ 
    return cmd_;
}

// Return this process's memory utilization
string Process::Ram()
{ 
    return LinuxParser::Ram(this->pid_);
}

// Return the user (name) that generated this process
string Process::User() const
{ 
    return user_;
}

// Return the age of this process (in seconds)
long int Process::UpTime() const
{ 
    return up_time_;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const 
{ 
    // return this->Pid() < a.Pid();
    return true;
}