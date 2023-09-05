#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setMovable(true);
    ui->tabWidget->setTabsClosable(true);


    connect(ui->actionOuvrir, SIGNAL(triggered()),this,SLOT(ouvrir_fichier()));
    connect(ui->actionNew,SIGNAL(triggered()),this,SLOT(creer_onglet()));

}

void MainWindow::ouvrir_fichier()
{//creation objet QFile

    QString chemin=QFileDialog::getOpenFileName();
    QString courant=QDir::currentPath();
    QFile f(chemin);
    if(!f.open(QIODevice::ReadOnly))
        QMessageBox::information(0,"info",f.errorString());
    QTextStream in(&f);
    ui->textEdit->setText(in.readAll());

    f.close();

    qDebug()<<"on ouvre le fichier"<<chemin;
}

void MainWindow::creer_onglet()
{
    ui->tabWidget->addTab(new QTextEdit,"nouvel onglet");
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}
