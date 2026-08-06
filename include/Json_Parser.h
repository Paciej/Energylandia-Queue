#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string.h>

class JsonParser {
private:
    const char* url = "https://queue-times.com/parks/317/queue_times.json";

    CURL *curl;
    CURLcode res;
    nlohmann::json jsonData;

public:
    JsonParser() {
        curl_global_init(CURL_GLOBAL_ALL);
    }

    int httpGet();
    void showJson();
    void printRides();

    ~JsonParser() {
        curl_global_cleanup();
    }
};

static size_t write_callback(char *data, size_t size, size_t nmemb, void *userp);