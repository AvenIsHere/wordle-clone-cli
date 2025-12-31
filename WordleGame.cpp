//
// Created by aven on 31/12/2025.
//

#include "WordleGame.h"

#include <format>
#include <iostream>

bool WordleGame::IsValidGuess() {
    if (guess.size() != WORD_LEN) {
        std::cout << std::format("Guess must be {} characters long.", WORD_LEN) << std::endl;
        return false;
    }
    if (std::ranges::find(guesses, guess) != guesses.end()) {
        std::cout << "You have already guessed " << guess << "!" << std::endl;
        return false;
    }
    return true;
}

void WordleGame::PrintGuess(const Status guessStatus[]) const {
    for (int i = 0; i < WORD_LEN; i++) {
        if (guessStatus[i] == GREEN) {
            std::cout << C_GREEN << guess[i] << C_RESET;
        } else if (guessStatus[i] == YELLOW) {
            std::cout << C_YELLOW << guess[i] << C_RESET;
        } else {
            std::cout << guess[i];
        }
    }
    std::cout << std::endl;
}

std::string WordleGame::MakeGuess() {
    bool guessValid = false;
    do {
        std::cout << "> ";
        std::cin >> guess;
        guessValid = false;
        if (IsValidGuess()) {
            guessValid = true;
            guesses.push_back(guess);
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
            for (int j = 0; j < guesses.size()-1; j++) {
                PrintGuess(guessStatuses[j].data());
            }
        }
    } while (!guessValid);
    return guess;
}

void WordleGame::ProcessGuess() {
    std::map<char, int> letterFreq = letterFreqPerm;
    if (guess == word) {
        std::cout << "Great job! The word was: " << word << std::endl;
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
        PrintGuess(guessStatuses[guesses.size()-1].data());
        guessed = false;
    }
}

void WordleGame::play() {
    for (char i : word) {
        letterFreqPerm[i]++;
    }
    do {
        if (guesses.size() == GUESS_COUNT) {
            std::cout << "Out of luck! The word was: " << word << std::endl;
            break;
        }
        guess = MakeGuess();
        ProcessGuess();
    } while (!guessed);
}