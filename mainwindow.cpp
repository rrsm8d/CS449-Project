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
    QRadioButton *p1HumanRadio = MainWindow::findChild<QRadioButton *>("p1HumanRadio");
    connect(p1HumanRadio, SIGNAL(toggled(bool)), this, SLOT(SetPlayerCpu(bool)));
    QRadioButton *p1ComputerRadio = MainWindow::findChild<QRadioButton *>("p1ComputerRadio");
    connect(p1ComputerRadio, SIGNAL(toggled(bool)), this, SLOT(SetPlayerCpu(bool)));


    QRadioButton *p2SRadio = MainWindow::findChild<QRadioButton *>("p2SRadio");
    connect(p2SRadio, SIGNAL(toggled(bool)), this, SLOT(SetPlayerLetter(bool)));
    QRadioButton *p2ORadio = MainWindow::findChild<QRadioButton *>("p2ORadio");
    connect(p2ORadio, SIGNAL(toggled(bool)), this, SLOT(SetPlayerLetter(bool)));
    QRadioButton *p2HumanRadio = MainWindow::findChild<QRadioButton *>("p2HumanRadio");
    connect(p2HumanRadio, SIGNAL(toggled(bool)), this, SLOT(SetPlayerCpu(bool)));
    QRadioButton *p2ComputerRadio = MainWindow::findChild<QRadioButton *>("p2ComputerRadio");
    connect(p2ComputerRadio, SIGNAL(toggled(bool)), this, SLOT(SetPlayerCpu(bool)));

    QPushButton *resetButton = MainWindow::findChild<QPushButton *>("resetButton");
    connect(resetButton, SIGNAL(released()), this, SLOT(ResetBoard()));

    QPushButton *debugButton = MainWindow::findChild<QPushButton *>("debugButton");
    connect(debugButton, SIGNAL(released()), this, SLOT(PrintDebugStats()));

    // Initialize the first grid on startup
    ResizeBoard(boardSizeSelector->value());
}

MainWindow::~MainWindow()
{
    delete ui;
}

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
    this->logic->SetBoardSize(size);
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
    // Special case when both players are CPU
    if(this->logic->player1.isCpu && this->logic->player2.isCpu)
    {
        while(!this->logic->isFinished)
            this->logic->CpuMove();
    }
    UpdateBoard();
}

void MainWindow::UpdateBoard()
{
    // First, iterate through all buttons and update their text
    QGridLayout *grid = MainWindow::findChild<QGridLayout *>("gridLayout");
    QLayoutItem *item;
    for (int row = 0; row < this->logic->board.size(); ++row)
    {
        for (int column = 0; column < this->logic->board.size(); ++column)
        {
            QWidget* widget = grid->itemAtPosition(row, column)->widget();
            // Check if the widget is a QPushButton, so nothing funky happens
            if (widget && qobject_cast<QPushButton*>(widget))
            {
                QPushButton* button = qobject_cast<QPushButton*>(widget);
                QChar value = this->logic->board[row][column];
                button->setText(value);
            }
        }
    }

    // Update turnLabel
    QLabel *turnLabel = MainWindow::findChild<QLabel *>("turnLabel");
    QString labelText = QString::fromStdString(this->logic->currentTurn->playerColor + " 's turn");
    turnLabel->setText(labelText);

    if(logic->isFinished)
    {
        Player* winner = logic->DetermineWinner();
        if(winner != nullptr)
        {
            labelText = QString::fromStdString(winner->playerColor + " has won. Score: " + std::to_string(winner->playerScore));
        } else
        {
            labelText = "Its a tie!";
        }
        turnLabel->setText(labelText);
        DisableBoard();
    }
}

void MainWindow::ChangeState()
{
    QPushButton* button = (QPushButton *)sender();
    // Still not sure how to directly access the position values of the grid, may update these index later
    int indexX = button->objectName()[0].digitValue(); // .digitValue() converts from QChar to int. Temporary until a better way is found
    int indexY = button->objectName()[1].digitValue();

    // Verify this button is not out of bounds (This should never happen?)
    if(!this->logic->isValidCell(indexX, indexY))
    {
        button = nullptr;
        return;
    }

    // If the button even exists...
    if (button) {
         // If this is a valid spot to place a letter AND at least one player is human
        if(this->logic->isEmptyCell(indexX, indexY) && !(this->logic->player1.isCpu && this->logic->player2.isCpu))
        {
            // Update logic
            this->logic->MakeMove(indexX, indexY);
            qDebug() << "Assigned value to " << button->objectName();

            // A CPU may have made a move too. Update the buttons that it makes moves to
            if(this->logic->currentTurn->isCpu)
                this->logic->CpuMove();

        } else qDebug() << button->objectName() << " already has a value!";

        // Special case when both players are CPU
        if(this->logic->player1.isCpu && this->logic->player2.isCpu)
        {
            while(!this->logic->isFinished)
                this->logic->CpuMove();
        }
    }
    button = nullptr;
    UpdateBoard();
}

