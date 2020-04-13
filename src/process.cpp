#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include <linux_parser.h>

using std::string;
using std::to_string;
using std::vector;

Process::Process(unsigned int pid) {
  pid_ = pid;
  prev_cpu_stats_ = LinuxParser::CpuUtilization(pid);
}

unsigned int Process::Pid() {
  return pid_;
}

void Process::UpdateCpuUtilization() {
  auto Hertz = (float) sysconf(_SC_CLK_TCK);

  std::map<std::string, float> current_cpu_stats = LinuxParser::CpuUtilization(pid_);

  float prev_total_time = prev_cpu_stats_["utime"] + prev_cpu_stats_["stime"] +
                          prev_cpu_stats_["cutime"] + prev_cpu_stats_["cstime"];
  float current_total_time = current_cpu_stats["utime"] + current_cpu_stats["stime"] +
                             current_cpu_stats["cutime"] + current_cpu_stats["cstime"];

  float prev_seconds = prev_cpu_stats_["system_uptime"] - (prev_cpu_stats_["starttime"] / Hertz);
  float current_seconds = current_cpu_stats["system_uptime"] - (current_cpu_stats["starttime"] / Hertz);

  float diff_total = current_total_time - prev_total_time;
  float diff_seconds = current_seconds - prev_seconds;

  cpu_utilization_ = diff_total / Hertz / diff_seconds;
}

float Process::CpuUtilization() {
  return cpu_utilization_;
}

std::string Process::Command() {
  return LinuxParser::Command(pid_);
}

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// TODO: Return the user (name) that generated this process
string Process::User() { return string(); }

// TODO: Return the age of this process (in seconds)
unsigned long Process::UpTime() { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }