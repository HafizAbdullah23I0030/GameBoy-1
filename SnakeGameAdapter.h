#pragma once
#include "Game.h"
#include "SnakeGame.h"

//larkay inheritance dekho zara
class SnakeGameAdapter : public Game
{
public:
    SnakeGameAdapter() = default;

    void run() override
    {
        SnakeGame snake;
        snake.run();
    }

    const char* getName() const override
    {
        return "Snake";
    }
};