void MainWindow::SetSimpleGame(bool checked)
{
    // If button even exists
    if(checked)
    {
        // Reset board and remake it
        QSpinBox *boardSizeSelector = MainWindow::findChild<QSpinBox *>("sizeSpinBox");
        int boardSize = boardSizeSelector->value();
        bool p1copy = this->logic->player1.isCpu;
        bool p2copy = this->logic->player2.isCpu;
        ResizeBoard(boardSize);
        delete logic;
        logic = new SimpleGame(boardSize);
        this->logic->player1.isCpu = p1copy;
        this->logic->player2.isCpu = p2copy;
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
        int boardSize = boardSizeSelector->value();
        bool p1copy = this->logic->player1.isCpu;
        bool p2copy = this->logic->player2.isCpu;
        ResizeBoard(boardSize);
        delete logic;
        logic = new GeneralGame(boardSize);
        this->logic->player1.isCpu = p1copy;
        this->logic->player2.isCpu = p2copy;
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
            this->logic->player1.playerLetter = "S";
            this->logic->player2.playerLetter = "O";
            ResetBoard();
        } else if(button->objectName() == "p1ORadio")
        {
            QRadioButton *p2Radio = MainWindow::findChild<QRadioButton *>("p2SRadio");
            p2Radio->setChecked(!p2Radio->isChecked());
            p2Radio = nullptr;
            this->logic->player1.playerLetter = "O";
            this->logic->player2.playerLetter = "S";
            ResetBoard();
        } else if(button->objectName() == "p2SRadio")
        {
            QRadioButton *p1Radio = MainWindow::findChild<QRadioButton *>("p1ORadio");
            p1Radio->setChecked(!p1Radio->isChecked());
            p1Radio = nullptr;
            this->logic->player1.playerLetter = "O";
            this->logic->player2.playerLetter = "S";
            ResetBoard();
        } else if(button->objectName() == "p2ORadio")
        {
            QRadioButton *p1Radio = MainWindow::findChild<QRadioButton *>("p1SRadio");
            p1Radio->setChecked(!p1Radio->isChecked());
            p1Radio = nullptr;
            this->logic->player1.playerLetter = "S";
            this->logic->player2.playerLetter = "O";
            ResetBoard();
        } else qDebug() << "Critical error, signal from unknown QRadioButton: " << button->objectName();
    }
}

void MainWindow::SetPlayerCpu(bool checked)
{
    if(checked)
    {
        QRadioButton* button = (QRadioButton *)sender();
        // NOT a pretty way of implementing this. Need to update later
        if(button->objectName() == "p1HumanRadio")
        {
            this->logic->SetPlayerHuman(1);
            ResetBoard();
        } else if(button->objectName() == "p1ComputerRadio")
        {
            this->logic->SetPlayerCpu(1);
            ResetBoard();
        } else if(button->objectName() == "p2HumanRadio")
        {
            this->logic->SetPlayerHuman(2);
            ResetBoard();
        } else if(button->objectName() == "p2ComputerRadio")
        {
            this->logic->SetPlayerCpu(2);
            ResetBoard();
        } else qDebug() << "Critical error, signal from unknown QRadioButton: " << button->objectName();
    }
}

void MainWindow::PrintDebugStats()
{
    for(auto &vec: this->logic->board)
    {
        for(char &c : vec)
        {
            std::cout << c << ", ";
        }
        std::cout << std::endl;
    }
    //QPushButton* button = (QPushButton *)sender();
    //button->setEnabled(false);
    DisableBoard();
}

void MainWindow::DisableBoard()
{
    QGridLayout *grid = MainWindow::findChild<QGridLayout *>("gridLayout");
    QLayoutItem *item;
    // Iterate through each button in the grid, disable them
    for (int row = 0; row < this->logic->board.size(); ++row) {
        for (int column = 0; column < this->logic->board.size(); ++column) {
            QWidget* widget = grid->itemAtPosition(row, column)->widget();

            if (widget && qobject_cast<QPushButton*>(widget)) {
                // Check if the widget is a QPushButton
                QPushButton* button = qobject_cast<QPushButton*>(widget);
                button->setEnabled(false);
            }
        }
    }
}
