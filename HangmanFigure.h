#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;


class HangmanFigure {
private:
    int lives; // Number of remaining lives

    sf::CircleShape head;
    sf::RectangleShape body, leftArm, rightArm, leftLeg, rightLeg;

    //sprites 
    sf::Texture ropeTexture;
    sf::Texture headTexture;
    sf::Texture trunkTexture;
    sf::Texture halfBodyTexture;
    sf::Texture fullBodyTexture;

    sf::Sprite ropeSprite;
    sf::Sprite headSprite;
    sf::Sprite trunkSprite;
    sf::Sprite halfBodySprite;
    sf::Sprite fullBodySprite;

public:
    //prototypes 
    HangmanFigure();
    void draw(sf::RenderWindow& window);
    void decreaseLife();
    int getLives() const;
    void reset();
};
