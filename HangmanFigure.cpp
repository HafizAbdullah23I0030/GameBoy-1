#include "HangmanFigure.h"

HangmanFigure::HangmanFigure() : lives(5)
{
    //loading the spriets
    if (!ropeTexture.loadFromFile("HangmanFigure/Rope.png"))
        cout << "Failed to load Rope.png";
    if (!headTexture.loadFromFile("HangmanFigure/HeadInRope.png"))
        cout << "Failed to load HeadInRope.png";
    if (!trunkTexture.loadFromFile("HangmanFigure/trunkinropy.png"))
        cout << "Failed to load trunkinropy.png";
    if (!halfBodyTexture.loadFromFile("HangmanFigure/halfbodyinrope.png"))
        cout << "Failed to load halfbodyinrope.png";
    if (!fullBodyTexture.loadFromFile("HangmanFigure/fullbodyinrope.png"))
        cout << "Failed to load fullbodyinrope.png";

    // Set up sprites
    ropeSprite.setTexture(ropeTexture);
    headSprite.setTexture(headTexture);
    trunkSprite.setTexture(trunkTexture);
    halfBodySprite.setTexture(halfBodyTexture);
    fullBodySprite.setTexture(fullBodyTexture);

    //variables for position
    float baseX = 400;
    float baseY = 300;
    float sizex = 3.0f;
    float sizey = 3.0f;
    //these functions scale the sprites 
    ropeSprite.setScale(sizex, sizey);
    headSprite.setScale(sizex, sizey);
    trunkSprite.setScale(sizex, sizey);
    halfBodySprite.setScale(sizex, sizey);
    fullBodySprite.setScale(sizex, sizey);

    //these set the position of the sprite
    ropeSprite.setPosition(baseX, baseY);
    headSprite.setPosition(baseX, baseY);
    trunkSprite.setPosition(baseX, baseY);
    halfBodySprite.setPosition(baseX, baseY);
    fullBodySprite.setPosition(baseX, baseY);
}

void HangmanFigure::draw(sf::RenderWindow& window)
{
    if (lives <= 4)
    {
        window.draw(ropeSprite);
    }
    if (lives <= 3)
    {
        window.draw(headSprite);
    }
    if (lives <= 2) window.draw(trunkSprite);
    if (lives <= 1) window.draw(halfBodySprite);
    if (lives <= 0) window.draw(fullBodySprite);
}

//handle lives
void HangmanFigure::decreaseLife()
{
    if (lives > 0)
        lives--;
}

//getter for lives
int HangmanFigure::getLives() const
{
    return lives;
}

void HangmanFigure::reset() {
    lives = 5;  // Reset lives to the initial value
}
