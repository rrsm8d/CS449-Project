#ifndef GAMELOGIC_H
#define GAMELOGIC_H
#include <iostream>
#include <vector>

struct Player
{
    std::string playerLetter;
    std::string playerColor;
};

class GameLogic
{
public:
    // Constructor
    GameLogic(int boardSize, int mode);
    ~GameLogic();
    void SetBoardSize(int boardSize);
    void SetGameMode(int mode);
    void MakeMove(int x, int y);

    void ClearBoard();
    bool StandardGame();
    bool GeneralGame();
    void SwitchTurn();
    bool isEmptyCell(int x, int y);
    bool isValidCell(int x, int y);

    Player *currentTurn;
    std::vector<std::vector<char>> board;
    int gameMode ; // 0 = Simple, 1 = General, 2 = None/Finished
    Player player1;
    Player player2;
private:
};

#endif // GAMELOGIC_H
