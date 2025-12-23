#include <format>
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

const int WORD_COUNT = 50;

struct strArr {
    string words[WORD_COUNT];
};

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
    size_t returnSize = size * nmemb;

    ((string*)userp)->append((char*)buffer, returnSize);

    return returnSize;
}

strArr GetWords() {
    strArr words;
    string tempBuffer;

    CURL* handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, format("https://random-word-api.herokuapp.com/word?number={}&length=5", WORD_COUNT).c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &tempBuffer);
    CURLcode success = curl_easy_perform(handle);
    if (success == CURLE_OK) {
        try {
            json j = json::parse(tempBuffer);
            for (int i = 0; i < j.size(); i++) {
                words.words[i] = j[i];
            }
        } catch (json::parse_error &e) {
            cerr << "JSON parse error: " << e.what() << endl;
        }
    }
    curl_easy_cleanup(handle);
    return words;
}

int main() {
    curl_global_init(CURL_GLOBAL_SSL);

    strArr words = GetWords();

    for (int i = 0; i < WORD_COUNT; i++) {
        bool guessed;
        string word = words.words[i];
        do {
            string guess;
            cout << "> ";
            cin >> guess;
            if (guess == word) {
                cout << "Great job! The word was: " << word << endl;
                guessed = true;
            }
            else {
                cout << "Letters in word: ";
                for (int j = 0; j < guess.size(); j++) {
                    if (auto position = word.find(guess[j]); position != string::npos) {
                        cout << guess[j];
                        if (position == j) {
                            cout << "(G)";
                        }
                        cout << ", ";

                    }
                }
                cout << endl;
                guessed = false;
            }
        } while (!guessed);

    }

    return 0;
}