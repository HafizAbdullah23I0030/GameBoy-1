#include "SnakeGame.h"
#include "MyString.h"
#include <SFML/System/Clock.hpp>
#include <fstream>
#include <iostream>
using namespace std;
using namespace sf;

sf::Clock c;    //G H A N T A

//constructor 
SnakeGame::SnakeGame()
    : window(sf::VideoMode(600, 800), "Snake"),
    snakeLength(1),
    direction(1, 0),
    isGameOver(false),
    speed(0.2f),
    inMenu(true),
    selectedDifficulty(EASY),
    menuSelectionIndex(0),
    menu_delay(0.2f),
    score(0),
    isPaused(false)
{
    srand(time(NULL));  //rand shand 
    // Initialize snake
    SnakeTexture.loadFromFile("SnakeSprites/body.png");
    snake = new sf::Sprite[WIDTH * HEIGHT];
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        snake[i].setTexture(SnakeTexture);
        snake[i].setScale(TILE_SIZE / static_cast<float>(SnakeTexture.getSize().x), TILE_SIZE / static_cast<float>(SnakeTexture.getSize().y));
        snake[i].setPosition(-TILE_SIZE, -TILE_SIZE);
    }
    snake[0].setPosition(WIDTH / 2 * TILE_SIZE, HEIGHT / 2 * TILE_SIZE);

    FoodTexture.loadFromFile("SnakeSprites/FoodApple.png");
    FoodSprite.setTexture(FoodTexture);
    FoodSprite.setScale(TILE_SIZE / static_cast<float>(FoodTexture.getSize().x), TILE_SIZE / static_cast<float>(FoodTexture.getSize().y));

    font.loadFromFile("Fonts/Equilibrium/Equilibrium 400.ttf");

    const char* options[] = { "Easy", "Medium", "Hard" };
    for (int i = 0; i < 3; ++i) {
        menuOptions[i].setFont(font);
        menuOptions[i].setCharacterSize(40);
        menuOptions[i].setPosition(WIDTH * TILE_SIZE / 4, HEIGHT * TILE_SIZE / 3 + i * 60);
        menuOptions[i].setFillColor(sf::Color::White);
        char menuText[10];
        MyString::copy(options[i], menuText);
        menuOptions[i].setString(menuText);
    }
    menuOptions[menuSelectionIndex].setFillColor(sf::Color::Yellow);
    menuOptions[menuSelectionIndex].setScale(1.2f, 1.2f);

    // Load sounds
    gameOverBuffer.loadFromFile("AudioResources/GameOverSoundSamples/GameOverSound.mp3");
    gameOverSound.setBuffer(gameOverBuffer);

    EatingSoundBuffer.loadFromFile("AudioResources/SnakeAudio/snakeeating.mp3");
    EatingSound.setBuffer(EatingSoundBuffer);

    EasyLevelSoundBuffer.loadFromFile("AudioResources/SnakeAudio/EasyLevel.mp3");
    EasyLevelSound.setBuffer(EasyLevelSoundBuffer);

    MediumLevelSoundBuffer.loadFromFile("AudioResources/SnakeAudio/MediumLevel.mp3");
    MediumLevelSound.setBuffer(MediumLevelSoundBuffer);

    HardLevelSoundBuffer.loadFromFile("AudioResources/SnakeAudio/HardLevel.mp3");
    HardLevelSound.setBuffer(HardLevelSoundBuffer);

    MenuButtonSoundBuffer.loadFromFile("AudioResources/SnakeAudio/menubutton2.mp3");
    MenuButtonSound.setBuffer(MenuButtonSoundBuffer);

    spawnFood();
}


//tabahi machine
SnakeGame::~SnakeGame() {
    delete[] snake;
}

void SnakeGame::run()
{
    while (window.isOpen())
    {
        processEvents();

        if (inMenu)
        {
            drawMenu();
            continue;
        }

        if (!isGameOver)
        {
            update();
        }

        render();
    }
}

//process events
void SnakeGame::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
    if (inMenu) {
        handleMenuInput();
    }
    else {
        handleInput();
    }
}

//just the keyboard handling
void SnakeGame::handleInput()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && direction != sf::Vector2i(0, 1))
        direction = sf::Vector2i(0, -1);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && direction != sf::Vector2i(0, -1))
        direction = sf::Vector2i(0, 1);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && direction != sf::Vector2i(1, 0))
        direction = sf::Vector2i(-1, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && direction != sf::Vector2i(-1, 0))
        direction = sf::Vector2i(1, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
    {
        isPaused = !isPaused;  // Toggle pause state
    }
}

