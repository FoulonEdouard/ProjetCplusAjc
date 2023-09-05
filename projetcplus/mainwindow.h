#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QFile>
#include <QApplication>
#include<QFile>
#include<QFileDialog>
#include<QTabBar>
#include<QTextDocument>
#include<QDebug>
#include<QString>
#include<QIODevice>
#include <QMainWindow>
#include<QDir>
#include<QTabWidget>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void ouvrir_fichier();
    void creer_onglet();



    void on_tabWidget_tabCloseRequested(int index);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
