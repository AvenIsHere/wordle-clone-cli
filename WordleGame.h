//
// Created by aven on 31/12/2025.
//

#ifndef WORDLE_WORDLEGAME_H
#define WORDLE_WORDLEGAME_H
#include <array>
#include <map>
#include <vector>

#include "consts.h"


class WordleGame {
    std::string word;
    std::vector<std::string> guesses;
    std::map<char, int> letterFreqPerm;
    bool guessed = false;
    std::string guess;
    std::array<std::array<Status, WORD_LEN>, GUESS_COUNT> guessStatuses{};

    bool IsValidGuess();
    void PrintGuess(const Status guessStatus[]) const;
    std::string MakeGuess();
    void ProcessGuess();
public:
    explicit WordleGame(const std::string &givenWord) {
        word = givenWord;
    }
    void play();
};


#endif //WORDLE_WORDLEGAME_H