/**
*@file mainwindow.cpp
*Fichier du projet éditeur de texte en QT
*@author Foulon Edouard
*@date 06/09/2023
*@Version 1.0
*/

#include "mainwindow.h"       //Appels Headers
#include "ui_mainwindow.h"
#include<QDebug>              //Ecriture sur QTcreator
#include<QString>             //Utilisation chaîne de caractère
#include<QTabWidget>          //Classe héritant de QWidget pour la gestion des Widget sur l'ui
#include<QMessageBox>         //Ecriture  de message sur l'interface graphique
#include<QTextEdit>           //Zone de texte sur l'interface graphique
#include<QTextStream>         //Reception de flux pour lire un fichier texte
#include<QFile>               //Pour ouverture/fermeture de fichier
#include<QFileDevice>         //Classe parent de QFile qui va gérer lecture/écriture  des fichiers
#include<QTextCursor>         //Création d'un suivit sur le curseur de la souris pour des lectures sur l'interface graphique
#include<QInputDialog>        //Permet l'ouverture d'une fenetre sur l'interface graphique

/**
 * @brief
 * Le gros du programme est ici, on définit ici les méthodes a utilisé sur l'interface graphique en fonction des connects.:\n
 * -Appel connect.\n
 * -Fonction ouverture/lecture fichier .\n
 * -Fonction sauvegarde de fichier.\n
 * -Fonction affichage ligne/colonnes.\n
 * -Fonction recherche de mot.\n
 */

/**
*Paramétrage de la MainWindow:\n
*/


MainWindow::MainWindow(QWidget *parent)  //Constructeur de la classe MainWindow
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);  //Génére l'interface graphique

    ui->tabWidget->setMovable(true);       //Permet de déplacer les onglets
    ui->tabWidget->setTabsClosable(true);   //Ajoute des boutons croix de fermeture sur les onglets

    connect(ui->actionOuvrir, SIGNAL(triggered()),this,SLOT(ouvrir_fichier()));    //Connect sur "ouvrir" dans fichier
    connect(ui->actionSauvegarder,SIGNAL(triggered()),this,SLOT(save()));          //Connect sur "Sauvergarder" dans fichier
    connect(ui->actionLigne_colonne,SIGNAL(triggered()),this,SLOT(affiche_ligne_colonne()));  //Connect sur "Ligne_colonne" dans affichage
    connect(ui->actionOuvrir_fenetre_recherche,SIGNAL(triggered()),this,SLOT(recherche()));   //Connect sur "Ouvrir fenetre recherche" dans recherche
}

/**
*Méthode pour ouvrir un fichier et créer un nouvel onglet si l'onglet a  déjà un fichier en lecture:\n
*Pas encore au point: L'ouverture se fait bien dans les onglets Tab1 et Tab2.\n
*Et si on essaie d'ouvrir un fichier alors que tab2 en possède déjà un alors un nouvel onglet sera bien créer avec le nouveau fichier.\n
*Mais si on veut ouvrir un fichier depuis un des nouveaux onglets l'ui va planter.\n
*Cela vient du fait que mon currentTextEdit se génére à chaque onglet que j'ouvre et reste égal.\n
*/

void MainWindow::ouvrir_fichier()
{

    int index = ui->tabWidget->currentIndex();    //récupére l'index de l'onglet
    QWidget *currentWidget = ui->tabWidget->currentWidget();  //Nous place dans le widget ouvert sur l'ui
    QTextEdit *currentTextEdit = currentWidget->findChild<QTextEdit *>();  //Définit une zone de TextEdit courante dans le widget courrant

    QString chemin=QFileDialog::getOpenFileName();  //Permet d'aller cherche un fichier dans nos répertoires
    QString fileName = chemin.split(u'/').last();  //Récupère le nom du fichier


           if(currentTextEdit->toPlainText() != ""){   //Condition si l'onglet possède déjà du texte

               QTextEdit *i=new(QTextEdit);
               ui->tabWidget->addTab(i, fileName);    //Ajout nouvel onglet
               ui->tabWidget->setCurrentIndex(index+1); //Nous place dans l'onglet suivant qui est ce nouvel onglet

               QFile f(chemin);     //ouverture fichier
               if(!f.open(QIODevice::ReadOnly))   //Lecture du fichier
                   QMessageBox::information(0,"info",f.errorString());    //Si erreur lors de la sélection du fichier
               QTextStream  in(&f);  //Imprimme le fichier texte dans un QTextStream
              i->setText(in.readAll());  //Lit le flux du QTextStream et le met dans l'onglet

               f.close();  //Fermeture fichier
           }

           else {     //Condition l'onglet est vide
                ui->tabWidget->setTabText(index,fileName);
                QFile f(chemin);
                if(!f.open(QIODevice::ReadOnly))
                   QMessageBox::information(0,"info",f.errorString());
                QTextStream  in(&f);
                currentTextEdit->setText(in.readAll());  //Cette fois on ajoute juste le texte dans l'onglet qui était vide

                f.close();}

    qDebug()<<"on ouvre le fichier"<<fileName; //Indication sur QtCreator du nom du fichier ouvert
}

