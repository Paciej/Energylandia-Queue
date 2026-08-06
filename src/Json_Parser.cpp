#include "Json_Parser.h"
#include <iostream>
#include <string>

static size_t write_callback(char *data, size_t size, size_t nmemb, void *userp) {

    std::string* buffer = static_cast<std::string*>(userp);
    buffer->append(static_cast<char*>(data), size * nmemb);

    return size * nmemb;
}

int JsonParser::httpGet() {

    if (!curl) {
        return -1;
    }

    std::string responseBuf;

    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, this->url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuf);
    this->res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        std::cout << "curl_easy_perform() returned " << curl_easy_strerror(res);
        return -1;
    }

    try {
        this->jsonData = nlohmann::json::parse(responseBuf);
    } catch (const nlohmann::json::parse_error& e) {
        std::cout << e.what() << std::endl;
    }

    curl_easy_cleanup(curl);
    return res;
}

void JsonParser::showJson() {
    for (auto it = jsonData.begin(); it != jsonData.end(); it++) {
        std::cout << it.key() << " : " << it.value() << std::endl;
    }
}

void JsonParser::printRides() {
    for (auto& land : jsonData["lands"]) {
        std::cout << "================= " << land["name"].get<std::string>() << " =================" << std::endl;

        for (auto& ride : land["rides"]) {
            std::cout << "ride: " << ride["name"].get<std::string>() << std::endl;
            std::cout << "Wait time: " << ride["wait_time"].get<unsigned int>() << std::endl << std::endl;
        }
    }
}

std::vector<RideRecord> JsonParser::getRides() {
    std::vector<RideRecord> records;    

    for (auto& land : jsonData["lands"]) {
        std::string landName = land["name"].get<std::string>();

        for (auto& ride : land["rides"]) {
            records.push_back(RideRecord {
                landName,
                ride["name"].get<std::string>(),
                ride["wait_time"].get<int>(),
                ride.value("is_open", false),
                ride["last_updated"].get<std::string>()
            });
        }
    }

    return records;
}