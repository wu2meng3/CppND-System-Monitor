#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <memory>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() 
{ 
    return cpu_;
}

// Return a container composed of the system's processes
// Refresh processes_ at each call
vector<Process>& System::Processes() 
{ 
    auto pids = LinuxParser::Pids();
    for (const auto& pid : pids) {
        Process proc(pid);
        // If the process already exists, skip it.
        if (std::find(processes_.begin(), processes_.end(), proc) != processes_.end()) {
            continue;
        }
        processes_.push_back(proc);
    }
    // Update cpu utilization
    for (auto& proc : processes_) {
        proc.ResetCpuUtilization();
    }
    ReorderProcesses();
    return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() 
{ 
    return LinuxParser::Kernel();
}

// Return the system's memory utilization
float System::MemoryUtilization() 
{
    return LinuxParser::MemoryUtilization();
}

// Return the operating system name
std::string System::OperatingSystem() 
{ 
    return LinuxParser::OperatingSystem(); 
}

// Return the number of processes actively running on the system
int System::RunningProcesses() 
{ 
    return LinuxParser::RunningProcesses();
}

// Return the total number of processes on the system
int System::TotalProcesses() 
{ 
    return LinuxParser::TotalProcesses();
}

// Return the number of seconds since the system started running
long int System::UpTime() 
{ 
    return LinuxParser::UpTime(); 
}

// Reorder processes with descending order regarding cpu utilization
void System::ReorderProcesses()
{
    std::sort(processes_.rbegin(), processes_.rend());
}