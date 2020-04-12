#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(unsigned long seconds) {
  unsigned int hour = seconds / 3600;
  unsigned int minute = (seconds % 3600) / 60;
  unsigned int second = seconds % 60;

  std::string hour_str = std::to_string(hour);
  std::string minute_str = std::to_string(minute);
  std::string second_str = std::to_string(second);

  hour_str.insert(hour_str.begin(), 2 - hour_str.size(), '0');
  minute_str.insert(minute_str.begin(), 2 - minute_str.size(), '0');
  second_str.insert(second_str.begin(), 2 - second_str.size(), '0');

  return hour_str + ":" + minute_str + ":" + second_str;
}