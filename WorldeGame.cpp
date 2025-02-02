#include "WordleGame.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "MyString.h"
#include <iostream>
#include <fstream>


WordleGame::WordleGame()
    : currentAttempt(0), maxAttempts(6), guessIndex(0), hintUsed_orNot(0),
    score(0), displayReplayPrompt(false), isReplayChoiceMade(false) {
    // Allocate memory for word
    word = new char[6];
    hintClock.restart();
    // Allocate memory for guesses and feedback
    guesses = new char* [maxAttempts];
    feedback = new char* [maxAttempts];
    currentStreak = 0;
    loadStreak();

    for (int i = 0; i < maxAttempts; ++i) {
        guesses[i] = new char[6];
        feedback[i] = new char[6];
        MyString::copy("", guesses[i]);
        MyString::copy("", feedback[i]);
    }

    const char* newWord = wordBank.getWord();
    MyString::copy(newWord, word);
    MyString::copy("", currentGuess);
    show_word_onScreen(newWord);

    if (!backgroundTexture.loadFromFile("Backgrounds/1.png")) {
        std::cerr << "Failed to load background image\n";
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setColor(sf::Color(255, 255, 255, 185));

    if (!backgroundMusic.openFromFile("AudioResources/WordleSound/Undertale OST - 085 Fallen Down (Reprise).mp3")) {
        std::cerr << "Music not working\n";
    }
    backgroundMusic.setLoop(true);
    backgroundMusic.play();


    if (!EasterEggMusic.openFromFile("AudioResources/EasterEggSound/spiderman-meme-song.mp3")) {
        std::cerr << "Easter Egg Music not working\n";
    }

    for (int i = 0; i < maxAttempts; ++i) {
        guesses[i][0] = '\0';
        feedback[i][0] = '\0';
    }

    wordleClock.restart();
}

void WordleGame::show_word_onScreen(const char* newWord) {
    for (int i = 0; newWord[i] != '\0'; i++)
    {
        std::cout << newWord[i];
    }
}

WordleGame::~WordleGame() {
    if (word) {
        delete[] word;
    }

    if (guesses && feedback) {
        for (int i = 0; i < maxAttempts; ++i) {
            if (guesses[i]) delete[] guesses[i];
            if (feedback[i]) delete[] feedback[i];
        }
        delete[] guesses;
        delete[] feedback;
    }
}
///

void WordleGame::run(sf::RenderWindow& window) {
    while (window.isOpen())
    {
        handleEvents(window);
        update();
        render(window);
    }
}

// String length
int WordleGame::stringLength(const char* str) {
    return MyString::length(str);
}

// Copy string
void WordleGame::copyString(const char* source, char* destination) {
    MyString::copy(source, destination);
}
// Check if guess is correct
bool WordleGame::checkGuess(const char* guess) {
    if (MyString::length(guess) != 5) {
        return false; // Invalid guess length
    }

    // Copy the guess into the guesses array
    MyString::copy(guess, guesses[currentAttempt]);

    // Evaluate the guess and store feedback
    evaluateGuess(guess, feedback[currentAttempt]);
    currentAttempt++;

    // Check if the guess matches the target word
    if (isWordGuessed()) {
        std::cout << "Correct!\n";
        score += 10;

        // Update streak
        currentStreak++;
        if (currentStreak > maxStreak) {
            maxStreak = currentStreak;

            // Play special music if not already played
            if (!EasterEggMusicPlayed) {
                backgroundMusic.pause();
                EasterEggMusic.play();
                EasterEggMusicPlayed = true;
                EasterEggMusic.setLoop(false); \
                    EasterEggMusic.setPlayingOffset(sf::seconds(0));
            }
        }

        saveStreak(); // Save streaks to file
    }
    else if (currentAttempt >= maxAttempts) {
        // Reset streak on game over
        std::cout << "Incorrect! Game over.\n";
        currentStreak = 0;
        saveStreak(); // Save streaks to file
    }

    return true;
}


///


void WordleGame::evaluateGuess(const char* guess, char result[6]) {
    char wordCopy[6];
    copyString(word, wordCopy);

    for (int i = 0; i < 5; ++i) result[i] = 'X';
    result[5] = '\0';

    for (int i = 0; i < 5; ++i) {
        if (guess[i] == wordCopy[i]) {
            result[i] = 'G';
            wordCopy[i] = '*';
        }
    }

    for (int i = 0; i < 5; ++i) {
        if (result[i] == 'G') {
            continue;
        }

        for (int j = 0; j < 5; ++j) {
            if (guess[i] == wordCopy[j] && wordCopy[j] != '*') {
                result[i] = 'Y';
                wordCopy[j] = '*';
                break;
            }
        }
    }
}

void WordleGame::handleEvents(sf::RenderWindow& window) {
    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (isGameOver() && !isReplayChoiceMade) {
            displayReplayPrompt = true;

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) { // 1 for replay
                    replayChoice = 'y';
                    isReplayChoiceMade = true;
                    reset_game();
                    displayReplayPrompt = false;
                    hintUsed_orNot = false;
                }
                else if (event.key.code == sf::Keyboard::Num0) { // 0 for not replay
                    replayChoice = 'n';
                    isReplayChoiceMade = true;
                    saveScore();
                    window.close();
                }
            }

        }




        if (event.type == sf::Event::TextEntered) {
            char input = static_cast<char>(event.text.unicode);

            if (std::isalpha(input) && guessIndex < 5) {
                currentGuess[guessIndex++] = std::tolower(input);
                currentGuess[guessIndex] = '\0';
            }
            else if (input == '\b' && guessIndex > 0) { // Backspace
                currentGuess[--guessIndex] = '\0';
            }
            else if (input == '\r' && guessIndex == 5) {
                if (checkGuess(currentGuess)) {
                    guessIndex = 0;
                    currentGuess[0] = '\0'; // Reset the guess
                }
            }
        }

        // Check for hint key (F1)
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F1 && !hintUsed_orNot) {
            hintUsed_orNot = true; // Activate the hint
            if (score >= 10 && hintUsed_orNot == 1) {
                score -= 5;
            }
        }
    }
}



