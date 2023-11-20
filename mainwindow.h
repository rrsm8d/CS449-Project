#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gamelogic.h"
#include "simplegame.h"
#include "generalgame.h"
#include <QMainWindow>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    GameLogic* logic = new SimpleGame(4);

private slots: // Executed whenever a signal is submitted

    void ResizeBoard(int i); // Change the button layout within the grid
    void ClearBoard(); // Remove all buttons within the grid
    void ChangeState(); // Adding X/O to buttons
    void ResetBoard(); // Clearing values on cells
    void DisableBoard(); // Disable buttons on a finished game
    void UpdateBoard(); // Update the text on buttons and turn label

    void SetSimpleGame(bool checked); // Set game logic.
    void SetGeneralGame(bool checked); // Set game logic.
    void SetPlayerLetter(bool checked); // Update gamelogic
    void SetPlayerCpu(bool checked); // Change player's controller

    void PrintDebugStats(); // debug
};
#endif // MAINWINDOW_H
