
#pragma once
#include<iostream>
using namespace std;
//using namespace sf;

#include <cstdlib>

class Category
{
private:
    //laiba from askari/dha meme incoming
    const char* words[24] = { 
     "joker", "alfred", "steve",
     "skibidi", "joohnydepp", "bheem", "raju", 
     "asimmunir", "programming", "africa", "junoon", "bayan", "elephant",
     "walterwhite", "hank", "spiderman", "maryjane", 
     "einstein", "lewishamilton", "tonystarck", "brucewayne",  "armstrong", "tesla", "placebo" };
    char selectedWord[20];

public:
    Category();
    const char* getRandomWord();
};
