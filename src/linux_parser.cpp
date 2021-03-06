#include "linux_parser.h"
#include <dirent.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line, version;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string value;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  float memTotal, memAvailable;
  while (stream >> value) {
    if (value == "MemAvailable:") {
      stream >> memAvailable;
    }
    if (value == "MemTotal:") {
      stream >> memTotal;
    }
  }
  float result = (memAvailable / memTotal);
  return (result);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line, value;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return std::stol(value);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  string value = "", cpu = "", user = "", nice = "", system = "", idle = "",
         iowait = "", irq = "", softIrq = "", steal = "", guest = "",
         guestNice = "";
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, value);
    std::istringstream linestream(value);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
        softIrq >> steal >> guest >> guestNice;
  }
  return (std::stol(user) + std::stol(nice) + std::stol(system) +
          std::stol(irq) + std::stol(softIrq) + std::stol(steal) +
          std::stol(guest) + std::stol(guestNice) + std::stol(idle) +
          std::stol(iowait));
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string line = "", shlp = "";
  string sUtime = "", sStime = "", sCUtime = "", sCStime = "";
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 1; i < 14; i++) linestream >> shlp;
    linestream >> sUtime >> sStime >> sCUtime >> sCStime;
  }
  return (std::stol(sUtime) + std::stol(sStime) + std::stol(sCUtime) +
          std::stol(sCStime));
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  string value = "", cpu = "", user = "", nice = "", system = "", idle = "",
         iowait = "", irq = "", softIrq = "", steal = "", guest = "",
         guestNice = "";
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, value);
    std::istringstream linestream(value);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> softIrq >>
        irq >> steal >> guest >> guestNice;
  }
  return (std::stol(user) + std::stol(nice) + std::stol(system) +
          std::stol(irq) + std::stol(softIrq) + std::stol(steal) +
          std::stol(guest) + std::stol(guestNice));
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string value = "", cpu = "", user = "", nice = "", system = "", idle = "",
         iowait = "", irq = "", softIrq = "", steal = "", guest = "",
         guestNice = "";
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, value);
    std::istringstream linestream(value);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> softIrq >>
        irq >> steal >> guest >> guestNice;
  }
  return (std::stol(idle) + std::stol(iowait));
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  float totalProcesses;
  while (stream >> value) {
    if (value == "processes") {
      stream >> totalProcesses;
      return totalProcesses;
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  float totalProcesses;
  while (stream >> value) {
    if (value == "procs_running") {
      stream >> totalProcesses;
      return totalProcesses;
    }
  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    string result = line.erase(0, 5);
    return result;
  }
  return string();
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line, key = "", value = "";
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmData:") {
        int data = (100 * std::stof(value) / 1024 + 0.5);
        return to_string(data / 100) + "." +
               ((data % 100 < 10) ? "0" + to_string(data % 100)
                                  : to_string(data % 100));
      }
    }
  }
  return string();
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line, key = "", value = "";
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") return value;
    }
  }
  return string();
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line, searchTerm = "x:" + to_string(pid);
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      auto pos = line.find(searchTerm);
      if (pos != string::npos) return line.substr(0, pos - 1);
    }
  }
  return "root";
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 14; i++) linestream >> value;
    return stol(value);
  }
  return 0;
}