void WordleGame::update() {
    // Checks if special music has finished playing
    if (EasterEggMusicPlayed && EasterEggMusic.getStatus() == sf::SoundSource::Stopped) {
        backgroundMusic.play(); // Resume main music
        EasterEggMusicPlayed = false;
    }
}

void WordleGame::render(sf::RenderWindow& window) {
    sf::Font font;
    font.loadFromFile("Fonts/super-legend-boy-font/SuperLegendBoy-4w8Y.ttf");

    window.clear(sf::Color::White);
    drawBackground(window);
    drawGameTitle(window, font);
    drawGuesses(window, font);
    drawHint(window, font);
    drawScore(window, font);

    if (displayReplayPrompt) {
        ask_for_replay_prompt(window, font);
    }
    if (isGameOver()) {
        drawGameOver(window, font);
    }
    window.display();
}


void WordleGame::drawScore(sf::RenderWindow& window, sf::Font& font) {
    // Calculate window dimensions
    float windowWidth = window.getSize().x;

    // Draw score text
    sf::Text scoreText("Score: " + std::to_string(score), font, 30);
    scoreText.setFillColor(sf::Color::Black);

    // Center horizontally and position at the top
    sf::FloatRect scoreBounds = scoreText.getLocalBounds();
    scoreText.setPosition((windowWidth - scoreBounds.width) / 2, 10);
    window.draw(scoreText);

    // Draw streak text below the score
    sf::Text streakText(
        "Current Streak: " + std::to_string(currentStreak) +
        "\nMax Streak: " + std::to_string(maxStreak),
        font, 20
    );
    streakText.setFillColor(sf::Color::Black);

    // Center horizontally and position below the score
    sf::FloatRect streakBounds = streakText.getLocalBounds();
    streakText.setPosition((windowWidth - streakBounds.width) / 2, 50);
    window.draw(streakText);
}




void WordleGame::drawBackground(sf::RenderWindow& window) {
    window.draw(backgroundSprite);
}

