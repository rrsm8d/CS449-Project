#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gamelogic.h"
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
    GameLogic logic = {7, 2}; // Not sure why, but the program wants me to create the object like this.

private slots: // Executed whenever a signal is submitted

    void ResizeBoard(int i); // Change the button layout within the grid
    void ClearBoard(); // Remove all buttons within the grid
    void ChangeState(); // Adding X/O to buttons
    void ResetBoard(); // Clearing values on cells

    void SetSimpleGame(bool checked); // Set game logic.
    void SetGeneralGame(bool checked); // Set game logic.
    void SetPlayerLetter(bool checked); // Update gamelogic
};
#endif // MAINWINDOW_H
