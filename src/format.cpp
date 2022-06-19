#include <string>

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
    return std::to_string(hours_out) + ":" + std::to_string(minutes_out) + ":" + std::to_string(seconds_out);
}