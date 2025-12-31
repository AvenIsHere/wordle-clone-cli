//
// Created by aven on 31/12/2025.
//

#include "WordleDict.h"

#include <format>
#include <iostream>
#include <ostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "consts.h"
using std::vector;
using json = nlohmann::json;

static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
    size_t returnSize = size * nmemb;

    static_cast<std::string *>(userp)->append(static_cast<char *>(buffer), returnSize);

    return returnSize;
}

std::vector<std::string> WordleDict::GetWords() {
    curl_global_init(CURL_GLOBAL_ALL);
    vector<std::string> words;
    std::string tempBuffer;

    CURL* handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, std::format("https://random-word-api.herokuapp.com/word?number={}&length={}", WORD_COUNT, WORD_LEN).c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &tempBuffer);
    CURLcode success = curl_easy_perform(handle);
    if (success == CURLE_OK) {
        try {
            json j = json::parse(tempBuffer);
            for (const auto & i : j) {
                words.push_back(i);
            }
        } catch (json::parse_error &e) {
            std::cerr << "JSON parse error: " << e.what() << std::endl;
        }
    }
    curl_easy_cleanup(handle);
    curl_global_cleanup();
    return words;
}