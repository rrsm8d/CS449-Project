#include <QtTest>

// add necessary includes here
#include "../gamelogic.h"
#include "../gamelogic.cpp"
class MyUnitTests : public QObject
{
    Q_OBJECT

public:
    MyUnitTests();
    ~MyUnitTests();

private slots:
    // CURRENT AC TO-DO:
    // 3.1, 4.3, 5.1, 5.2, 6.3, 6.4, 7.1, 7.2
    void BoardSize_WhenValid_IsSet(); // AC 1.1
    void BoardSize_WhenInvalid_IsAdjusted(); // AC 1.2
    void Board_WhenBoardSizeIsSet_IsClearedAndResized(); // AC 1.3

    void GameMode_WhenRadioClicked_IsSet(); // AC 2.1
    void BoardAndPlayer_WhenRadioClicked_IsCleared(); // AC 2.2

    void BoardAndPlayer_WhenEmptyCellClicked_Simple_IsSet(); // AC 4.1
    void Board_WhenFilledCellClicked_Simple_IsIgnored(); // AC 4.2
    void Board_WhenAnyCellClickedFinished_Simple_IsIgnored(); // AC 4.3
    void Board_WhenOOBCellClicked_Simple_IsIgnored(); // AC 4.4

    void BoardAndPlayer_WhenResetButtonClicked_Simple_IsSet(); // AC 5.1
    void Replay_WhenReplayButtonClicked_Simple_IsPlayed(); // AC 5.2 (WILL FAIL)

    void BoardAndPlayer_WhenEmptyCellClickedNoMatch_General_IsSet(); // AC 6.1
    void Board_WhenFilledCellClicked_General_IsIgnored(); // AC 6.2
    void Board_WhenAnyCellClickedFinished_General_IsIgnored(); // AC 6.3
    void Board_WhenEmptyCellClickedMakesMatch_General_IsSet(); // AC 6.4 (WILL FAIL)

    void Board_WhenResetButtonClicked_General_IsSet(); // AC 7.1
    void Replay_WhenReplayButtonClicked_General_IsPlayed(); // AC 7.2 (WILL FAIL)
};

MyUnitTests::MyUnitTests()
{

}

MyUnitTests::~MyUnitTests()
{

}

// AC 1.1
void MyUnitTests::BoardSize_WhenValid_IsSet()
{
    GameLogic logic {5, 2};
    QVERIFY(logic.board.size() == 5);
    logic.SetBoardSize(10);
    QVERIFY(logic.board.size() == 10);
}
// AC 1.2
void MyUnitTests::BoardSize_WhenInvalid_IsAdjusted()
{
    int overSize = 11;
    int underSize = 1;

    GameLogic logic {overSize, 2};
    QVERIFY(logic.board.size() == 10);

    logic.SetBoardSize(underSize);
    QVERIFY(logic.board.size() == 4);
}

// AC 1.3
void MyUnitTests::Board_WhenBoardSizeIsSet_IsClearedAndResized()
{
    GameLogic logic {5, 1};
    Player* playerCopy = logic.currentTurn;
    logic.MakeMove(0,0); // S player makes move at button (0, 0)
    logic.SetBoardSize(6);

    QVERIFY(logic.board.size() == 6);
    QVERIFY(logic.board[0][0] == 'W');
    QVERIFY(playerCopy == logic.currentTurn); // currentPlayer should go back to player1
}

// AC 2.1
void MyUnitTests::GameMode_WhenRadioClicked_IsSet()
{
    GameLogic logic {5, 1};
    logic.SetGameMode(0);
    QVERIFY(logic.gameMode == 0);
}

// AC 2.2
void MyUnitTests::BoardAndPlayer_WhenRadioClicked_IsCleared()
{
    GameLogic logic {5, 1};
    Player* playerCopy = logic.currentTurn;
    logic.MakeMove(0,0); // S player makes move at button (0, 0)
    logic.MakeMove(3,3); // O player makes move at button (3, 3)
    logic.MakeMove(1,5); // S player makes move at button (1, 5)
    logic.SetGameMode(0);
    // Make sure the whole board was cleared
    for(auto &vec: logic.board)
    {
        for(char &c : vec)
        {
            if(c != 'W') // If it has a value...
            {
                QFAIL("The board was not properly cleared");
            }
        }
    }
    QVERIFY(playerCopy == logic.currentTurn);
}

// 3.1 will be manually tested

// AC 4.1
void MyUnitTests::BoardAndPlayer_WhenEmptyCellClicked_Simple_IsSet()
{
    GameLogic logic {5, 0};
    Player* playerCopy = logic.currentTurn;
    logic.MakeMove(0,0); // S player makes move at button (0, 0)
    QVERIFY(logic.board[0][0] == 'S');
    QVERIFY(playerCopy != logic.currentTurn);
}

// AC 4.2
void MyUnitTests::Board_WhenFilledCellClicked_Simple_IsIgnored()
{
    GameLogic logic {5, 0};
    logic.board[0][0] = 'O';

    Player* playerCopy = logic.currentTurn;
    //qDebug()  << playerCopy << " : " << logic.currentTurn;
    logic.MakeMove(0,0);
    //qDebug()  << playerCopy << " : " << logic.currentTurn;
    // Nothing should have changed, making the move ignored.
    QVERIFY(playerCopy == logic.currentTurn);
    QVERIFY(logic.board[0][0] == 'O');
}

