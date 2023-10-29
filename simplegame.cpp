#include "simplegame.h"

SimpleGame::SimpleGame(int boardSize) : GameLogic::GameLogic(boardSize)
{
    // Nothing to see here?
}

void SimpleGame::MakeMove(int x, int y)
{
    if(isValidCell(x, y) && isEmptyCell(x, y) && !this->isFinished)
    {
        this->board[x][y] = this->currentTurn->playerLetter[0];
        this->moveCount++;
        // Simple games will continue if the player makes a match
        if(isMatch(x,y))
        {
            std::cout << "Match made: Simple" << std::endl;
        } else
        {
            std::cout << "Switching Turns" << std::endl;
            SwitchTurn();
        }
        // Simple game will end when there is no more space left
        if(moveCount >= this->board.size()*this->board.size())
        {
            this->isFinished = true;
        }
    }
}
