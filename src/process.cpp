#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"
#include "unistd.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid_in)
: pid_(pid_in), user_(LinuxParser::User(pid_in)), cmd_(LinuxParser::Command(pid_in)), ram_(LinuxParser::Ram(pid_in))
{
}


// Return this process's ID
int Process::Pid() const
{ 
    return pid_;
}

// Return this process's CPU utilization
// according to https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
// cpu_utilization_ = (utime + stime + cutime + cstime) / (uptime_system - start_time)
//                  = (utime + stime + cutime + cstime) / (up_time_)
// up_time_ = uptime_system - start_time is the uptime for this process 
// Set cpu_utilization_ and up_time_
void Process::ResetCpuUtilization()
{
    long process_jiffies_so_far =  LinuxParser::ActiveJiffies(this->pid_);    
    long process_active_jiffies = process_jiffies_so_far - prev_process_jiffies_so_far_;
    float process_active_time = (float) process_active_jiffies / sysconf(_SC_CLK_TCK);
    float up_time_so_far = LinuxParser::UpTime(pid_);
    float wall_time = up_time_so_far - up_time_;
    
    up_time_ = up_time_so_far;
    prev_process_jiffies_so_far_ = process_jiffies_so_far;
 
    cpu_utilization_ = (float) process_active_time / (float) wall_time;
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