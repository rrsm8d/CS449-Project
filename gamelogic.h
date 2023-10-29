#ifndef GAMELOGIC_H
#define GAMELOGIC_H
#include <iostream>
#include <vector>

struct Player
{
    std::string playerLetter;
    std::string playerColor;
    int playerScore;
};

class GameLogic
{
public:
    // Constructor
    GameLogic(int boardSize);
    ~GameLogic();
    void SetBoardSize(int boardSize);

    void ClearBoard();
    void SwitchTurn();
    bool isEmptyCell(int x, int y);
    bool isValidCell(int x, int y);
    bool isMatch(int x, int y);

    virtual void MakeMove(int x, int y);
    Player* DetermineWinner();

    Player *currentTurn;
    std::vector<std::vector<char>> board;
    int moveCount;
    Player player1;
    Player player2;
    bool isFinished;
private:
    bool isO(int x, int y);
    bool isS(int x, int y);
};

#endif // GAMELOGIC_H
