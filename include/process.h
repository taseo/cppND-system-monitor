#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <map>

class Process {
 public:
  Process(unsigned int pid);
  unsigned int Pid();
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();
  void UpdateCpuUtilization();
  std::string Ram();                       // TODO: See src/process.cpp
  unsigned long UpTime();                  // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

 private:
  std::map<std::string, float> prev_cpu_stats_;
  float cpu_utilization_{};
  unsigned int pid_;
};

#endif