#pragma once
class Game {
public:
    virtual ~Game() = default;
    virtual void run() = 0;    
    virtual const char* getName() const = 0;
};
