#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) { pid_ = pid; }

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { return cpu_; }

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator>(const Process& a) const {
  return (CpuUtilization() > a.CpuUtilization()) ? true : false;
}

bool Process::operator<(const Process& a) const {
  return (CpuUtilization() < a.CpuUtilization()) ? true : false;
}

void Process::CpuUsage(long procTicks, long sysTicks) {
  long processDuration = procTicks - oldTicks_;
  long duration = sysTicks - oldSysTicks_;

  cpu_ = static_cast<float>(processDuration) / duration;

  oldTicks_ = procTicks;
  oldSysTicks_ = sysTicks;
}