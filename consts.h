//
// Created by aven on 31/12/2025.
//

#ifndef WORDLE_CONSTS_H
#define WORDLE_CONSTS_H
#include <string>

constexpr int WORD_COUNT = 5;
constexpr int WORD_LEN = 5;
constexpr int GUESS_COUNT = 6;

const std::string C_RESET  = "\033[0m";
const std::string C_GREEN  = "\033[32m";
const std::string C_YELLOW = "\033[33m";

enum Status {GREEN, YELLOW, GRAY};

#endif //WORDLE_CONSTS_H