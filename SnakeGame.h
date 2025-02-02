
#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;
using namespace sf;

// Constants for difficulty levels
//Acha Zain mujhy nhi pata tha aesay comments hoty hain Hamza Tariq nay bataya inkay baray main :)
#define EASY 0
#define MEDIUM 1
#define HARD 2

class SnakeGame
{

private:
    // Window and assets
    sf::RenderWindow window;
    sf::Font font;

    int score;

    // Menu system
    bool inMenu;
    int selectedDifficulty;
    sf::Text menuOptions[3];
    int menuSelectionIndex;

    // Snake game members
    const int WIDTH = 30;
    const int HEIGHT = 40;
    const int TILE_SIZE = 20;

    int snakeLength;
    sf::Vector2i direction;
    sf::Clock clock;
    sf::Vector2i food;
    bool isGameOver;
    float speed;

    // Sound shishtum
    sf::SoundBuffer gameOverBuffer, EatingSoundBuffer, EasyLevelSoundBuffer, MediumLevelSoundBuffer, HardLevelSoundBuffer, MenuButtonSoundBuffer;
    sf::Sound gameOverSound, EatingSound, EasyLevelSound, MediumLevelSound, HardLevelSound, MenuButtonSound;

    sf::Sprite* snake;
    sf::Texture SnakeTexture, FoodTexture;
    sf::Sprite FoodSprite;


    int fruitsEatenForEasterEgg;  // Counter for fruits eaten since last activation
    bool easterEggActive;         // indication if Easter Egg is active
    float easterEggTimer;         // Timer for Easter Egg duration
    const float EASTER_EGG_DURATION = 5.0f; // Duration of acceleration (in seconds)
    const int FRUITS_THRESHOLD = 10;        // Fruits needed to activate Easter Egg
    float normalSpeed;            // To store normal speed when Easter Egg is not active



    // Game logic methods
    void processEvents();
    void handleInput();
    void update();
    void render();
    void spawnFood();
    bool checkCollision();

    // Menu methods
    void drawMenu();
    void handleMenuInput();
    void setDifficulty(int difficulty);

    void resetGame();
    //File handlinggg
    void saveScore();


    float menu_delay;

public:
    bool isPaused;
    SnakeGame();
    ~SnakeGame();
    void run(); //run mureed ahem 
    const short gap = 50;// used for gap in the gamename display
    const short stpix = 125;    //used for the starting pixel display

    int getScore()
    {
        return score;
    }
};
