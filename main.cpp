#include <format>
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <cstdlib>

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
constexpr int GUESS_COUNT = 6;

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

bool IsValidGuess(const string& guess, vector<string> guesses) {
    if (guess.size() != WORD_LEN) {
        cout << format("Guess must be {} characters long.", WORD_LEN) << endl;
        return false;
    }
    if (std::ranges::find(guesses, guess) != guesses.end()) {
        cout << "You have already guessed " << guess << "!" << endl;
        return false;
    }
    return true;
}

void PrintGuess(const Status guessStatus[], const string& guess) {
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
        std::array<std::array<Status, WORD_LEN>, GUESS_COUNT> guessStatuses{};
        do {
            map<char, int> letterFreq = letterFreqPerm;
            bool guessValid;
            if (guesses.size() == GUESS_COUNT) {
                cout << "Out of luck! The word was: " << word << endl;
                break;
            }
            string guess;
            do {
                cout << "> ";
                cin >> guess;
                guessValid = false;
                if (IsValidGuess(guess, guesses)) {
                    guessValid = true;
                    guesses.push_back(guess);
                    #ifdef _WIN32
                        system("cls");
                    #else
                        system("clear");
                    #endif
                    for (int j = 0; j < guesses.size()-1; j++) {
                        PrintGuess(guessStatuses[j].data(), guesses[j]);
                    }
                }
            } while (!guessValid);
            if (guess == word) {
                cout << "Great job! The word was: " << word << endl;
                guessed = true;
            }
            else {
                guessStatuses[guesses.size()-1] = {GRAY, GRAY, GRAY, GRAY, GRAY};
                for (int j = 0; j < WORD_LEN; j++) {
                    if (guess[j] == word[j]) {
                        letterFreq.at(guess[j])--;
                        guessStatuses[guesses.size()-1][j] = GREEN;
                    }
                }
                for (int j = 0; j < WORD_LEN; j++) {
                    if (guessStatuses[guesses.size()-1][j] == GREEN) continue;
                    if (letterFreq.contains(guess[j]) && letterFreq[guess[j]] > 0) {
                        letterFreq[guess[j]]--;
                        guessStatuses[guesses.size()-1][j] = YELLOW;
                    }
                }
                PrintGuess(guessStatuses[guesses.size()-1].data(), guess);
                guessed = false;
            }
        } while (!guessed);

    }

    curl_global_cleanup();

    return 0;
}