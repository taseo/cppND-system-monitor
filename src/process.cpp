#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include <linux_parser.h>
#include <iomanip>

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

void Process::UpdateStats() {
  auto Hertz = (float) sysconf(_SC_CLK_TCK);

  std::map<std::string, float> current_cpu_stats = LinuxParser::CpuUtilization(pid_);

  float prev_total_time = prev_cpu_stats_["utime"] + prev_cpu_stats_["stime"] +
                          prev_cpu_stats_["cutime"] + prev_cpu_stats_["cstime"];
  float current_total_time = current_cpu_stats["utime"] + current_cpu_stats["stime"] +
                             current_cpu_stats["cutime"] + current_cpu_stats["cstime"];

  float prev_uptime = prev_cpu_stats_["system_uptime"] - (prev_cpu_stats_["starttime"] / Hertz);
  float current_uptime = current_cpu_stats["system_uptime"] - (current_cpu_stats["starttime"] / Hertz);

  float diff_total = current_total_time - prev_total_time;
  float diff_uptime = current_uptime - prev_uptime;

  cpu_utilization_ = diff_total / Hertz / diff_uptime;
  uptime_ = current_uptime;
}

float Process::CpuUtilization() {
  return cpu_utilization_;
}

std::string Process::Command() {
  return LinuxParser::Command(pid_);
}

std::string Process::Ram() {
  std::stringstream stream;
  stream << std::fixed << std::setprecision(1) << (LinuxParser::Ram(pid_) / 1024);

  return stream.str();
}

string Process::User() {
  return LinuxParser::User(LinuxParser::Uid(pid_));
}

float Process::UpTime() {
  return uptime_;
}
