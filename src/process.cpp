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

Process::Process(int pid_in)
: pid_(pid_in), up_time_(LinuxParser::UpTime(pid_in)), user_(LinuxParser::User(pid_in)), cmd_(LinuxParser::Command(pid_in)), ram_(LinuxParser::Ram(pid_in))
{
}


// Return this process's ID
int Process::Pid() const
{ 
    return pid_;
}

// Return this process's CPU utilization
void Process::ResetCpuUtilization()
{
    long process_jiffies_so_far =  LinuxParser::ActiveJiffies(this->pid_);
    long total_jiffies_so_far = LinuxParser::Jiffies();
    long process_jiffies_now = process_jiffies_so_far - prev_process_jiffies_so_far_;
    long total_jiffies_now = total_jiffies_so_far - prev_total_jiffies_so_far_;
    if (total_jiffies_now <= 0) throw std::runtime_error("invalid total jiffies to calculate process's CPU utilization.");

    prev_process_jiffies_so_far_ = process_jiffies_so_far;
    prev_total_jiffies_so_far_ = total_jiffies_so_far;
    cpu_utilization_ = (float) process_jiffies_now / (float) total_jiffies_now;
}

// Return the command that generated this process
string Process::Command() const
{ 
    return cmd_;
}

// Return this process's memory utilization
string Process::Ram() const
{ 
    return ram_;
}

// Return the user (name) that generated this process
string Process::User() const
{
    return user_;
}

// Return the age of this process (in seconds)
long Process::UpTime() const
{ 
    return up_time_;
}

float Process::CpuUtilization() const
{
    return cpu_utilization_;
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator < (Process const& a) const 
{
    return cpu_utilization_ < a.CpuUtilization();
}

bool Process::operator == (Process const& a) const
{
    return pid_ == a.Pid();
}

bool Process::operator != (Process const& a) const
{
    return !operator==(a);
}