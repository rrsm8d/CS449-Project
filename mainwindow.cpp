#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
      QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Connect a bunch of widgets to signals and slots
    QSpinBox *boardSizeSelector = MainWindow::findChild<QSpinBox *>("sizeSpinBox");
    connect(boardSizeSelector, SIGNAL(valueChanged(int)), this, SLOT(ResizeBoard(int)));

    QRadioButton *simpleGameRadio = MainWindow::findChild<QRadioButton *>("simpleGameRadio");
    connect(simpleGameRadio, SIGNAL(toggled(bool)), this, SLOT(SetSimpleGame(bool)));
    QRadioButton *generalGameRadio = MainWindow::findChild<QRadioButton *>("generalGameRadio");
    connect(generalGameRadio, SIGNAL(toggled(bool)), this, SLOT(SetGeneralGame(bool)));

    QRadioButton *p1SRadio = MainWindow::findChild<QRadioButton *>("p1SRadio");
    connect(p1SRadio, SIGNAL(toggled(bool)), this, SLOT(SetPlayerLetter(bool)));
    QRadioButton *p1ORadio = MainWindow::findChild<QRadioButton *>("p1ORadio");
    connect(p1ORadio, SIGNAL(toggled(bool)), this, SLOT(SetPlayerLetter(bool)));
    QRadioButton *p2SRadio = MainWindow::findChild<QRadioButton *>("p2SRadio");
    connect(p2SRadio, SIGNAL(toggled(bool)), this, SLOT(SetPlayerLetter(bool)));
    QRadioButton *p2ORadio = MainWindow::findChild<QRadioButton *>("p2ORadio");
    connect(p2ORadio, SIGNAL(toggled(bool)), this, SLOT(SetPlayerLetter(bool)));

    QPushButton *resetButton = MainWindow::findChild<QPushButton *>("resetButton");
    connect(resetButton, SIGNAL(released()), this, SLOT(ResetBoard()));

    // Initialize the first grid on startup
    ResizeBoard(boardSizeSelector->value());
}

MainWindow::~MainWindow()
{
    delete ui;
}

// TO-DO: Reset turn order on resize (Blue should always go first)
void MainWindow::ResizeBoard(int size)
{
    // Get rid of the current buttons first
    ClearBoard();
    QWidget *widget = MainWindow::findChild<QWidget *>("centralWidget");
    QGridLayout *grid = MainWindow::findChild<QGridLayout *>("gridLayout");
    // Make a new set of buttons using the given size
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            QPushButton *newButton = new QPushButton(widget);
            // Easy to debug name, 00, 01, etc...
            newButton->setObjectName(std::to_string(i)+std::to_string(j));
            connect(newButton, SIGNAL(released()), this, SLOT(ChangeState()));
            newButton->setText("_");
            // Add the button to the grid layout at position (i,j)
            grid->addWidget(newButton, i, j);
            newButton = nullptr;
        }
    }
    qDebug() << "The board has been resized to: " << size;
    widget = nullptr;
    grid = nullptr;
    // Adjust logical code
    this->logic.SetBoardSize(size);
    //this->logic.ClearBoard();
}

void MainWindow::ClearBoard()
{
    QGridLayout *grid = MainWindow::findChild<QGridLayout *>("gridLayout");
    // The following was made with help by GPT-3.5
    // Iterate through each widget in the grid, and delete them
    QLayoutItem *item;
    while ((item = grid->takeAt(0))) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
    qDebug() << "The board has been cleared";
    item = nullptr;
    grid = nullptr;
}

void MainWindow::ResetBoard()
{
    // Function only used by reset button
    QSpinBox *boardSizeSelector = MainWindow::findChild<QSpinBox *>("sizeSpinBox");
    ResizeBoard(boardSizeSelector->value());
    boardSizeSelector = nullptr;
}

