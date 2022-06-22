#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <tuple>
#include <experimental/filesystem>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

namespace fs = std::experimental::filesystem;

// Given a key, get the corresponding value
string get_val(const string& filename, const string& key_str_target) {
  string line, key_str, val_str;
  std::ifstream stream(filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {    
      std::istringstream linestream(line);
      linestream >> key_str >> val_str;
      
      if (key_str == key_str_target) return val_str;
    }
    // throw std::runtime_error("Cannot find key_str : " + key_str_target + " in file : " + filename);
  } else {
    // cannot find key_str
    throw std::runtime_error("get_val: Cannot access file : " + filename);
  }
  return {};
}

// Given a key, get the corresponding value as well as unit as a string
std::tuple<string, string> get_val_with_unit(const string& filename, const string& key_str_target) {
  string line, key_str, val_str, unit_str;
  std::ifstream stream(filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {    
      std::istringstream linestream(line);
      linestream >> key_str >> val_str >> unit_str;
      
      if (key_str == key_str_target) return std::make_tuple(val_str, unit_str);
    }
    //throw std::runtime_error("Cannot find key_str : " + key_str_target + " in file : " + filename);
  }
  return std::make_tuple<string, string>("","");
}

// Get the n-th (starting from 1) field of the first line of the file
string get_field(const string& filename, const int& index) {
  string line, temp_str;
  std::ifstream stream(filename);
  if (stream.is_open()) {
    std::getline(stream, line); 
    std::istringstream linestream(line);
    for (int i = 0; i < index; i++) {
      linestream >> temp_str;
    }
    return temp_str;
  } else {
    // cannot find key_str
    throw std::runtime_error("get_field: Cannot access file : " + filename);
  }
  return {};
}

// An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  string filename = kProcDirectory + kVersionFilename;
  std::ifstream stream(filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
    return kernel;
  } else {
    throw std::runtime_error("Kernel: Cannot access file : " + filename);
  }
}

// Use std::filesystem to iterate directories
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  const fs::path proc{kProcDirectory};
  for (auto const& dir_entry : fs::directory_iterator{proc}) {
    auto dir_name = dir_entry.path().filename().string();
    if (std::all_of(dir_name.begin(), dir_name.end(), isdigit)) {
        int pid = stoi(dir_name);
        pids.push_back(pid);
    }
  }

  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{ 
  auto mem_total = std::stof(get_val(kProcDirectory + kMeminfoFilename, "MemTotal:"));
  auto mem_free = std::stof(get_val(kProcDirectory + kMeminfoFilename, "MemFree:"));

  return (mem_total-mem_free) / mem_total;
}

// Read and return the system uptime
long LinuxParser::UpTime() 
{ 
  string line;
  long up_time = 0;
  string filename = kProcDirectory + kUptimeFilename;
  std::ifstream stream(filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> up_time;
    return up_time;
  } else {
    throw std::runtime_error("UpTime: Cannot access file : " + filename);
  }
}

// Read and return the number of active jiffies for the system
// active_jiffies = user + nice + system + irq + softirq + steal
long LinuxParser::ActiveJiffies() 
{ 
  long user = 0, nice = 0, system = 0, idle = 0;
  long iowait = 0, irq = 0, softirq = 0, steal = 0;
  long guest = 0, guestnice = 0;
  
  string line;
  string dummy_str;
  string filename = kProcDirectory + kStatFilename;
  std::ifstream stream(filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> dummy_str >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guestnice;
    return user + nice + system + irq + softirq + steal;
  } else {
    throw std::runtime_error("ActiveJiffies: Cannot access file : " + filename);
  }
}

// Read and return the number of idle jiffies for the system
// idle_jiffies = idle + iowait
long LinuxParser::IdleJiffies() 
{ 
  long user = 0, nice = 0, system = 0, idle = 0;
  long iowait = 0, irq = 0, softirq = 0, steal = 0;
  long guest = 0, guestnice = 0;
  
  string line;
  string dummy_str;
  string filename = kProcDirectory + kStatFilename;
  std::ifstream stream(filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> dummy_str >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guestnice;
    return idle + iowait;
  } else {
    throw std::runtime_error("IdleJiffies: Cannot access file : " + filename);
  }
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{ 
  return std::stoi(get_val(kProcDirectory + kStatFilename, "processes"));
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 
  return std::stoi(get_val(kProcDirectory + kStatFilename, "procs_running"));
}

// Read and return the number of active jiffies for a PID
// Used to calculate Cpu utilization of each process
// Read /proc/[PID]/stat
// active_jiffies = user(14) + system(15) + user waited for children(16) + system waited for children(17)
long LinuxParser::ActiveJiffies(int pid) 
{ 
  string line, dummy_str;
  long user = 0, system = 0, user_waited_for_children = 0, system_waited_for_children = 0;
  string filename = kProcDirectory + std::to_string(pid) + kStatFilename;
  std::ifstream stream(filename);
  if (stream.is_open()) { 
    std::getline(stream, line);
    std::istringstream linestream(line);
    // skip first 13 fields
    for (int i = 0; i < 13; i++) {
       linestream >> dummy_str;
    }
    linestream >> user >> system >> user_waited_for_children >> system_waited_for_children;
    return user + system + user_waited_for_children + system_waited_for_children;
  } else {
    throw std::runtime_error("ActiveJiffies: Cannot access file : " + filename);
  }
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) 
{ 
  string line;
  string filename = kProcDirectory + std::to_string(pid) + kCmdlineFilename;
  std::ifstream stream(filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    return line; 
  } else {
    throw std::runtime_error("Command: Cannot access file : " + filename);
  }
}

// Read and return the memory used by a process (in units of MB)
string LinuxParser::Ram(int pid) 
{ 
  string filename = kProcDirectory + std::to_string(pid) + kStatusFilename;
  auto [val_str, unit_str] = get_val_with_unit(filename, "VmSize:");
  if (unit_str != "kB") throw std::runtime_error("wrong unit : " + unit_str);
  return std::to_string(std::stoi(val_str) / 1024);
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) 
{ 
  string filename = kProcDirectory + std::to_string(pid) + kStatusFilename;
  return get_val(filename, "Uid:");
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) 
{ 
  string line;
  string username, dummy_str, uid_str;
  string uid_str_target = LinuxParser::Uid(pid);
  string filename = kPasswordPath;
  std::ifstream filestream(filename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> dummy_str >> uid_str) {
        if (uid_str == uid_str_target) {
          std::replace(username.begin(), username.end(), '_', ' ');
          return username;
        }
      }
    }
  } else {
    throw std::runtime_error("User: Cannot access file : " + filename);
  }
  return username;
}

// Read and return the uptime of a process
// up_time_process = up_time_system - start_time_process
long LinuxParser::UpTime(int pid) 
{
  string filename = kProcDirectory + std::to_string(pid) + kStatFilename;
  // 22 --> starttime: the time the process started after system boot
  long start_time = std::stol(get_field(filename, 22));
  return LinuxParser::UpTime() - start_time / sysconf(_SC_CLK_TCK);
}
