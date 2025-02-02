#include "GameBoyMenu.h"
#include "MyString.h"
#include <fstream>
#include <iostream>

GameBoyMenu::GameBoyMenu() : currentSelection(0), gameCount(0) {
    for (int i = 0; i < maxOptions - 2; ++i) {
        games[i] = NULL; // Initialize game pointers to nullptr
    }
    initialize();
}

GameBoyMenu::~GameBoyMenu() {
    for (int i = 0; i < maxOptions - 2; ++i) {
        if (games[i] != nullptr) {
            delete games[i]; //ye to hoga
        }
    }
}

void GameBoyMenu::initialize() {
    window.create(sf::VideoMode(600, 800), "GameBoy Menu");
    currentSelection = 0;

    if (!font.loadFromFile("Fonts/pixel-game/Pixel Game.otf")) {
        std::cout << "Failed to load font" << std::endl;
    }

    if (!backgroundTexture.loadFromFile("Backgrounds/MainBG/wallpaper.jpg")) {
        std::cout << "Failed to load background" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);

    if (!menuMusic.openFromFile("AudioResources/Main Menu Audio/OutRun - Magical Sound Shower.mp3")) {
        std::cout << "Failed to load menu music" << std::endl;
    }
    menuMusic.setLoop(true);
    menuMusic.play();

    for (int i = 0; i < maxOptions; ++i) {
        menuOptions[i].setFont(font);
        menuOptions[i].setCharacterSize(30);
        menuOptions[i].setPosition(200, 250 + i * 50);
        menuOptions[i].setFillColor(i == currentSelection ? sf::Color::Red : sf::Color::White);
    }

    menuOptions[maxOptions - 2].setString("High Scores");
    menuOptions[maxOptions - 1].setString("Quit");
}

void GameBoyMenu::addGame(Game* game, int index) {
    if (index < 0 || index >= maxOptions - 2) {
        std::cout << "Game index out of range" << std::endl;
        return;
    }
    games[index] = game;
    menuOptions[index].setString(game->getName());
    gameCount++;
}

void GameBoyMenu::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up) {
                menuOptions[currentSelection].setFillColor(sf::Color::White);
                currentSelection = (currentSelection - 1 + maxOptions) % maxOptions;
                menuOptions[currentSelection].setFillColor(sf::Color::Red);
            }
            else if (event.key.code == sf::Keyboard::Down) {
                menuOptions[currentSelection].setFillColor(sf::Color::White);
                currentSelection = (currentSelection + 1) % maxOptions;
                menuOptions[currentSelection].setFillColor(sf::Color::Red);
            }
            else if (event.key.code == sf::Keyboard::Enter) {
                menuMusic.pause();
                selectOption();
                if (window.isOpen()) {
                    menuMusic.play(); // Resume music only if the menu window is still open
                }
            }
        }
    }
}

void GameBoyMenu::update() {
    // tujay baad main dekhtay
}

void GameBoyMenu::render() {
    window.clear();
    window.draw(backgroundSprite);
    for (int i = 0; i < maxOptions; ++i) {
        if (games[i] != nullptr || i >= maxOptions - 2) { // Valid games or extra options (High Scores, Quit)
            window.draw(menuOptions[i]);
        }
    }
    window.display();
}

void GameBoyMenu::selectOption() {
    if (currentSelection == maxOptions - 1) { // Quit
        window.close();
        return;
    }

    if (currentSelection == maxOptions - 2) { // High Scores
        displayHighScores();
        return;
    }

    // Launch selected game
    window.close();
    if (games[currentSelection] != nullptr) {
        games[currentSelection]->run();
    }

    // Recreate the menu window after a game finishes
    window.create(sf::VideoMode(600, 800), "GameBoy Menu");
}

void GameBoyMenu::displayHighScores() {
    window.clear(sf::Color::Black);

    sf::Text highScoresText;
    highScoresText.setFont(font);
    highScoresText.setCharacterSize(20);
    highScoresText.setFillColor(sf::Color::White);
    highScoresText.setPosition(50, 50);

    // Read high scores from files
    char buffer[500];
    char snakeScores[500];
    char wordleScores[500];

    std::ifstream snakeFile("HighScores/snake_highscores.txt");
    std::ifstream wordleFile("HighScores/wordle_highscores.txt");

    if (snakeFile.is_open()) {
        snakeFile.getline(snakeScores, 500);
        snakeFile.close();
    }
    else {
        MyString::copy("No Snake Scores Available\n", snakeScores);
    }

    if (wordleFile.is_open()) {
        wordleFile.getline(wordleScores, 500);
        wordleFile.close();
    }
    else {
        MyString::copy("No Wordle Scores Available\n", wordleScores);
    }

    MyString::copy("High Scores\n\nSnake Game:\n", buffer);
    MyString::copy(snakeScores, buffer + MyString::length(buffer));
    MyString::copy("\n\nWordle Game:\n", buffer + MyString::length(buffer));
    MyString::copy(wordleScores, buffer + MyString::length(buffer));

    highScoresText.setString(buffer);

    window.draw(highScoresText);
    window.display();

    // Wait for the user to press a key
    while (true) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                return; 
            }
        }
    }
}

void GameBoyMenu::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}
