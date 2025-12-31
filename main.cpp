#include <format>
#include "WordleDict.h"
#include "consts.h"
#include "WordleGame.h"

int main() {

    for (std::vector<std::string> words = WordleDict::GetWords(WORD_COUNT, WORD_LEN); const auto& word : words) {
        WordleGame game(word);
        game.play();
    }

    return 0;
}