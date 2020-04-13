#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <map>

class Process {
public:
  Process(unsigned int pid);
  unsigned int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  void UpdateStats();
  std::string Ram();
  float UpTime();

private:
  std::map<std::string, float> prev_cpu_stats_;
  float cpu_utilization_{};
  float uptime_{};
  unsigned int pid_;
};

#endif