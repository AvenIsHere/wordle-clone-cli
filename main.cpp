#include <format>
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using std::string;
using std::vector;
using std::endl;
using std::cout;
using std::format;
using std::cerr;
using std::map;
using std::cin;

using json = nlohmann::json;

constexpr int WORD_COUNT = 5;
constexpr int WORD_LEN = 5;

const string C_RESET  = "\033[0m";
const string C_GREEN  = "\033[32m";
const string C_YELLOW = "\033[33m";
const string C_RED    = "\033[31m";

enum Status {GREEN, YELLOW, GRAY};

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
    size_t returnSize = size * nmemb;

    static_cast<string *>(userp)->append(static_cast<char *>(buffer), returnSize);

    return returnSize;
}

vector<string> GetWords() {
    vector<string> words;
    string tempBuffer;

    CURL* handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, format("https://random-word-api.herokuapp.com/word?number={}&length={}", WORD_COUNT, WORD_LEN).c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &tempBuffer);
    CURLcode success = curl_easy_perform(handle);
    if (success == CURLE_OK) {
        try {
            json j = json::parse(tempBuffer);
            for (int i = 0; i < j.size(); i++) {
                words.push_back(j[i]);
            }
        } catch (json::parse_error &e) {
            cerr << "JSON parse error: " << e.what() << endl;
        }
    }
    curl_easy_cleanup(handle);
    return words;
}

int main() {
    curl_global_init(CURL_GLOBAL_ALL);

    for (vector<string> words = GetWords(); const auto& word : words) {
        bool guessed = false;
        vector<string> guesses;
        map<char, int> letterFreqPerm;
        for (char i : word) {
            letterFreqPerm[i]++;
        }
        do {
            map<char, int> letterFreq = letterFreqPerm;
            bool guessValid;
            if (guesses.size() == 6) {
                cout << "Out of luck! The word was: " << word << endl;
                break;
            }
            string guess;
            do {
                cout << "> ";
                cin >> guess;
                if (guess.size() != WORD_LEN) {
                    cout << format("Guess must be {} characters long.", WORD_LEN) << endl;
                    guessValid = false;
                }
                else if (std::ranges::find(guesses, guess) != guesses.end()) {
                    cout << "You have already guessed " << guess << "!" << endl;
                    guessValid = false;
                }
                else {
                    guessValid = true;
                    guesses.push_back(guess);
                }
            } while (!guessValid);
            if (guess == word) {
                cout << "Great job! The word was: " << word << endl;
                guessed = true;
            }
            else {
                Status guessStatus[WORD_LEN] = {GRAY, GRAY, GRAY, GRAY, GRAY};
                cout << "Letters in word: ";
                for (int j = 0; j < WORD_LEN; j++) {
                    if (guess[j] == word[j]) {
                        letterFreq.at(guess[j])--;
                        guessStatus[j] = GREEN;
                    }
                }
                for (int j = 0; j < WORD_LEN; j++) {
                    if (guessStatus[j] == GREEN) continue;
                    if (letterFreq.contains(guess[j]) && letterFreq[guess[j]] > 0) {
                        letterFreq[guess[j]]--;
                        guessStatus[j] = YELLOW;
                    }
                }
                for (int i = 0; i < WORD_LEN; i++) {
                    if (guessStatus[i] == GREEN) {
                        cout << C_GREEN << guess[i] << C_RESET;
                    } else if (guessStatus[i] == YELLOW) {
                        cout << C_YELLOW << guess[i] << C_RESET;
                    } else {
                        cout << guess[i];
                    }
                }
                cout << endl;
                guessed = false;
            }
        } while (!guessed);

    }

    curl_global_cleanup();

    return 0;
}