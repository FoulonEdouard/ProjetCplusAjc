#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMessageBox>
#include<QTextEdit>
#include<QTextStream>
#include<QFileDevice>
#include<QByteArray>



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


    QWidget *currentWidget = ui->tabWidget->currentWidget();
    QTextEdit *currentTextEdit = currentWidget->findChild<QTextEdit *>();

    QString chemin=QFileDialog::getOpenFileName();
    QString fileName = chemin.split(u'/').last();
    QFile f(chemin);
    if(!f.open(QIODevice::ReadOnly))
        QMessageBox::information(0,"info",f.errorString());
     QTextStream  in(&f);
    currentTextEdit->setText(in.readAll());

    f.close();

    qDebug()<<"on ouvre le fichier"<<fileName;
}

void MainWindow::creer_onglet()
{

    ui->tabWidget->addTab(new(QTextEdit),"nouvel onglet");
    ui->tabWidget->setCurrentWidget(this);

    ouvrir_fichier();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}
