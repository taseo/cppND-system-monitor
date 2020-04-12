#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include <linux_parser.h>
#include <format.h>

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor &System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

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

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return 0; }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return 0; }

unsigned long System::UpTime() {
  return LinuxParser::UpTime();
}