#ifndef GAMELOGIC_H
#define GAMELOGIC_H
#include <iostream>
#include <vector>
#include <cstdlib> // for RNG in CPU decision making
#include <algorithm> // temporary

struct Player
{
    std::string playerLetter;
    std::string playerColor;
    int playerScore;
    bool isCpu;
};

class GameLogic
{
public:
    // Constructor
    GameLogic(int boardSize);
    ~GameLogic();
    void SetBoardSize(int boardSize);
    void SetPlayerCpu(int player);
    void SetPlayerHuman(int player);

    void ClearBoard();
    void SwitchTurn();
    bool isEmptyCell(int x, int y);
    bool isValidCell(int x, int y);
    bool isMatch(int x, int y);

    virtual void MakeMove(int x, int y);
    virtual void CpuMove();
    Player* DetermineWinner();

    Player *currentTurn;
    std::vector<std::vector<char>> board;
    int moveCount;
    Player player1;
    Player player2;
    bool isFinished;
    bool isO(int x, int y);
    bool isS(int x, int y);
private:

};

#endif // GAMELOGIC_H
