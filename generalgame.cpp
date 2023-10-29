#include "generalgame.h"

GeneralGame::GeneralGame(int boardSize) : GameLogic::GameLogic(boardSize)
{
    // Nothing to see here?
}

void GeneralGame::MakeMove(int x, int y)
{
    if(isValidCell(x, y) && isEmptyCell(x, y) && !this->isFinished)
    {
        this->board[x][y] = this->currentTurn->playerLetter[0];
        this->moveCount++;
        // General games will end if the player makes a match
        if(isMatch(x,y))
        {
            std::cout << "Match made: General" << std::endl;
            // GAME SHOULD END
            this->isFinished = true;
        }
        // If no matches were made
        if (moveCount >= this->board.size()*this->board.size())
        {
            this->isFinished = true;
        }
        std::cout << "Switching Turns" << std::endl;
        SwitchTurn();
    }
}

