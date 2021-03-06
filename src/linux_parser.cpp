#include <dirent.h>
#include <string>
#include <vector>

#include "linux_parser.h"

std::string LinuxParser::OperatingSystem() {
  std::string line;
  std::string key;
  std::string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

std::string LinuxParser::Kernel() {
  std::string kernel, line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream line_stream(line);

    for (int i = 0; i < 3; i++) {
      line_stream >> kernel;
    }
  }

  return kernel;
}

// TODO: update this to use std::filesystem
std::vector<unsigned int> LinuxParser::Pids() {
  std::vector<unsigned int> pids;
  DIR *directory = opendir(kProcDirectory.c_str());
  struct dirent *file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      std::string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  std::string key;
  std::string line;
  std::string mem_info;

  std::string::size_type string_size;

  float mem_total = 0.0;
  float mem_free = 0.0;
  float mem_usage = 0.0;

  std::ifstream file_stream(kProcDirectory + kMeminfoFilename);

  if (file_stream.is_open()) {
    while (std::getline(file_stream, line)) {
      std::istringstream line_stream(line);

      while (line_stream >> key >> mem_info) {
        if (key == "MemTotal:") {
          mem_total = stof(mem_info, &string_size);
        }

        if (key == "MemFree:") {
          mem_free = stof(mem_info, &string_size);
        }

        if (mem_total > 0 && mem_free > 0) {
          mem_usage = (float) (mem_total - mem_free) / (float) mem_total;
          return mem_usage;
        }
      }
    }
  }

  return mem_usage;
}

float LinuxParser::UpTime() {
  std::string line;
  std::ifstream file_stream(kProcDirectory + kUptimeFilename);

  float uptime = 0.0;

  if (file_stream.is_open()) {
    std::getline(file_stream, line);
    std::istringstream line_stream(line);

    line_stream >> uptime;
  }

  return uptime;
}

std::vector<std::map<std::string, unsigned long>> LinuxParser::CpuUtilization() {
  std::string line;
  std::vector<std::map<std::string, unsigned long>> result;

  std::string cpu;
  unsigned long user;
  unsigned long nice;
  unsigned long system;
  unsigned long idle;
  unsigned long iowait;
  unsigned long irq;
  unsigned long softirq;
  unsigned long steal;
  unsigned long guest;
  unsigned long guest_nice;

  std::ifstream file_stream(kProcDirectory + kStatFilename);

  if (file_stream.is_open()) {
    while (std::getline(file_stream, line)) {
      std::istringstream line_stream(line);

      line_stream >> cpu;

      if (cpu == "cpu") { continue; } // skip total CPU utilization

      if (cpu.substr(0, 3) != "cpu") { break; }

      line_stream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;

      std::map<std::string, unsigned long> cpu_stats;

      cpu_stats.insert(std::pair<std::string, unsigned long>("user", user));
      cpu_stats.insert(std::pair<std::string, unsigned long>("nice", nice));
      cpu_stats.insert(std::pair<std::string, unsigned long>("system", system));
      cpu_stats.insert(std::pair<std::string, unsigned long>("idle", idle));
      cpu_stats.insert(std::pair<std::string, unsigned long>("iowait", iowait));
      cpu_stats.insert(std::pair<std::string, unsigned long>("irq", irq));
      cpu_stats.insert(std::pair<std::string, unsigned long>("softirq", softirq));
      cpu_stats.insert(std::pair<std::string, unsigned long>("steal", steal));
      cpu_stats.insert(std::pair<std::string, unsigned long>("guest", guest));
      cpu_stats.insert(std::pair<std::string, unsigned long>("guest_nice", guest_nice));

      result.push_back(cpu_stats);
    }

    return result;
  }

  return result;
}

std::map<std::string, float> LinuxParser::CpuUtilization(unsigned int pid) {
  std::string line;
  std::string param;
  std::string::size_type string_size;
  std::map<std::string, float> result;

  unsigned int counter = 0;

  std::ifstream file_stream(kProcDirectory + std::to_string(pid) + kStatFilename);

  if (file_stream.is_open()) {
    std::getline(file_stream, line);
    std::istringstream line_stream(line);

    while (line_stream >> param) {
      switch (counter) {
        case 13:
          result.insert(std::pair<std::string, float>("utime", std::stof(param, &string_size)));
          break;
        case 14:
          result.insert(std::pair<std::string, float>("stime", std::stof(param, &string_size)));
          break;
        case 15:
          result.insert(std::pair<std::string, float>("cutime", std::stof(param, &string_size)));
          break;
        case 16:
          result.insert(std::pair<std::string, float>("cstime", std::stof(param, &string_size)));
          break;
        case 21:
          result.insert(std::pair<std::string, float>("starttime", std::stof(param, &string_size)));
          break;
      }

      counter++;

      if (counter > 21) {
        break;
      }
    }

    result.insert(std::pair<std::string, float>("system_uptime", LinuxParser::UpTime()));

    return result;
  }

  return result;
}

unsigned int LinuxParser::TotalProcesses() {
  std::string line;
  std::string key;
  unsigned int total_processes = 0;

  std::ifstream file_stream(kProcDirectory + kStatFilename);

  if (file_stream.is_open()) {
    while (std::getline(file_stream, line)) {
      std::istringstream line_stream(line);

      line_stream >> key;

      if (key != "processes") { continue; }

      line_stream >> total_processes;

      return total_processes;
    }
  }

  return total_processes;
}

unsigned int LinuxParser::RunningProcesses() {
  std::string line;
  std::string key;
  unsigned int running_processes = 0;

  std::ifstream file_stream(kProcDirectory + kStatFilename);

  if (file_stream.is_open()) {
    while (std::getline(file_stream, line)) {
      std::istringstream line_stream(line);

      line_stream >> key;

      if (key != "procs_running") { continue; }

      line_stream >> running_processes;

      return running_processes;
    }
  }

  return running_processes;
}

std::string LinuxParser::Command(unsigned int pid) {
  std::string line;
  std::ifstream file_stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);

  std::string command;

  if (file_stream.is_open()) {
    std::getline(file_stream, line);
    std::istringstream line_stream(line);

    line_stream >> command;
  }

  return command;
}

float LinuxParser::Ram(unsigned int pid) {
  std::string line;
  std::string key;
  float ram_usage = 0.0;

  std::ifstream file_stream(kProcDirectory + std::to_string(pid) + kStatusFilename);

  if (file_stream.is_open()) {
    while (std::getline(file_stream, line)) {
      std::istringstream line_stream(line);

      line_stream >> key;

      if (key != "RssAnon:") { continue; }

      line_stream >> ram_usage;

      return ram_usage;
    }
  }

  return ram_usage;
}

unsigned int LinuxParser::Uid(unsigned int pid) {
  std::string line;
  std::string key;
  unsigned int uid = 0;

  std::ifstream file_stream(kProcDirectory + std::to_string(pid) + kStatusFilename);

  if (file_stream.is_open()) {
    while (std::getline(file_stream, line)) {
      std::istringstream line_stream(line);

      line_stream >> key;

      if (key != "Uid:") { continue; }

      line_stream >> uid;

      return uid;
    }
  }

  return uid;
}

std::string LinuxParser::User(unsigned int uid) {
  std::string line;
  std::string x;
  std::string user = std::string();
  unsigned int current_uid;

  std::ifstream file_stream(kPasswordPath);

  if (file_stream.is_open()) {
    while (std::getline(file_stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream line_stream(line);

      line_stream >> user >> x >> current_uid;

      if (current_uid == uid) { return user; }
    }
  }

  return user;
}
