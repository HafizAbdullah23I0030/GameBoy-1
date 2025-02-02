#include "GameBoyMenu.h"
#include "HangmanGameAdapter.h"
#include "SnakeGameAdapter.h"
#include "WordleGameAdapter.h"

//driver func
int main()
{
    GameBoyMenu menu;
    menu.addGame(new HangmanGameAdapter(), 0);
    menu.addGame(new SnakeGameAdapter(), 1);
    menu.addGame(new WordleGameAdapter(), 2);

    // Run the menu
    menu.run();
    return 0;
}
