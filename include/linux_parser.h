#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
  // Paths
  const std::string kProcDirectory{"/proc/"};
  const std::string kCmdlineFilename{"/cmdline"};
  const std::string kStatusFilename{"/status"};
  const std::string kStatFilename{"/stat"};
  const std::string kUptimeFilename{"/uptime"};
  const std::string kMeminfoFilename{"/meminfo"};
  const std::string kVersionFilename{"/version"};
  const std::string kOSPath{"/etc/os-release"};
  const std::string kPasswordPath{"/etc/passwd"};

  // System
  float MemoryUtilization();
  float UpTime();
  std::vector<unsigned int> Pids();
  unsigned int TotalProcesses();
  unsigned int RunningProcesses();
  std::string OperatingSystem();
  std::string Kernel();

  std::vector<std::map<std::string, unsigned long>> CpuUtilization();
  std::map<std::string, float> CpuUtilization(unsigned int pid);

  // Processes
  std::string Command(unsigned int pid);
  float Ram(unsigned int pid);
  unsigned int Uid(unsigned int pid);
  std::string User(unsigned int uid);
};

#endif