//menu inputs
void SnakeGame::handleMenuInput() {
    menu_delay -= clock.restart().asSeconds();
    if (menu_delay > 0) {
        return;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        MenuButtonSound.play();
        menuOptions[menuSelectionIndex].setFillColor(sf::Color::White);
        menuOptions[menuSelectionIndex].setScale(1.f, 1.f);
        menuSelectionIndex = (menuSelectionIndex + 2) % 3;
        menuOptions[menuSelectionIndex].setFillColor(sf::Color::Yellow);
        menuOptions[menuSelectionIndex].setScale(1.2f, 1.2f);
        menu_delay = 0.3f;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        MenuButtonSound.play();
        menuOptions[menuSelectionIndex].setFillColor(sf::Color::White);
        menuOptions[menuSelectionIndex].setScale(1.f, 1.f);
        menuSelectionIndex = (menuSelectionIndex + 1) % 3;
        menuOptions[menuSelectionIndex].setFillColor(sf::Color::Yellow);
        menuOptions[menuSelectionIndex].setScale(1.2f, 1.2f);
        menu_delay = 0.3f;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        setDifficulty(menuSelectionIndex);
        inMenu = false;
    }
}

//update the screen 
void SnakeGame::update() {
    if (isPaused) {
        return;  // Skip updating game logic if paused
    }

    static float elapsedTime = 0;
    static sf::Clock gameOverClock;  // Separate clock for game-over delay
    elapsedTime += clock.restart().asSeconds();

    if (elapsedTime > speed && !isGameOver) {
        elapsedTime = 0;

        for (int i = snakeLength - 1; i > 0; --i) {
            snake[i].setPosition(snake[i - 1].getPosition());
        }
        snake[0].move(direction.x * TILE_SIZE, direction.y * TILE_SIZE);
        if (checkCollision()) {
            isGameOver = true;
            gameOverSound.play();
            gameOverClock.restart();

            saveScore(); 
        }


        if (!isGameOver && snake[0].getPosition().x == food.x * TILE_SIZE &&
            snake[0].getPosition().y == food.y * TILE_SIZE) {
            EatingSound.play();
            if (snakeLength < WIDTH * HEIGHT) {
                snakeLength++;
            }
            score += 10;
            spawnFood();
        }
    }

    if (isGameOver)
    {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                resetGame();
            }
        return;
    }
}

void SnakeGame::resetGame() {
    score = 0;
    snakeLength = 1;
    direction = sf::Vector2i(1, 0);

    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        snake[i].setPosition(-TILE_SIZE, -TILE_SIZE);
    }
    snake[0].setPosition(WIDTH / 2 * TILE_SIZE, HEIGHT / 2 * TILE_SIZE);
    spawnFood();
    gameOverSound.stop();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        window.close();
    }
}


//render function
void SnakeGame::render() {
    window.clear(sf::Color::Black);

    // Draw boundaries
    sf::RectangleShape boundary(sf::Vector2f(window.getSize().x, 20));
    boundary.setFillColor(sf::Color::White);
    boundary.setPosition(0, 0);
    window.draw(boundary);

    boundary.setPosition(0, window.getSize().y - 20);
    window.draw(boundary);

    boundary.setSize(sf::Vector2f(20, window.getSize().y));
    boundary.setPosition(0, 0);
    window.draw(boundary);

    boundary.setPosition(window.getSize().x - 20, 0);
    window.draw(boundary);

    if (inMenu)
    {
        resetGame();
        drawMenu();
    }
    else {
        for (int i = 0; i < snakeLength; ++i)
        {
            window.draw(snake[i]);
        }
        window.draw(FoodSprite);
        sf::Text scoreText;
        //setting the position of score text
        scoreText.setFont(font);
        scoreText.setString("Score: " + std::to_string(score)); //tostring conversoin
        scoreText.setCharacterSize(20);
        scoreText.setFillColor(sf::Color::Magenta);
        scoreText.setPosition(WIDTH * TILE_SIZE / 2 - 50, 0);
        window.draw(scoreText);

        if (isGameOver) {
            sf::Text gameOverText("GAME OVER!", font, 50);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(WIDTH * TILE_SIZE / 4, HEIGHT * TILE_SIZE / 2);
            window.draw(gameOverText);

            sf::Text finalScoreText("Your Score: " + std::to_string(score), font, 40);
            finalScoreText.setFillColor(sf::Color::Yellow);
            finalScoreText.setPosition(WIDTH * TILE_SIZE / 3.5f, HEIGHT * TILE_SIZE / 1.5f);
            window.draw(finalScoreText);

            sf::Text exitText("Press ESC to return to menu", font, 30);
            exitText.setFillColor(sf::Color::White);
            exitText.setPosition(WIDTH * TILE_SIZE / 4.0f, HEIGHT * TILE_SIZE / 1.0f);
            window.draw(exitText);
            update();
        }

        // Display "PAUSED" text if the game is paused
        if (isPaused) {
            sf::Text pauseText("PAUSED", font, 50);
            pauseText.setFillColor(sf::Color::Red);
            pauseText.setPosition(WIDTH * TILE_SIZE / 3, HEIGHT * TILE_SIZE / 2);
            window.draw(pauseText);
        }
    }

    window.display();
}