// TO-DO: 4.3 Implement a finished state to game logic
// AC 4.3
void MyUnitTests::Board_WhenAnyCellClickedFinished_Simple_IsIgnored()
{
    GameLogic logic {5, 0};
    logic.board[3][2] = 'O';
    //<- Set game to finished here ->
    Player* playerCopy = logic.currentTurn;
    logic.MakeMove(3,2); // Arbitrary location
    // Should NOT change anything
    QVERIFY(playerCopy == logic.currentTurn);
    QVERIFY(logic.board[3][2] == 'O');
}


// AC 4.4
void MyUnitTests::Board_WhenOOBCellClicked_Simple_IsIgnored()
{
    GameLogic logic {5, 0};
    Player* playerCopy = logic.currentTurn;
    //qDebug()  << playerCopy << " : " << logic.currentTurn;
    try {
        logic.MakeMove(99,99);
    }
    catch (const std::out_of_range& oor) {
        QFAIL("Illegal OOB move in simple game was not handled");
    }
    //qDebug()  << playerCopy << " : " << logic.currentTurn;
    QVERIFY(playerCopy == logic.currentTurn);
}

// TO-DO: Implement a finished game state
// AC 5.1
void MyUnitTests::BoardAndPlayer_WhenResetButtonClicked_Simple_IsSet()
{
    GameLogic logic = {5, 0};
    Player* playerCopy = logic.currentTurn;

    logic.MakeMove(0,0);
    logic.SetBoardSize(5); // Reset button shouldn't have a different board size

    QVERIFY(logic.board[0][0] == 'W');
    QVERIFY(playerCopy == logic.currentTurn);
}

// TO-DO: Implement a replay function so this can be made
// AC 5.2 (WILL FAIL)
void MyUnitTests::Replay_WhenReplayButtonClicked_Simple_IsPlayed()
{
    QFAIL("Replay functionality not yet implemented");
}

// AC 6.1
void MyUnitTests::BoardAndPlayer_WhenEmptyCellClickedNoMatch_General_IsSet()
{
    GameLogic logic {5, 1};
    Player* playerCopy = logic.currentTurn;
    logic.MakeMove(0,0); // S player makes move at button (0, 0)
    QVERIFY(logic.board[0][0] == 'S');
    QVERIFY(playerCopy != logic.currentTurn);
}

// AC 6.2
void MyUnitTests::Board_WhenFilledCellClicked_General_IsIgnored()
{
    GameLogic logic {5, 1};
    logic.board[0][0] = 'O';

    Player* playerCopy = logic.currentTurn;
    //qDebug()  << playerCopy << " : " << logic.currentTurn;
    logic.MakeMove(0,0);
    //qDebug()  << playerCopy << " : " << logic.currentTurn;
    // Nothing should have changed, making the move ignored.
    QVERIFY(playerCopy == logic.currentTurn);
    QVERIFY(logic.board[0][0] == 'O');
}


// TO-DO: Implement a finished state to game logic
// AC 6.3
void MyUnitTests::Board_WhenAnyCellClickedFinished_General_IsIgnored()
{
    GameLogic logic {5, 1};
    logic.board[3][2] = 'O';
    //<- Set game to finished here ->
    Player* playerCopy = logic.currentTurn;
    logic.MakeMove(3,2); // Arbitrary location
    // Should NOT change anything
    QVERIFY(playerCopy == logic.currentTurn);
    QVERIFY(logic.board[3][2] == 'O');
}

// TO-DO: Implement general game logic
// AC 6.4 (WILL FAIL)
void MyUnitTests::Board_WhenEmptyCellClickedMakesMatch_General_IsSet()
{
    GameLogic logic {5, 1};
    logic.board[0][0] = 'S';
    logic.board[0][1] = 'O';

    Player* playerCopy = logic.currentTurn;
    //qDebug()  << playerCopy << " : " << logic.currentTurn;
    logic.MakeMove(0,2); // Should make an SOS match
    //qDebug()  << playerCopy << " : " << logic.currentTurn; // This should not be changed, since a match was made
    QVERIFY(logic.board[0][2] == 'S');
    QVERIFY(playerCopy == logic.currentTurn);
}

// TO-DO: Implement a finished game state
// AC 7.1
void MyUnitTests::Board_WhenResetButtonClicked_General_IsSet()
{
    GameLogic logic = {5, 1};
    Player* playerCopy = logic.currentTurn;

    logic.MakeMove(0,0);
    logic.SetBoardSize(5); // Reset button shouldn't have a different board size

    QVERIFY(logic.board[0][0] == 'W');
    QVERIFY(playerCopy == logic.currentTurn);
}

// TO-DO: Implement a replay function so this can be made
// AC 7.2 (WILL FAIL)
void MyUnitTests::Replay_WhenReplayButtonClicked_General_IsPlayed()
{
    QFAIL("Replay functionality not yet implemented");
}

QTEST_APPLESS_MAIN(MyUnitTests)

#include "tst_myunittests.moc"
