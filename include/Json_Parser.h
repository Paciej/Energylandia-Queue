#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string.h>

class JsonParser {
private:

CURL *curl;
CURLcode res;
const char* energyUrl = "https://queue-times.com/parks/317/queue_times.json";

size_t write_callback(void *data, size_t size, size_t nmemb, void* userp);

public:
JsonParser() {
    curl = curl_easy_init();
}

int httpGet();

};