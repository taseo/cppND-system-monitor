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
  void UpdateStats();
  std::string Ram();                       // TODO: See src/process.cpp
  float UpTime();
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

 private:
  std::map<std::string, float> prev_cpu_stats_;
  float cpu_utilization_{};
  float uptime_{};
  unsigned int pid_;
};

#endif