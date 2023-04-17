#include "mainwindow.h"

#include <QApplication>

/// @brief open UI screen
/// @param argc 
/// @param argv 
/// @return 
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
