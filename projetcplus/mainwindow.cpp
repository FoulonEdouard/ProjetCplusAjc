#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMessageBox>
#include<QTextEdit>
#include<QTextStream>
#include<QFileDevice>
#include<QTextCursor>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->tabWidget->addTab(new QTextEdit(this),"essai");
    ui->tabWidget->setMovable(true);
    ui->tabWidget->setTabsClosable(true);

    connect(ui->actionOuvrir, SIGNAL(triggered()),this,SLOT(ouvrir_fichier()));
    connect(ui->actionSauvegarder,SIGNAL(triggered()),this,SLOT(save()));
    connect(ui->actionLigne_colonne,SIGNAL(triggered()),this,SLOT(affiche_ligne_colonne()));

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

void MainWindow::save()
{
       QString chemin=QFileDialog::getOpenFileName();
       QString fileName=chemin.split(u'/').last();

        QFile f(fileName);
        f.open(QIODevice::ReadWrite);
        QTextStream stream( &f );
        QTextEdit* currentTextEdit = ui->tabWidget->currentWidget()->findChild<QTextEdit *>();
        QString text = currentTextEdit->toPlainText();

            stream << text ;

}

void MainWindow::affiche_ligne_colonne()
{int ligne;
 int colonne;
    QWidget *currentWidget = ui->tabWidget->currentWidget();
    QTextEdit *currentTextEdit = currentWidget->findChild<QTextEdit *>();
    QTextCursor curseur=currentTextEdit->textCursor();
    ligne=curseur.blockNumber()+1;
    colonne=curseur.columnNumber()+1;

    ui->statusbar->showMessage(tr("Ligne %1  Colonnne  %2").arg(ligne).arg(colonne));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}
