#include "Hangman.h"
#include "MyString.h"
#include <iostream>

using namespace sf;
using namespace std;

const short gap = 50;   //gap between the pixels. needed for animation
const short stpix = 125;    //the atrting pixel of the gamename 

//constructor
Hangman::Hangman() : window(VideoMode(600, 800), "Hangman"), hangmanFigure(), category()
{
    initialize();
    loadResources();
}

void Hangman::initialize()
{
    //get a random word
    const char* randomWord = category.getRandomWord();
    MyString::copy(randomWord, word);

    cout << word << endl;

    // Initialize 
    // 
    // edWord with underscores
    int len = MyString::length(word);
    for (int i = 0; i < len; ++i) {
        guessedWord[i] = '_';
    }
    guessedWord[len] = '\0';
}

void Hangman::loadResources() {
    backgroundTexture.loadFromFile("Backgrounds/2.png");       //setting the background and tweaking the opacity and associated values
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setColor(Color(255, 255, 255, 215));

    //loading fonts
    if (!font.loadFromFile("Fonts/super-legend-boy-font/SuperLegendBoy-4w8Y.ttf")) {
        cout << "Unable to load font" << endl;
    }
    //audios of the game
    HangmanSoundBuffer.loadFromFile("AudioResources/HangManAudio/Hangman.mp3");
    HangmanSound.setBuffer(HangmanSoundBuffer);
    HangmanSound.setLoop(1);
    HangmanSound.play();
}

bool returnToMenu = false; // Add this as a private variable
void Hangman::handleEvents()
{
    Event event;
    while (window.pollEvent(event))
    {
        if (event.type == Event::Closed)
            window.close();

        // Restart game when F2 is pressed (instead of exiting)
        if (isGameFinished && Keyboard::isKeyPressed(Keyboard::F2)) {
            restartGame();
        }

        // Close game when ESC is pressed
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            if (isGameFinished) {
                window.close();
            }
        }

        // Allow input only if game is NOT finished
        if (!isGameFinished && event.type == Event::TextEntered)
        {
            char guess = static_cast<char>(event.text.unicode);
            if (isalpha(guess)) {
                trackGuesses(tolower(guess));
            }
        }
    }
}


//update the window of the game
void Hangman::update()
{
    if (isGameOver()) {
        isGameFinished = true;
        return;
    }
    if (isWordGuessed()) {
        isGameFinished = true;
        return;
    }
}


//draw funciton
void Hangman::draw()
{
    window.clear(sf::Color::White);
    window.draw(backgroundSprite);
    drawGameName();
    drawGuessedWord();
    drawHangmanFigure();

    if (isGameOver()) {
        drawGameOverMessage();
    }
    else if (isWordGuessed()) {
        drawWinMessage();
    }

    window.display();
}


//this funciton checks guesses
void Hangman::trackGuesses(char guess)
{
    bool found = false;

    // Check if the guess is in the word
    for (int i = 0; word[i] != '\0'; ++i)
    {
        if (word[i] == guess) {
            guessedWord[i] = guess;
            found = true;
        }
    }

    // Decrease a life if the guess is incorrect
    if (!found) {
        hangmanFigure.decreaseLife();
    }
}

//checks if the added word is the same as the current word
bool Hangman::isWordGuessed() const
{
    return MyString::compare(word, guessedWord);
}

//check for game over
bool Hangman::isGameOver()
{
    return hangmanFigure.getLives() == 0;
}

//draws the gamename and is used in the animation
void Hangman::drawGameName()
{
    Text letters[] =
    {
        Text("H", font, 45), Text("A", font, 45), Text("N", font, 45),
        Text("G", font, 45), Text("M", font, 45), Text("A", font, 45),
        Text("N!", font, 45)
    };

    for (int i = 0; i < 7; ++i)
    {
        if (timer.getElapsedTime().asSeconds() > 3.0 + i * 0.2)
        {
            letters[i].setPosition(stpix + i * gap, 20);
            letters[i].setFillColor(Color::Black);
            window.draw(letters[i]);
        }
    }
}

//draws the guessed word by the player
void Hangman::drawGuessedWord()
{
    if (timer.getElapsedTime().asSeconds() > 6.0 && !isGameOver())
    {
        Text guessedWordText(guessedWord, font, 30);
        guessedWordText.setPosition(220, 450);
        guessedWordText.setFillColor(Color::Black);
        window.draw(guessedWordText);
    }
}

//draws the hangman figure
void Hangman::drawHangmanFigure()
{
    hangmanFigure.draw(window);
}

//gameover message function

void Hangman::drawGameOverMessage()
{
    Text gameOverText("GAME OVER!", font, 30);
    gameOverText.setPosition(170, 600);
    gameOverText.setFillColor(Color::Red);

    Text shadow(gameOverText);
    shadow.setFillColor(Color(0, 0, 0, 150));
    shadow.setPosition(gameOverText.getPosition().x + 5, gameOverText.getPosition().y + 5);

    Text retryText("Press 'F2' to Restart or 'Esc' to Exit", font, 20);
    retryText.setPosition(140, 650);
    retryText.setFillColor(Color::Black);

    window.draw(shadow);
    window.draw(gameOverText);
    window.draw(retryText);
}

void Hangman::drawWinMessage()
{
    Text winText("YOU WON!", font, 30);
    winText.setPosition(200, 600);
    winText.setFillColor(Color::Green);

    Text shadow(winText);
    shadow.setFillColor(Color(0, 0, 0, 150));
    shadow.setPosition(winText.getPosition().x + 5, winText.getPosition().y + 5);

    Text menuText("Press 'F2' to Restart or 'Esc' to Exit", font, 20);
    menuText.setPosition(180, 650);
    menuText.setFillColor(Color::Black);

    window.draw(shadow);
    window.draw(winText);
    window.draw(menuText);
}


void Hangman::restartGame()
{
    isGameFinished = false;
    returnToMenu = false;  // Ensure this is also reset

    
    hangmanFigure.reset();

    initialize();

    timer.restart();
}





void Hangman::run() {
    while (window.isOpen())
    {
        handleEvents();
        update();
        draw();

        if (returnToMenu) {
            restartGame();
        }
    }
}


