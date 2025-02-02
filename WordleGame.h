#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "WordBank.h"
#pragma once
class WordleGame {
public:
    WordleGame();
    ~WordleGame();
    void handleEvents(sf::RenderWindow& window);
    void update();
    void render(sf::RenderWindow& window);

    bool isGameOver() const;
    bool isWordGuessed() const;
    bool checkGuess(const char* guess);
    int getstpix();
    int getgap();
    void run(sf::RenderWindow& window);

private:


    char* word;
    char** guesses;
    char** feedback;
    int currentAttempt;
    const int maxAttempts;
    WordBank wordBank;

    char currentGuess[6]; // Holds the current guess
    int guessIndex;       // Tracks the current index in the guess
    bool hintUsed_orNot;

    bool displayReplayPrompt;
    bool isReplayChoiceMade;
    char replayChoice;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Music backgroundMusic, EasterEggMusic;
    sf::Clock wordleClock, hintClock;

    const int gap = 50;
    const short stpix = 150;

    int currentStreak;
    int maxStreak;

    bool EasterEggMusicPlayed;

    void copyString(const char* source, char* destination);
    int stringLength(const char* str);
    void evaluateGuess(const char* guess, char result[6]);

    void drawBackground(sf::RenderWindow& window);
    void drawGameTitle(sf::RenderWindow& window, sf::Font& font);
    void drawGuesses(sf::RenderWindow& window, sf::Font& font);
    void drawGameOver(sf::RenderWindow& window, sf::Font& font);



    void drawHint(sf::RenderWindow& window, sf::Font& font);
    void reset_game();
    void drawScore(sf::RenderWindow& window, sf::Font& font);

    void ask_for_replay_prompt(sf::RenderWindow& window, sf::Font& font);
    int score;
    void saveScore();
    void loadStreak();
    void saveStreak();
    void show_word_onScreen(const char*);
};