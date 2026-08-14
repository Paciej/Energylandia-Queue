#pragma once

#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string.h>
#include <vector>
#include "Ride_Record.h"

class JsonParser {
private:
    CURL *curl;
    CURLcode res;
    nlohmann::json jsonData;

public:
    JsonParser() {
        curl_global_init(CURL_GLOBAL_ALL);
    }

    int httpGet(const char* url = "https://queue-times.com/parks/317/queue_times.json");
    void showJson();
    void printRides();
    void printRides(std::vector<RideRecord> records);
    void printRides(std::vector<RideAverage> records);
    std::vector<RideRecord> getRides();

    ~JsonParser() {
        curl_global_cleanup();
    }
};