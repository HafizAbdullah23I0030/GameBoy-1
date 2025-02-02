#include "Category.h"
#include <cstdlib>
#include <ctime>
#include<iostream>
using namespace std;
//using namespace sf;

//constructor
Category::Category()
{
    selectedWord[0] = '\0'; // Initialize selectedWord with an empty string
    srand(time(nullptr));
}

//selection of a random word from the array
const char* Category::getRandomWord()
{
    int index = rand() % 32;    //using the rand() function to get a random word

    // Manual copying of the chosen word into `selectedWord`
    int i = 0;
    while (words[index][i] != '\0')
    {
        selectedWord[i] = words[index][i];
        ++i;
    }
    selectedWord[i] = '\0'; // Null-terminate the string

    return selectedWord;
}