void SnakeGame::spawnFood()
{
    bool validPosition;
    do {
        validPosition = true;
        //Boundary checking for food (should be spawned inside the boundary)
        food.x = rand() % (WIDTH - 2) + 1;
        food.y = rand() % (HEIGHT - 2) + 1;
        //
        for (int i = 0; i < snakeLength; ++i) {
            if (snake[i].getPosition() == sf::Vector2f(food.x * TILE_SIZE, food.y * TILE_SIZE)) {
                validPosition = false;
                break;
            }
        }
    } while (!validPosition);

    FoodSprite.setPosition(food.x * TILE_SIZE, food.y * TILE_SIZE);
}


//collision chekcing 
bool SnakeGame::checkCollision()
{
    // Check if the snake collides with itself
    for (int i = 1; i < snakeLength; ++i) {
        if (snake[0].getPosition() == snake[i].getPosition())
        {
            return true;
        }
    }

    // Check if the snake collides with boundaries
    sf::Vector2f headPos = snake[0].getPosition();
    if (headPos.x < TILE_SIZE || headPos.y < TILE_SIZE ||
        headPos.x >= (WIDTH - 1) * TILE_SIZE || headPos.y >= (HEIGHT - 1) * TILE_SIZE) {
        return true;
    }

    return false;
}

//menu draw func
void SnakeGame::drawMenu() {
    window.clear(sf::Color::Black);

    sf::Text titleText("Snake", font, 50);
    titleText.setFillColor(sf::Color::Green);
    titleText.setPosition(WIDTH * TILE_SIZE / 4, HEIGHT * TILE_SIZE / 6);
    window.draw(titleText);

    for (int i = 0; i < 3; ++i) {
        window.draw(menuOptions[i]);
    }

    sf::Text instructionsText("Press ENTER to start!", font, 30);
    instructionsText.setFillColor(sf::Color::White);
    instructionsText.setPosition(WIDTH * TILE_SIZE / 4, HEIGHT * TILE_SIZE / 1.2f);
    window.draw(instructionsText);

    window.display();
}

//difficulty selection
void SnakeGame::setDifficulty(int difficulty) {
    switch (difficulty) {
    case 0: // Easy
        selectedDifficulty = EASY;
        speed = 0.2f;
        EasyLevelSound.play();
        break;
    case 1: // Medium
        selectedDifficulty = MEDIUM;
        speed = 0.1f;
        MediumLevelSound.play();
        break;
    case 2: 
        selectedDifficulty = HARD;
        speed = 0.05f;
        HardLevelSound.play();
        break;
    }
    score = 0;        // Reset score
    snakeLength = 1;  // Reset snake length
    isGameOver = false;
    direction = sf::Vector2i(1, 0); // Reset direction
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        snake[i].setPosition(-TILE_SIZE, -TILE_SIZE); // Move out of bounds to creatan illusion
    }
    snake[0].setPosition(WIDTH / 2 * TILE_SIZE, HEIGHT / 2 * TILE_SIZE);
    spawnFood();
}


void SnakeGame::saveScore()
{

    const int MAX_SCORES = 100;
    int scores[MAX_SCORES];
    int count = 0;

    // Read existing scores
    std::ifstream inFile("HighScores/snake_highscores.txt");
    std::cout << "Save score function called.\n";

    if (inFile.is_open()) {
        while (inFile >> scores[count] && count < MAX_SCORES - 1) {
            count++;
        }
        inFile.close();
    }

    // adding the new score
    scores[count++] = score;

    // bubble sort for storing the scores in descending order
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (scores[i] < scores[j]) {
                std::swap(scores[i], scores[j]);
            }
        }
    }

// storing into the file
    std::ofstream outFile("HighScores/snake_highscores.txt");
    std::cout << "Save score function called.\n";
    if (outFile.is_open()) {
        for (int i = 0; i < count; i++) {
            outFile << scores[i] << std::endl;
        }
        outFile.close();
    }
    else {
        std::cerr << "Unable to save score to file." << std::endl;
    }
}
