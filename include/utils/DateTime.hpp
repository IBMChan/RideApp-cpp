#pragma once

#include <chrono>

class DateTime {
public:
    // Get current time in milliseconds since epoch
    static long long getCurrentTimeMillis();
};