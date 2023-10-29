#ifndef SIMPLEGAME_H
#define SIMPLEGAME_H

#include "gamelogic.h"


// Simple games will continue if the player makes a match
class SimpleGame : public GameLogic
{
public:
    SimpleGame(int boardSize);
    void MakeMove(int x, int y) override;
};

#endif // SIMPLEGAME_H
