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

    int index = ui->tabWidget->currentIndex();
    QWidget *currentWidget = ui->tabWidget->currentWidget();
    QTextEdit *currentTextEdit = currentWidget->findChild<QTextEdit *>();

    QString chemin=QFileDialog::getOpenFileName();
    QString fileName = chemin.split(u'/').last();


           if(currentTextEdit->toPlainText() != ""){
               QTextEdit *i=new(QTextEdit);

               ui->tabWidget->addTab(i, fileName);
               ui->tabWidget->setCurrentIndex(index+1);

               QFile f(chemin);
               if(!f.open(QIODevice::ReadOnly))
                   QMessageBox::information(0,"info",f.errorString());
               QTextStream  in(&f);
              i->setText(in.readAll());

               f.close();
           }

           else {
                ui->tabWidget->setTabText(index,fileName);
                QFile f(chemin);
                if(!f.open(QIODevice::ReadOnly))
                   QMessageBox::information(0,"info",f.errorString());
                QTextStream  in(&f);
                currentTextEdit->setText(in.readAll());

                f.close();}

    qDebug()<<"on ouvre le fichier"<<fileName;
}


void MainWindow::creer_onglet()
{
}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}
