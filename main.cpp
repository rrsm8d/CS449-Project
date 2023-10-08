#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // Creates the application object for us
    MainWindow w;
    w.show(); // Show it on the screen
    return a.exec(); // Put application onto a loop, where our application is handled
}
