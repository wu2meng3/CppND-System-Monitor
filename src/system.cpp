#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <memory>
// filesystem library since C++17
#if __has_include(<filesystem>)
#  include <filesystem>
   namespace fs = std::filesystem;
#else
// for GNU libstdc++ prior to 9.1
// and LLVM libc++ prior to 9.0
// requires target_link_libraries(... stdc++fs) in 13:CMakeLists.txt
#  include <experimental/filesystem>
   namespace fs = std::experimental::filesystem;
#endif

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
using namespace LinuxParser;

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
    // Add processes to processes_ vector
    for (const auto& pid : pids) {
        Process proc(pid);
        // Skip existing processes
        if (std::find(processes_.begin(), processes_.end(), proc) != processes_.end())
            continue;

        processes_.push_back(proc);
    }

    // Trim processes without corresponding PID folder (/proc/PID)
    processes_.erase(std::remove_if(
                    processes_.begin(), 
                    processes_.end(),
                    [](Process& proc)
                    {
                        fs::path pid_folder_path{kProcDirectory+std::to_string(proc.Pid())};
                        return !fs::exists(pid_folder_path);
                    }));
    
    // Update cpu utilization
    for (auto& proc : processes_) {
        proc.ResetCpuUtilization();
    }

    // Reorder processes according to cpu utilization
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