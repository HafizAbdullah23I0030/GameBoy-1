#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Game.h"

class GameBoyMenu {
private:
    static const int maxOptions = 5; // 3 games + High Scores + Quit
    int currentSelection;
    sf::RenderWindow window;
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Music menuMusic;

    Game* games[maxOptions - 2]; // Array of pointers to games (excludes High Scores & Quit)
    int gameCount;
    sf::Text menuOptions[maxOptions];

    void initialize();
    void handleEvents();
    void update();
    void render();
    void selectOption();
    void displayHighScores();

public:
    GameBoyMenu();
    ~GameBoyMenu();
    void addGame(Game* game, int index);
    void run();
};
