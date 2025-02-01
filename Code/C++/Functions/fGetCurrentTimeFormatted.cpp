#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>

std::string fGetCurrentTimeFormatted() {
    using namespace std::chrono;

    // Get current time
    auto now = system_clock::now();
    auto time_t_now = system_clock::to_time_t(now);
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    // Convert to local time
    std::tm local_tm;
    localtime_r(&time_t_now, &local_tm); // Use localtime_s(&local_tm, &time_t_now) on Windows

    // Format the string
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << local_tm.tm_hour << "_"
        << std::setw(2) << local_tm.tm_min << "_"
        << std::setw(2) << local_tm.tm_sec << "_"
        << std::setw(3) << ms.count();

    return oss.str();
}