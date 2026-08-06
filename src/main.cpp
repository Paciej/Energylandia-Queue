#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "Json_Parser.h"
#include "Rides_Database.h"

int main() {
//    CURL *curl;
//    CURLcode res;
//    curl_global_init(CURL_GLOBAL_ALL);

//    curl = curl_easy_init();
//    if (curl) {
//        curl_easy_setopt(curl, CURLOPT_URL, "https://queue-times.com/parks/317/queue_times.json");
//        res = curl_easy_perform(curl);

//        if (res != CURLE_OK) {
//            std::cout << "curl_easy_perform() returned " << curl_easy_strerror(res);
//        }

//        curl_easy_cleanup(curl);
//    } else {
//        return -1;
//    }

//    curl_global_cleanup();

    JsonParser Parser;
    std::cout << "httpGet response: " << Parser.httpGet() << std::endl;
    //Parser.printRides();
    RidesDatabase db;

    while (true) {

    }

    return 0;
}
