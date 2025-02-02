#pragma once
#include "Game.h"
#include "Hangman.h"

class HangmanGameAdapter : public Game {
public:
    HangmanGameAdapter() = default;

    void run() override {
        Hangman hangman;
        hangman.run();
    }

    const char* getName() const override {
        return "Hangman";
    }
};
