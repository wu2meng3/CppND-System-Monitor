#include <string>
#include <iomanip>
#include <sstream>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) 
{   
    int hours_out = seconds / 3600;
    int minutes_out = (seconds - hours_out * 3600) / 60;
    int seconds_out = seconds % 60;
    std::stringstream ss{};
    ss << std::setw(2) << std::setfill('0') << hours_out << ":"
       << std::setw(2) << std::setfill('0') << minutes_out << ":"
       << std::setw(2) << std::setfill('0') << seconds_out;
    return ss.str();
}