/**
*Méthode pour sauvegardé un fichier :\n
*La sauvegarde se fait en cliquant sur sauvegarde dans fichier en sélectionnant le fichier où l'on veut sauvegarder.\n
*Pas encore au point : suite aux soucis des nouveaux onglets en lecture la sauvegarde ne marche que sur les tab1 et tab2\n
*Si on veut sauvegarder depuis les nouveaux onglets l'ui va planter\n
*/


void MainWindow::save()
{
       QString chemin=QFileDialog::getOpenFileName();
       QString fileName=chemin.split(u'/').last();

        QFile f(fileName);
        f.open(QIODevice::ReadWrite);  //Cette fois  en mode Lecture/Ecrituure
        QTextStream stream( &f );
        QTextEdit* currentTextEdit = ui->tabWidget->currentWidget()->findChild<QTextEdit *>();
        QString text = currentTextEdit->toPlainText();

            stream << text ; //récupère le nouveau texte dans le flux QTextStream

}

/**
*Méthode pour affichier la ligne et la colonne ou se situe le curseur :\n
*Une fois le curseur placé on clique sur Ligne_colonne dans affichage\n
*Le choix a été fait de définir la position initiale du curseur comme étant ligne 1 colonne 1; pas de ligne et colonne 0.\n
*Pas encore au point : suite aux soucis des nouveaux onglets en lecture l'affichage ne marche que sur les tab1 et tab2\n
*Si on veut afficher depuis les nouveaux onglets l'ui va planter\n
*/


void MainWindow::affiche_ligne_colonne()
{int ligne;
 int colonne;
    QWidget *currentWidget = ui->tabWidget->currentWidget();
    QTextEdit *currentTextEdit = currentWidget->findChild<QTextEdit *>();
    QTextCursor curseur=currentTextEdit->textCursor(); //Création du curseur relié au curseur souris
    ligne=curseur.blockNumber()+1;  //Pour obtenir le numéro de la ligne
    colonne=curseur.columnNumber()+1; //Pour obtenir le numéro de la colonne

    ui->statusbar->showMessage(tr("Ligne %1  Colonnne  %2").arg(ligne).arg(colonne)); //Affichage ligne et colonne dans la statusbar en bas de l'ui
}

/**
*Méthode pour rechercher un mot dans le texte en cliquand sur ouvrir fenetre recherche dans Recherche et en  tapant le mot :\n
*La méthode va indiquer le premier mot correspondant et une erreur si le mot n'est pas présent dans le text\n
*Pas encore au point : suite aux soucis des nouveaux onglets en lecture la recherche ne marche que sur les tab1 et tab2\n
*Si on veut rechercher depuis les nouveaux onglets l'ui va planter\n
*/


void MainWindow::recherche()
{
    QWidget *currentWidget = ui->tabWidget->currentWidget();
    QTextEdit *currentTextEdit = currentWidget->findChild<QTextEdit *>();
    QString text = QInputDialog::getText(this, tr("Rechercher"), tr("Entrez le mot a chercher:")); //Ouvvre une fenêtre ou on entre le mot a chercher
    currentTextEdit->moveCursor(QTextCursor::Start); //On part du début
    QTextCursor chercher = currentTextEdit->document()->find(text, currentTextEdit->textCursor(), QTextDocument::FindWholeWords);//création curseur recherche mot entier
            if(chercher.isNull()) { //Condition le mot n'est pas dans le texte
                QMessageBox::information(this, tr("Rechercher"), tr("L'expression \"%1\" n'apparaît pas dans le texte").arg(text));
            } else {  //Le mot est trouvé
                currentTextEdit->setTextCursor(chercher);
            }
    }
/**
*Connect fait avec un on_ pour fermer les onglets en cliquant sur la croix.\nn
*Pas encore au point:Ne me déteste pas Gilles pour le on_ celui me paraissait bien plus simple a effectué que les autres avec cette méthode!
*/


void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}



MainWindow::~MainWindow() //Destructeur MainWindow
{
    delete ui;
}



