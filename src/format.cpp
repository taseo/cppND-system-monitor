#include <string>
#include <iomanip>

#include "format.h"

std::string Format::ElapsedTime(unsigned long seconds) {
  unsigned int hour = seconds / 3600;
  unsigned int minute = (seconds % 3600) / 60;
  unsigned int second = seconds % 60;

  std::stringstream stream;

  stream << std::setfill('0') << std::setw(2) << hour << ":";
  stream << std::setfill('0') << std::setw(2) << minute << ":";
  stream << std::setfill('0') << std::setw(2) << second;

  return stream.str();
}