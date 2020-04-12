#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <chrono>
#include <map>
#include <vector>

class Processor {
public:
  Processor();

  std::vector<float> Utilization();

private:
  std::vector<std::map<std::string, unsigned long>> prev_cpu_stats;
  std::chrono::time_point<std::chrono::system_clock> prev_time;
  std::vector<float> cpu_utilization;
};

#endif