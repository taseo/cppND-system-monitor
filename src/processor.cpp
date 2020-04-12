#include <vector>
#include <chrono>

#include <linux_parser.h>
#include "processor.h"

Processor::Processor() {
  prev_cpu_stats = LinuxParser::CpuUtilization();

  for (unsigned int i = 0; i < prev_cpu_stats.size(); i++) {
    cpu_utilization.push_back(0.0);
  }

  prev_time = std::chrono::high_resolution_clock::now();
}

std::vector<float> Processor::Utilization() {
  auto current_time = std::chrono::high_resolution_clock::now();

  if (std::chrono::duration_cast<std::chrono::milliseconds>(current_time - prev_time).count() > 200) {
    std::vector<std::map<std::string, unsigned long int>> current_cpu_stats = LinuxParser::CpuUtilization();

    for (unsigned int i = 0; i < prev_cpu_stats.size(); i++) {
      unsigned long int prev_idle = prev_cpu_stats[i]["idle"] + prev_cpu_stats[i]["iowait"];
      unsigned long int current_idle = current_cpu_stats[i]["idle"] + current_cpu_stats[i]["iowait"];

      unsigned long prev_non_idle =
          prev_cpu_stats[i]["user"] + prev_cpu_stats[i]["nice"] + prev_cpu_stats[i]["system"] +
          prev_cpu_stats[i]["irq"] + prev_cpu_stats[i]["softirq"] + prev_cpu_stats[i]["steal"];
      unsigned long current_non_idle =
          current_cpu_stats[i]["user"] + current_cpu_stats[i]["nice"] + current_cpu_stats[i]["system"] +
          current_cpu_stats[i]["irq"] + current_cpu_stats[i]["softirq"] + current_cpu_stats[i]["steal"];

      unsigned long prev_total = prev_idle + prev_non_idle;
      unsigned long current_total = current_idle + current_non_idle;

      unsigned long diff_total = current_total - prev_total;
      unsigned long diff_idle = current_idle - prev_idle;

      cpu_utilization[i] = (float) (diff_total - diff_idle) / (float) diff_total;
    }

    prev_time = current_time;
    prev_cpu_stats = current_cpu_stats;
  }

  return cpu_utilization;
}