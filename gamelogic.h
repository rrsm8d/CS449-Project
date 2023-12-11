#ifndef GAMELOGIC_H
#define GAMELOGIC_H
#include <iostream>
#include <vector>
#include <fstream>
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
    void writeToFile();

    virtual void MakeMove(int x, int y);
    virtual void CpuMove();
    Player* DetermineWinner();

    Player *currentTurn;
    std::vector<std::vector<char>> board;
    std::vector<std::string> moveHistory;
    int moveCount;
    Player player1;
    Player player2;
    bool isFinished;
    bool isO(int x, int y);
    bool isS(int x, int y);
private:

};

#endif // GAMELOGIC_H
