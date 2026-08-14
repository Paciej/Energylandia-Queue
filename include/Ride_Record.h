#pragma once
#include <string>

struct RideRecord {
    std::string landName;
    std::string rideName;
    int waitTime;
    bool isOpen;
    std::string recordedAt;
};

struct RideAverage {
    std::string landName;
    std::string rideName;
    int avgWait;
};