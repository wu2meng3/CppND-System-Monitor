#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() 
{ 
    long idle_so_far = LinuxParser::IdleJiffies();
    long active_so_far = LinuxParser::ActiveJiffies();
    long idle_jiffies = idle_so_far - prev_idle_so_far;
    long active_jiffies = active_so_far - prev_active_so_far;
    long total_jiffies = idle_jiffies + active_jiffies;
    if (total_jiffies <= 0) throw std::runtime_error("Invalid total jiffies to calculate cpu utilization.");

    prev_idle_so_far = idle_so_far;
    prev_active_so_far = active_so_far;

    return (float) active_jiffies / (float) total_jiffies;
}