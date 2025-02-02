#pragma once
#include "Game.h"
#include "WordleGame.h"

class WordleGameAdapter : public Game {
public:
    WordleGameAdapter() = default;

    void run() override {
        WordleGame wordle;
        sf::RenderWindow window(sf::VideoMode(600, 800), "Wordle");
        wordle.run(window);
    }

    const char* getName() const override {
        return "Wordle";
    }
};
