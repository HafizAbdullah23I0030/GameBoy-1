#include <cstdlib>
#include <ctime>
#include "String.h"
#pragma once
class WordBank {
private:
    char** words;        // Dynamically allocated array of words
    int wordCount;       // Number of words in the bank
    void initializeWords();

public:
    WordBank();
    ~WordBank();
    const char* getWord() const;
};
