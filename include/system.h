#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
public:
  Processor &Cpu();
  std::vector<Process> &Processes();
  float MemoryUtilization();
  unsigned long UpTime();
  unsigned int TotalProcesses();
  unsigned int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();

private:
  static bool CompareProcesses(Process &a, Process &b) {
    return (a.CpuUtilization()) > (b.CpuUtilization());
  }

  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
  std::string operating_system_;
  std::string kernel_;
};

#endif