void WordleGame::drawGameTitle(sf::RenderWindow& window, sf::Font& font) {
    sf::Text letters[6] = {
        sf::Text("W", font, 35),
        sf::Text("O", font, 35),
        sf::Text("R", font, 35),
        sf::Text("D", font, 35),
        sf::Text("L", font, 35),
        sf::Text("E!", font, 35)
    };


    bool gamenameDisplay[6] = { false, false, false, false, false, false };

    if (wordleClock.getElapsedTime().asSeconds() > 1.3f) {
        gamenameDisplay[0] = true;
    }
    if (wordleClock.getElapsedTime().asSeconds() > 1.5f) {
        gamenameDisplay[1] = true;
    }
    if (wordleClock.getElapsedTime().asSeconds() > 1.6f) {
        gamenameDisplay[2] = true;
    }
    if (wordleClock.getElapsedTime().asSeconds() > 1.8f) {
        gamenameDisplay[3] = true;
    }
    if (wordleClock.getElapsedTime().asSeconds() > 2.1f) {
        gamenameDisplay[4] = true;
    }
    if (wordleClock.getElapsedTime().asSeconds() > 2.3f) {
        gamenameDisplay[5] = true;
    }

    for (int i = 0; i < 6; ++i) {
        if (gamenameDisplay[i]) {
            letters[i].setPosition(getstpix() + i * getgap(), 100);
            letters[i].setFillColor(sf::Color::Black);
            window.draw(letters[i]);
        }
    }
}

void WordleGame::drawGuesses(sf::RenderWindow& window, sf::Font& font) {
    // Calculate grid size
    const int gridWidth = 5 * 50;  // 5 letters per guess, each 50 pixels wide
    const int gridHeight = maxAttempts * 50;  // 6 attempts, each 50 pixels tall

    // Calculate the starting position to center the grid
    const int windowWidth = window.getSize().x;
    const int windowHeight = window.getSize().y;
    const int startX = (windowWidth - gridWidth) / 2;
    const int startY = (windowHeight - gridHeight) / 2;

    // Draw guesses
    for (int i = 0; i < currentAttempt; ++i) {
        for (int j = 0; j < 5; ++j) {
            // Draw block for feedback
            sf::RectangleShape rect({ 40, 40 });
            rect.setPosition(startX + j * 50, startY + i * 50);

            if (feedback[i][j] == 'G')
                rect.setFillColor(sf::Color::Green);
            else if (feedback[i][j] == 'Y')
                rect.setFillColor(sf::Color::Yellow);
            else
                rect.setFillColor(sf::Color(211, 211, 211)); // Light gray for incorrect

            window.draw(rect);

            // Draw guessed letter inside the block
            sf::Text letterText(std::string(1, guesses[i][j]), font, 30);
            letterText.setPosition(startX + j * 50 + 10, startY + i * 50 + 5);
            letterText.setFillColor(sf::Color::Black);
            window.draw(letterText);
        }
    }

    // Render the current guess in progress
    for (int j = 0; j < guessIndex; ++j) {
        sf::RectangleShape rect({ 40, 40 });
        rect.setPosition(startX + j * 50, startY + currentAttempt * 50);
        rect.setFillColor(sf::Color(211, 211, 211)); // Default color

        sf::Text letterText(std::string(1, currentGuess[j]), font, 30);
        letterText.setPosition(startX + j * 50 + 10, startY + currentAttempt * 50 + 5);
        letterText.setFillColor(sf::Color::Black);

        window.draw(rect);
        window.draw(letterText);
    }
}


void WordleGame::drawGameOver(sf::RenderWindow& window, sf::Font& font) {
    sf::Text message;
    message.setFont(font);
    message.setCharacterSize(30);
    message.setFillColor(sf::Color::Cyan);
    if (isWordGuessed()) {
        message.setString("You Win!");
    }
    else {
        message.setString("Game Over! The word was: " + std::string(word));
    }

    sf::FloatRect textRect = message.getLocalBounds();
    float textWidth = textRect.width;
    float windowWidth = window.getSize().x;

    message.setPosition((windowWidth - textWidth) / 2, 750); 
    window.draw(message);
}


bool WordleGame::isGameOver() const {
    return currentAttempt >= maxAttempts || isWordGuessed();
}

