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
        this->moveHistory.push_back(std::to_string(x) + " " + std::to_string(y) + " " + this->currentTurn->playerLetter[0]);
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

void GeneralGame::CpuMove()
{
    int boardSize = this->board.size();
    char playerLetter = this->currentTurn->playerLetter[0];

    // The position of the first move will not matter, make move randomly
    if(this->moveCount == 0)
    {
        int x = (rand() % (boardSize));
        int y = (rand() % (boardSize));
        MakeMove(x,y);
    } else
    {
        // Filled boards will need to account for occupied spaces and finding good spots
        // First, check for a way to find a match
        std::vector<std::pair<int, int>> potentialMatches;
        std::vector<std::pair<int, int>> emptyCoords;

        // Find all empty spots
        for(int i = 0; i < boardSize; i++)
        {
            for(int j = 0; j < boardSize; j++)
            {
                if(this->board[i][j] == '_')
                {
                    emptyCoords.push_back(std::make_pair(i, j));
                }
            }
        }

        for(auto &pair : emptyCoords)
        {
            int x = pair.first;
            int y = pair.second;
            // CHECKING FOR CPU PLAYER S
            if(playerLetter == 'S')
            {
                bool matchFound = false;
                // Look for anything that has S O [blank]
                // Diagonal checks
                if(isO(x+1, y+1) && isS(x+2, y+2) && !matchFound)
                {
                    potentialMatches.push_back(std::make_pair(x,y));
                    matchFound = true;
                }
                if(isO(x+1, y-1) && isS(x+2, y-2) && !matchFound)
                {
                    potentialMatches.push_back(std::make_pair(x,y));
                    matchFound = true;
                }
                if(isO(x-1, y-1) && isS(x-2, y-2) && !matchFound)
                {
                    potentialMatches.push_back(std::make_pair(x,y));
                    matchFound = true;
                }
                if(isO(x-1, y+1) && isS(x-2, y+2) && !matchFound)
                {
                    potentialMatches.push_back(std::make_pair(x,y));
                    matchFound = true;
                }
                // Horizontal check
                if(isO(x+1, y) && isS(x+2, y) && !matchFound)
                {
                    potentialMatches.push_back(std::make_pair(x,y));
                    matchFound = true;
                }
                if(isO(x-1, y) && isS(x-2, y) && !matchFound)
                {
                    potentialMatches.push_back(std::make_pair(x,y));
                    matchFound = true;
                }
                // Vertical check
                if(isO(x, y+1) && isS(x, y+2) && !matchFound)
                {
                    potentialMatches.push_back(std::make_pair(x,y));
                    matchFound = true;
                }
                if(isO(x, y-1) && isS(x, y-2) && !matchFound)
                {
                    potentialMatches.push_back(std::make_pair(x,y));
                    matchFound = true;
                }
            }
            // END OF CHECKING FOR CPU PLAYER S

            // CHECKING FOR CPU PLAYER O
            if (playerLetter == 'O')
            {
                bool matchFound = false;
                // Diagonal checks
                if(isS(x+1, y+1) && isS(x-1, y-1) && !matchFound)
                {
                    potentialMatches.push_back(std::make_pair(x,y));
                    matchFound = true;
                }
                if(isS(x+1, y-1) && isS(x-1, y+1) && !matchFound)
                {
                    potentialMatches.push_back(std::make_pair(x,y));
                    matchFound = true;
                }
                // Horizontal check
                if(isS(x+1, y) && isS(x-1, y) && !matchFound)
                {
                    potentialMatches.push_back(std::make_pair(x,y));
                    matchFound = true;
                }
                // Vertical check
                if(isS(x, y+1) && isS(x, y-1) && !matchFound)
                {
                    potentialMatches.push_back(std::make_pair(x,y));
                    matchFound = true;
                }
            }
            // END OF CHECKING FOR CPU PLAYER O
        }

        // ugly, fix immediately
        for (auto it = potentialMatches.begin(); it != potentialMatches.end(); ++it) {
            emptyCoords.erase(std::remove_if(emptyCoords.begin(), emptyCoords.end(),
                                         [it](const std::pair<int, int>& p) {
                                             return p == *it;
                                         }),
                          emptyCoords.end());
        }


        // Make a move (We can only make one move here)
        if(potentialMatches.size() > 0)
        {
            int x = potentialMatches[0].first;
            int y = potentialMatches[0].second;
            MakeMove(x,y);
        } else {
            int randCoord = (rand() % (emptyCoords.size() - 0 + 1));
            int x = emptyCoords[randCoord].first;
            int y = emptyCoords[randCoord].second;
            MakeMove(x,y);
        }
    }
}
