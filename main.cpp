#include <format>
#include "WordleDict.h"
#include "WordleGame.h"

int main() {

    for (const std::vector<std::string> words = WordleDict::GetWords(); const auto& word : words) {
        WordleGame game(word);
        game.play();
    }

    return 0;
}