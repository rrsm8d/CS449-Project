#ifndef GENERALGAME_H
#define GENERALGAME_H

#include "gamelogic.h"


// General game will end as soon as any match is made
class GeneralGame : public GameLogic
{
public:
    GeneralGame(int boardSize);
    void MakeMove(int x, int y) override;
    void CpuMove() override;
};

#endif // GENERALGAME_H
