#include "Json_Parser.h"

size_t JsonParser::write_callback(void *data, size_t size, size_t nmemb, void *userp) {

}

int JsonParser::httpGet() {

    if (!curl) {
        return -1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, this->energyUrl);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

}