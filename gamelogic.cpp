#include "gamelogic.h"

GameLogic::GameLogic(int boardSize, int mode)
{
    // Resize the board vector to current board size
    SetBoardSize(boardSize);
    // Set game mode (0=simple, 1=general, 2 = none/finished)
    // If an invalid mode selection (This shouldn't ever happen)
    SetGameMode(mode);
    // Set the player turn info defaults
    this->player1.playerLetter = "S";
    this->player2.playerLetter = "O";
    this->player1.playerColor = "Blue";
    this->player2.playerColor = "Red";
    this->currentTurn = &player1;
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

// 0 = Simple, 1 = General, 2 = None/Error
void GameLogic::SetGameMode(int mode)
{
    // If an invalid mode selection (This shouldn't ever happen)
    if(mode < 0 || mode > 2)
    {
        this->gameMode = 2;
    } else this->gameMode = mode;
    ClearBoard();
}

void GameLogic::MakeMove(int x, int y)
{
    if(isValidCell(x, y) && isEmptyCell(x, y))
    {
        this->board[x][y] = this->currentTurn->playerLetter[0];
        // TO-DO: Run game logic to determine if we should switch players after this move
        SwitchTurn();
    }
}

void GameLogic::ClearBoard()
{
    for(auto &vec: this->board)
    {
        for(char &c : vec)
        {
            c = 'W'; // A value that has not been asigned yet will be W.
        }
    }
    this->currentTurn = &this->player1;
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
    if(this->board[x][y] == 'W')
        return true;
    return false;
}

bool GameLogic::isValidCell(int x, int y)
{
    if((x > this->board.size() || x < 0) || (y > this->board.size() || y < 0))
        return false;
    return true;
}

bool GameLogic::StandardGame()
{
    // Game logic
    // If a match is found, current players turn is NOT changed. Draw a line
    // Else, SwitchTurn()
    // If vector is full, end game
    return false;
}

bool GameLogic::GeneralGame()
{
    // Game logic
    // If a match is found, end game and draw line
    // SwitchTurn()
    return false;
}
