#include "gamelogic.h"


GameLogic::GameLogic(int boardSize)
{
    // Resize the board vector to current board size
    SetBoardSize(boardSize);
    // Set the player turn info defaults
    this->player1.playerLetter = "S";
    this->player2.playerLetter = "O";
    this->player1.playerColor = "Blue";
    this->player2.playerColor = "Red";
    this->player1.playerScore = 0;
    this->player2.playerScore = 0;
    this->player1.isCpu = false;
    this->player2.isCpu = false;
    this->currentTurn = &player1;
    this->moveCount = 0;
    this->isFinished = false;
}

GameLogic::~GameLogic()
{
    this->currentTurn = nullptr;
}

void GameLogic::SetBoardSize(int boardSize)
{
    // Resize appropriately
    if(boardSize < 4)
        boardSize = 4;
    if(boardSize > 10)
        boardSize = 10;
    // Resize the outer vector to have boardSize elements
    this->board.resize(boardSize);
    // Resize each inner vector to have boardSize elements
    for (int i = 0; i < boardSize; i++) {
        this->board[i].resize(boardSize);
    }
    ClearBoard();
}

void GameLogic::SetPlayerCpu(int player)
{
    if(player == 1)
        this->player1.isCpu = true;
    else if(player == 2)
        this->player2.isCpu = true;
}

void GameLogic::SetPlayerHuman(int player)
{
    if(player == 1)
        this->player1.isCpu = false;
    else if(player == 2)
        this->player2.isCpu = false;
}

// This function is old and overridden
void GameLogic::MakeMove(int x, int y)
{
    if(isValidCell(x, y) && isEmptyCell(x, y))
    {
        this->board[x][y] = this->currentTurn->playerLetter[0];
    }
}

void GameLogic::CpuMove()
{
    std::cerr << "Unusual call to GameLogic::CpuMove();";
}

Player* GameLogic::DetermineWinner()
{
    if(this->player1.playerScore > this->player2.playerScore)
    {
        return &player1;
    } else if(this->player2.playerScore > this->player1.playerScore)
    {
        return &player2;
    } else return nullptr;
}

void GameLogic::ClearBoard()
{
    for(auto &vec: this->board)
    {
        for(char &c : vec)
        {
            c = '_'; // A value that has not been asigned yet will be _.
        }
    }
    this->currentTurn = &this->player1;
    this->player1.playerScore = 0;
    this->player2.playerScore = 0;
    this->isFinished = false;
    this->moveCount = 0;
    this->moveHistory.clear();
    // Is the first player a computer? If so, make its move automatically.
    if(this->currentTurn->isCpu)
        CpuMove();
}

void GameLogic::SwitchTurn()
{
    // Swap to player2 if currently player1, otherwise reverse.
    if(this->currentTurn == &this->player1)
    {
        this->currentTurn = &player2;
    }else this->currentTurn = &player1;
}

bool GameLogic::isEmptyCell(int x, int y)
{
    if(this->board[x][y] == '_')
        return true;
    return false;
}

bool GameLogic::isValidCell(int x, int y)
{
    if((x >= this->board.size() || x < 0) || (y >= this->board.size() || y < 0))
        return false;
    return true;
}

bool GameLogic::isMatch(int x, int y)
{
    char thisLetter = this->currentTurn->playerLetter[0];
    bool isMatch = false;
    // S and O players have different logic

    // S will need to check up to a distance of 2 in all directions
    if(thisLetter == 'S')
    {
        // Diagonal checks
        if(isO(x+1, y+1) && isS(x+2, y+2))
        {
            isMatch = true;
            this->currentTurn->playerScore++;
        }
        if(isO(x+1, y-1) && isS(x+2, y-2))
        {
            isMatch = true;
            this->currentTurn->playerScore++;
        }
        if(isO(x-1, y-1) && isS(x-2, y-2))
        {
            isMatch = true;
            this->currentTurn->playerScore++;
        }
        if(isO(x-1, y+1) && isS(x-2, y+2))
        {
            isMatch = true;
            this->currentTurn->playerScore++;
        }
        // Horizontal check
        if(isO(x+1, y) && isS(x+2, y))
        {
            isMatch = true;
            this->currentTurn->playerScore++;
        }
        if(isO(x-1, y) && isS(x-2, y))
        {
            isMatch = true;
            this->currentTurn->playerScore++;
        }
        // Vertical check
        if(isO(x, y+1) && isS(x, y+2))
        {
            isMatch = true;
            this->currentTurn->playerScore++;
        }
        if(isO(x, y-1) && isS(x, y-2))
        {
            isMatch = true;
            this->currentTurn->playerScore++;
        }
    }

    // O will only need to check a distance of 1 in all directons
    if(thisLetter == 'O')
    {
        // Diagonal checks
        if(isS(x+1, y+1) && isS(x-1, y-1))
        {
            isMatch = true;
            this->currentTurn->playerScore++;
        }
        if(isS(x+1, y-1) && isS(x-1, y+1))
        {
            isMatch = true;
            this->currentTurn->playerScore++;
        }
        // Horizontal check
        if(isS(x+1, y) && isS(x-1, y))
        {
            isMatch = true;
            this->currentTurn->playerScore++;
        }
        // Vertical check
        if(isS(x, y+1) && isS(x, y-1))
        {
            isMatch = true;
            this->currentTurn->playerScore++;
        }
    }

    return isMatch;
}

bool GameLogic::isO(int x, int y)
{
    if(isValidCell(x, y) && this->board[x][y] == 'O')
    {
        return true;
    } else return false;
}

bool GameLogic::isS(int x, int y)
{
    if(isValidCell(x, y) && this->board[x][y] == 'S')
    {
        return true;
    } else return false;
}

void GameLogic::writeToFile()
{
    std::string filename = "savedgame.txt";
    std::ofstream outputFile(filename);
    // Check if the file is opened successfully
    if (outputFile.is_open()) {
        // Write each pair of coordinates to the file
        for (const auto& coord : this->moveHistory)
        {
            // Format the coordinates as "x y S/O" and write to the file
            outputFile << coord << "\n";
        }
        // Close the file
        outputFile.close();
        std::cout << "Coordinates written to file: " << filename << std::endl;
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }

}
