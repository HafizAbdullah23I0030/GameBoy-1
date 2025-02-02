#include "WordBank.h"
#include <cstdlib> 
#include <ctime>   
#include <cstring> 
#include <iostream>
#include "MyString.h"
using namespace std;
//using namespace sf;

WordBank::WordBank() : words(nullptr), wordCount(20)
{
    words = new char* [wordCount];
    for (int i = 0; i < wordCount; ++i) {
        words[i] = new char[6]; // Assuming 5-letter words + null terminator
    }
    initializeWords();

    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

WordBank::~WordBank() {

    for (int i = 0; i < wordCount; ++i) {
        delete[] words[i];
    }
    delete[] words;
}

void WordBank::initializeWords()
{
    const char* initialWords[] = {
        "water", "stone", "grass", "cloud", "night",
        "haste", "vines", "candy", "plane", "ocean",
        "peace", "smoke", "zebra", "glove", "truck",
        "shine", "creek", "train", "storm", "bliss"
    };


    for (int i = 0; i < wordCount; ++i) {
        MyString::copy(initialWords[i], words[i]);
    }
}

const char* WordBank::getWord() const {
    int randomIndex = std::rand() % wordCount;
    return words[randomIndex];
}