bool WordleGame::isWordGuessed() const {
    for (int i = 0; i < currentAttempt; ++i) {
        if (feedback[i][0] == 'G' && feedback[i][1] == 'G' &&
            feedback[i][2] == 'G' && feedback[i][3] == 'G' &&
            feedback[i][4] == 'G') {
            return true;
        }
    }
    return false;
}



int WordleGame::getstpix() {
    return stpix;  // Starting pixel position for the first letter
}

int WordleGame::getgap() {
    return gap;  // Gap between letters
}




void WordleGame::drawHint(sf::RenderWindow& window, sf::Font& font) {
    if (hintUsed_orNot) {
        if (hintClock.getElapsedTime().asSeconds() < 5) {
            hintUsed_orNot = false;
            return;
        }
        std::string hintTextStr = "Hint: First letter is '" + std::string(1, word[0]) + "'";
        sf::Text hintText(hintTextStr, font, 30);
        hintText.setFillColor(sf::Color::Blue);


        sf::FloatRect textRect = hintText.getLocalBounds();
        float textWidth = textRect.width;
        float windowWidth = window.getSize().x;

        hintText.setPosition((windowWidth - textWidth) / 2, 700); 
        window.draw(hintText);
    }
}

void WordleGame::reset_game() {
    currentAttempt = 0;
    guessIndex = 0;
    MyString::copy("", currentGuess);

    for (int i = 0; i < maxAttempts; ++i) {
        MyString::copy("", guesses[i]);
        MyString::copy("", feedback[i]);
    }

    const char* newWord = wordBank.getWord(); // Fetch a new word
    MyString::copy(newWord, word);

    hintUsed_orNot = false;
    hintClock.restart();
    isReplayChoiceMade = false;
    displayReplayPrompt = false;
    EasterEggMusicPlayed = false;
    show_word_onScreen(newWord);
}



void WordleGame::ask_for_replay_prompt(sf::RenderWindow& window, sf::Font& font) {
    sf::Text promptText("Play again? (1 for 'Y'/ 0 for 'N')", font, 20);
    promptText.setFillColor(sf::Color::Red);

    // Center the prompt text
    sf::FloatRect textRect = promptText.getLocalBounds();
    promptText.setPosition(
        (window.getSize().x - textRect.width) / 2,
        (window.getSize().y - textRect.height) / 2
    );

    window.draw(promptText);
}

void WordleGame::saveScore() {
    const char* filePath = "HighScores/wordle_highscores.txt";

    int scores[100]; // Array to hold scores
    int count = 0;   // Number of scores in the file

    // Read scores from file
    std::ifstream inFile(filePath);
    if (inFile.is_open()) {
        while (inFile >> scores[count]) {
            ++count;
            if (count == 100) {
                std::cerr << "Warning: Maximum capacity of scores reached (100).\n";
                break;
            }
        }
        inFile.close();
    }
    else {
        std::cerr << "Warning: Unable to open file. A new file will be created.\n";
    }

    // Add the current score
    scores[count] = score;
    ++count;

    // Sort scores in descending order using Bubble Sort
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            if (scores[j] < scores[j + 1]) {
                int temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }

    // Write sorted scores back to the file
    std::ofstream outFile(filePath);
    if (outFile.is_open()) {
        for (int i = 0; i < count; ++i) {
            outFile << scores[i] << "\n";
        }
        outFile.close();
    }
    else {
        std::cerr << "Error: Unable to open file for saving scores. Check file path and permissions.\n";
    }
}

void WordleGame::loadStreak() {
    const char* filePath = "HighScores/achievements_wordle.txt";
    std::ifstream inFile(filePath);

    if (inFile.is_open()) {
        inFile >> maxStreak;
        inFile.close();
    }
    else {
        // File not found or cannot be opened, initialize streaks to 0
        currentStreak = 0;
        maxStreak = 0;
    }
}

void WordleGame::saveStreak() {
    const char* filePath = "HighScores/achievements_wordle.txt";
    std::ofstream outFile(filePath);

    if (outFile.is_open()) {
        outFile << maxStreak << "\n";
        outFile.close();
    }
    else {
        std::cerr << "Error: Unable to save streaks to file.\n";
    }
}