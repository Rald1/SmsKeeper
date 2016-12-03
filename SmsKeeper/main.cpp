#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mainWindow;
    if (!mainWindow.isBaseOpen())
        return 1;

    mainWindow.show();

    return a.exec();
}