void MainWindow::ChangeState()
{
    QPushButton* button = (QPushButton *)sender();
    // Still not sure how to directly access the position values of the grid, may update these index later
    int indexX = button->objectName()[0].digitValue(); // .digitValue() converts from QChar to int. Temporary until a better way is found
    int indexY = button->objectName()[1].digitValue();

    // Verify this button is not out of bounds (This should never happen?)
    if(!this->logic.isValidCell(indexX, indexY))
    {
        button = nullptr;
        return;
    }

    // If the button even exists...
    if (button) {
         // If this is a valid spot to place a letter
        if(this->logic.isEmptyCell(indexX, indexY))
        {
            // Grab the players letter from logic code, set it to the button
            QString playerLetter = QString::fromStdString(this->logic.currentTurn->playerLetter);
            button->setText(playerLetter);
            // Update logic
            this->logic.MakeMove(indexX, indexY);
            qDebug() << "Assigned value to " << button->objectName() << ", switching turns"; // Change debug message when game logic is finally properly implemented
            // Update turnLabel
            QLabel *turnLabel = MainWindow::findChild<QLabel *>("turnLabel");
            QString labelText = QString::fromStdString(this->logic.currentTurn->playerColor + " 's turn");
            turnLabel->setText(labelText);
        } else qDebug() << button->objectName() << " already has a value!";
    }
    button = nullptr;
}

void MainWindow::SetSimpleGame(bool checked)
{
    // If button even exists
    if(checked)
    {
        // Reset board and remake it
        QSpinBox *boardSizeSelector = MainWindow::findChild<QSpinBox *>("sizeSpinBox");
        ResizeBoard(boardSizeSelector->value());
        this->logic.SetGameMode(0);
        qDebug() << "The game mode has been set to simple";
        boardSizeSelector = nullptr;
    }
}

void MainWindow::SetGeneralGame(bool checked)
{
    // If button even exists
    if(checked)
    {
        // Reset board and remake it
        QSpinBox *boardSizeSelector = MainWindow::findChild<QSpinBox *>("sizeSpinBox");
        ResizeBoard(boardSizeSelector->value());
        this->logic.SetGameMode(1);
        qDebug() << "The game mode has been set to general";
        boardSizeSelector = nullptr;
    }
}

// TO-DO: Improve this ugly repetitive function
void MainWindow::SetPlayerLetter(bool checked)
{
    if(checked)
    {
        QRadioButton* button = (QRadioButton *)sender();
        // NOT a pretty way of implementing this. Need to update later
        if(button->objectName() == "p1SRadio")
        {
            QRadioButton *p2Radio = MainWindow::findChild<QRadioButton *>("p2ORadio");
            p2Radio->setChecked(!p2Radio->isChecked());
            p2Radio = nullptr;
            this->logic.player1.playerLetter = "S";
            this->logic.player2.playerLetter = "O";
            ResetBoard();
        } else if(button->objectName() == "p1ORadio")
        {
            QRadioButton *p2Radio = MainWindow::findChild<QRadioButton *>("p2SRadio");
            p2Radio->setChecked(!p2Radio->isChecked());
            p2Radio = nullptr;
            this->logic.player1.playerLetter = "O";
            this->logic.player2.playerLetter = "S";
            ResetBoard();
        } else if(button->objectName() == "p2SRadio")
        {
            QRadioButton *p1Radio = MainWindow::findChild<QRadioButton *>("p1ORadio");
            p1Radio->setChecked(!p1Radio->isChecked());
            p1Radio = nullptr;
            this->logic.player1.playerLetter = "O";
            this->logic.player2.playerLetter = "S";
            ResetBoard();
        } else if(button->objectName() == "p2ORadio")
        {
            QRadioButton *p1Radio = MainWindow::findChild<QRadioButton *>("p1SRadio");
            p1Radio->setChecked(!p1Radio->isChecked());
            p1Radio = nullptr;
            this->logic.player1.playerLetter = "S";
            this->logic.player2.playerLetter = "O";
            ResetBoard();
        } else qDebug() << "Critical error, signal from unknown QRadioButton: " << button->objectName();
    }
}
