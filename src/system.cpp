#include <unistd.h>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include <linux_parser.h>

Processor &System::Cpu() { return cpu_; }

std::vector<Process> &System::Processes() {
  std::vector<unsigned int> pids = LinuxParser::Pids();
  std::vector<Process> processes;

  processes.reserve(pids.size());

  for (auto pid : pids) {
    processes.emplace_back(Process(pid));
  }

  usleep(200000);

  for (auto &process: processes) {
    process.UpdateStats();
  }

  std::sort(processes.begin(), processes.end());
  processes_ = processes;

  return processes_;
}

std::string System::Kernel() {
  if (kernel_.empty()) {
    kernel_ = LinuxParser::Kernel();
  }

  return kernel_;
}

float System::MemoryUtilization() {
  return LinuxParser::MemoryUtilization();
}

std::string System::OperatingSystem() {
  if (operating_system_.empty()) {
    operating_system_ = LinuxParser::OperatingSystem();
  }

  return operating_system_;
}

unsigned int System::RunningProcesses() {
  return LinuxParser::RunningProcesses();
}

unsigned int System::TotalProcesses() {
  return LinuxParser::TotalProcesses();
}

unsigned long System::UpTime() {
  return LinuxParser::UpTime();
}