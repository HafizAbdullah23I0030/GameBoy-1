#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "HangmanFigure.h"
#include "Category.h"

class Hangman {
private:
    sf::RenderWindow window;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Font font;
    HangmanFigure hangmanFigure;
    Category category;
    sf::SoundBuffer HangmanSoundBuffer;
    sf::Sound HangmanSound;

    char word[50];
    char guessedWord[50];
    sf::Clock timer;

    bool isGameFinished = false;    //check if the game is finished 

    //protoypes
    void initialize();
    void loadResources();
    void handleEvents();
    void update();
    void draw();

    void trackGuesses(char guess);
    bool isWordGuessed() const;
    bool isGameOver();

    void drawGameName();
    void drawGuessedWord();
    void drawHangmanFigure();
    void drawGameOverMessage();
    void drawWinMessage();
    void restartGame();


public:
    Hangman();
    void run(); // Main game loop
};
