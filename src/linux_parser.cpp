#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <tuple>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
    throw std::runtime_error("Cannot find key_str : " + key_str + " in file : " + filename);
  } else {
    // cannot find key_str
    throw std::runtime_error("Cannot access file : " + filename);
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
    throw std::runtime_error("Cannot find key_str : " + key_str + " in file : " + filename);
  }
  return {};
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
    throw std::runtime_error("Cannot access file : " + filename);
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
    throw std::runtime_error("Cannot access file : " + filename);
  }
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
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
    throw std::runtime_error("Cannot access file : " + filename);
  }
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() 
{ 
  string line;
  long jiffies = 0;
  string dummy_str;
  string filename = kProcDirectory + kStatFilename;
  std::ifstream stream(filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> dummy_str >> jiffies;
    return jiffies;
  } else {
    throw std::runtime_error("Cannot access file : " + filename);
  }
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

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

// Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{ 
  string line;
  string filename = kProcDirectory + std::to_string(pid) + kCmdlineFilename;
  std::ifstream stream(filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    return line; 
  } else {
    throw std::runtime_error("Cannot access file : " + filename);
  }
}

// Read and return the memory used by a process (in units of MB)
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{ 
  string filename = kProcDirectory + std::to_string(pid) + kStatusFilename;
  auto [val_str, unit_str] = get_val_with_unit(filename, "VmSize:");
  if (unit_str != "kB") throw std::runtime_error("wrong unit : " + unit_str);
  return std::to_string(std::stoi(val_str) / 1024);
}

// Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{ 
  string filename = kProcDirectory + std::to_string(pid) + kStatusFilename;
  return get_val(filename, "uid:");
}

// Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{ 
  string line;
  string user, dummy_str, pid_str;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> dummy_str >> pid_str) {
        if (pid_str == std::to_string(pid)) {
          std::replace(user.begin(), user.end(), '_', ' ');
          return user;
        }
      }
    }
  }
  return user;
}

// Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) 
{
  string filename = kProcDirectory + std::to_string(pid) + kStatFilename;
  long up_time = std::stol(get_field(filename, 22));
  return up_time / sysconf(_SC_CLK_TCK);
}
