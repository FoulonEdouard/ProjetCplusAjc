#include "mainwindow.h"
#include <QApplication>
#include<QFile>
#include<QFileDialog>
#include<QTabBar>
#include<QTextDocument>
#include<QDebug>
#include<QString>
#include<QIODevice>
#include<QDir>
#include<QTabWidget>
#include<QTextCursor>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MainWindow w;

    w.show();
    return a.exec();
}
