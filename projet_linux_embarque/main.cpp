/**
*@file main.cpp
*Fichier principal du Projet_Linux_Embarqué sur un OpenData
*@author Foulon Edouard
*@date 13/09/2023
*@Version 1.0
*/

#include"loader.h"                //Classe contenant l'appel avec Curl
#include"nlohmann/json.hpp"       //Librairie utilisée pour utilisé les fichiers JSON
#include<fstream>                 //Récupération flux de données
#include<iostream>                //Librairie standart pour les entrées/sorties
#include"gd.h"                    //Librairie graphique utilisée
#include <gdfontl.h>              //Va avec GD
#include"convertepoch.h"          //Classe contennant 2 méthodes pour passer une date en epoch et inversement
#include<algorithm>               //Librairie pour utiliser sort()
#include<vector>                  //Connteneur vector qui sera utilisé conjointement avec le JSON
#include<math.h>                  //Pour l'arrondi sur les floats


/**
 * @brief
 * Le but de ce programme est à travers la récupération d'un Url en OpenData d'obtenir des graphiques en C/C++:\n
 * L'Url selectionné ici sera :https://www.data.gouv.fr/fr/datasets/r/ecb94e56-22d4-40ee-bf40-c7890569d5bd.\n
 * Il s'agit d'un OpenData provenant de l'entreprise ENEDIS qui met en valeurs les différences de consommations électriques selon la température en France.\n
 * Il y aura 3 populations étudiées dans ces données:Résidents,Professionnels et Entreprises .\n
 * Les termes Tr et Tn feront respectivement références à :Température réelle et Température normale(basée sur les 30 dernières années hors aléa climatique)\n
 * \n
 * Ce programme est écrit pour fonctionner sous Debian et sous Buildroot avec unee carte qemu.\n
 * La première partie du programme va récupérer les informations du fichier JSON à l'Url demandée.\n
 * La 2ème partie est un prétravail sur le JSON pour récupérée les données trié et certains calculs déjà effectués en vu des graphiques.\n
 * La dernière partie correspond aux différents graphiques avec la librairie GD.\n
 *
 * Les commandes de compilations utilisées:\n
 * Debian :g++ -o proggdebian main.cpp loader.cpp convertepoch.cpp -lcurl -lgd -lpng -lm -lstdc++fs
 * Pour la qemu:/home/edouard/buildroot-2023.08/output/host/bin/aarch64-linux-g++ -o progarmgd main.cpp loader.cpp convertepoch.cpp -lcurl -lgd -lpng -lm -lstdc++fs

 */

//Utilisations namespaces pour simplifié le programme
using json = nlohmann::json;
using namespace std;


int main(int argc, char** argv) {

    //Appel classe Loader pour récupérer l'Url
    Loader loader;
    loader.load("https://www.data.gouv.fr/fr/datasets/r/ecb94e56-22d4-40ee-bf40-c7890569d5bd");

    //récupère le fichier et crée un parse JSON
    json data = json::parse(loader.readBuffer);

    /**
    *Une fois le Json récupéré on voit à sa lecture que les dates ne sont pas dans le bons ordres et font des doublons.\n
    *Les doublons correspondant aux trois population on va s'intéresser à l'une en particulier ( ici résidentiels ) il suffit de changer le nom dans le if(data[i]["fields"]["segment_client"]=="Residentiels") pour avoir les 2 autres.\n
    *On va stocké la colonne date_debut convertie au format epoch dans un vector mydate que l'on pourra trier.\n
    *L'idée sera pas la suite de se servir de ce vector pour faire correspondre les dates du JSON pour l'obtenir dans l'ordre.\n
    */
    //Variable pour les boucles for et création d'un vector mydate
    int i;
    int i2;
    vector<int> mydate;

    //travail date
    for(i=0; i<=131;i++){  //Boucle sur les 132 valeurs du fichiers JSON
      if(data[i]["fields"]["segment_client"]=="Residentiels")   //Conditions sur la population Residentiels
       {
        string date1=(data[i]["fields"]["date_debut"]);  // On récupère la data_debut que l'on met au format string
        const time_t date2 = HistoryCache::convertTimeToEpoch(date1.c_str()); //Convertit la date string au format epoch
        mydate.push_back(date2);}} //Remplit le vector mydate en rajoutant les dates epoch au fur et à mesure

    sort(mydate.begin(),mydate.end());//Tri les dates epoch dans l'ordre croissant

    /**
    *On va regardé plus loin dans un diagramme en secteur la consommation utilisée par les trois population donc on fait les calculs de % totaux.\n
    *On va reproduire une boucle avec la population comme contidion pour avoir la consommation total à temps réel:\n
    * for(i=0; i<=131;i++)
         {
            if(data[i]["fields"]["segment_client"]=="Professionnels")
            {int j=data[i]["fields"]["conso_a_tr"];
            profinal+=j;}
         }
    */


    //Variables de stockages des résultats pour les consommations totales
    int profinal=0,entfinal=0,resfinal=0;


        for(i=0; i<=131;i++)
           {
            if(data[i]["fields"]["segment_client"]=="Professionnels")
            {int j=data[i]["fields"]["conso_a_tr"];
            profinal+=j;}
           }

        for(i=0; i<=131;i++)
           {
             if(data[i]["fields"]["segment_client"]=="Entreprises")
             {int j=data[i]["fields"]["conso_a_tr"];
             entfinal+=j;}
           }
         for(i=0; i<=131;i++)
           {
               if(data[i]["fields"]["segment_client"]=="Residentiels")
               {int j=data[i]["fields"]["conso_a_tr"];
               resfinal+=j;}
           }

 // passage des totaux en %
 float  consotrtotale=resfinal+entfinal+profinal;
 float  pourcentage_conso_tr_ent=(entfinal/consotrtotale)*100;
 float  pourcentage_conso_tr_pro=profinal/consotrtotale*100;
 float  pourcentage_conso_tr_res=resfinal/consotrtotale*100;

 //Préparation des variables pour le diagramme en secteur
 float convres=(pourcentage_conso_tr_res/100)*360;
 float convent=(pourcentage_conso_tr_ent/100)*360;
 float convpro=(pourcentage_conso_tr_pro/100)*360;

 /**
 *Un premier graphique en secteur avec la librairie GD indiquant le % de consommation à Tr par population.\n
 *Ce graphique comme les suivants sera au format png.\n
 */
  //1ERE IMAGE : PIE CHART
           gdImagePtr im;  // Création de l'image
           FILE *pngout;   // Création du fichier.png

  //Chaîne de caratères titre et légende
  char titre[30]={"Conso population"};
  char s1[30]={"conso residents a tr"};
  char s2[30]={"conso profesionnels a tr"};
  char s3[30]={"conso entreprises a tr"};


           im = gdImageCreate(500, 500);  //Image de taille 500x500

           //Création couleurs
           int black,white,red,blue,orange;
           white = gdImageColorAllocate(im, 255, 255, 255);
           black = gdImageColorAllocate(im, 0, 0, 0);
           red = gdImageColorAllocate(im, 255, 0, 0);
           blue = gdImageColorAllocate(im, 0, 0, 255);
           orange= gdImageColorAllocate(im, 255, 128, 0);

           //Implementation du fontptr et du foreground pour les couleurs arrière/avant plan pour les couleurs de chaînes de string par la suite
           int foreground,foreground1,foreground2,foreground3;
           foreground=foreground1=foreground2=foreground3=0;
           gdFontPtr fontptr = gdFontGetLarge();

           //Choix des couleurs pour l'écriture du titre et la légende
           foreground = gdImageColorAllocate(im, 0, 0, 0);
           foreground1 =gdImageColorAllocate(im, 255, 0, 0);
           foreground2 =gdImageColorAllocate(im, 0, 0, 255);
           foreground3=gdImageColorAllocate(im, 255, 128, 0);

  /**
  *On rajoutera une chaîne de caractère sur une image avec le format  suivant:\n
  *gdImageString(im, fontptr,                                       //choix de l'image et appel fontptr
                       180,                                         //Coordonnée axe des x
                       40,                                          //Coordonnée axe des y avec le 0 en partant du haut
                       (unsigned char*)titre, foreground);          //Appel de la chaîne de caractère et de sa couleur
  *On utilisera également des lignes que ce soit pour souligner/légendes/axes : gdImageLine(im,180,60,315,60,black) avec (image,x1,y1,x2,y2,couleur)  \n
  */
           //Titre souligné + Légende couleurs
           gdImageString(im, fontptr,
                       180,
                       40,
                       (unsigned char*)titre, foreground);
               gdImageLine(im,180,60,315,60,black);

               gdImageString(im, fontptr,
                       250 ,
                       410 ,
                       (unsigned char*)s1, foreground1);
               gdImageLine(im,230,420,240,420,red);

               gdImageString(im, fontptr,
                       250,
                       430,
                       (unsigned char*)s2, foreground2);
               gdImageLine(im,230,440,240,440,blue);

               gdImageString(im, fontptr,
                       250,
                       450 ,
                       (unsigned char*)s3, foreground3);
               gdImageLine(im,230,460,240,460,orange);

   /**
   *Pour le diagramme en secteur on va définir un rayon puis tracer différents arc de cercle en fonctions des % obtenue précédemment.\n
   *Pour un cercle :   gdImageFilledArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, 360, 360, white, gdPie); \n
   *On modifie les angles pour avoir  un arc de cercle , ici 360,360 correspond à un cercle entier.\n
   */
             int cor_rad = 150; //Le rayon

             // cercle blanc si cercle n'est pas remplit puis arc de cercle successifs
             gdImageFilledArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, 360, 360, white, gdPie);
             gdImageFilledArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, 360, convent, red, gdPie);
             gdImageFilledArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, convent, convent+convres, blue, gdPie);
             gdImageFilledArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, convent+convres, convent+convpro+convres, orange, gdPie);

             //Contour cercle
             gdImageEllipse(im,250,250,300,300,black);

  /**
  *Les fichiers images seront dans le répertoire ~/projet_linux_embarque qui a était reproduit dans l'overlay buildroot.\n
  */
             //Ouverture image.png avec l'image tracé précédemment
             pngout = fopen("/home/edouard/projet_linux_embarque/testprojet.png", "wb");
             gdImagePng(im, pngout);

             //Fermeture et nettoyage mémoire
             fclose(pngout);
             gdImageDestroy(im);


   /**
   *La 2ème image est un histogramme de la différence en % entre la consommation à Tr et à Tn en fonction de la date.\n
   *De la même manière on paramètre taille de l'image/couleurs/foreground/format et position du fichier.\n
   *L'histogramme sera construit sur une succession de rectangle avec gdImageFilledRectangle(im2,x1,y1,x2,y2,color); \n
   */
      // 2EME IMAGE:  HISTOGRAMME
               gdImagePtr im2;
               FILE *pngout2;

        im2 = gdImageCreate(1500,730);
        int grisfonce2;
        grisfonce2=gdImageColorAllocate(im, 230, 230, 230); //Pour l'arrière plan de l'image qui prend la première couleur définie
        int black2,white2,orange2,blue2,marron2,red2,red3,orange3,blue3,marron3;
        white2 = gdImageColorAllocate(im2, 255, 255, 255);
        black2 = gdImageColorAllocate(im2, 0, 0, 0);
        orange2= gdImageColorAllocate(im2, 255, 128, 0);
        blue2 = gdImageColorAllocate(im2, 0, 0, 255);
        marron2=gdImageColorAllocate(im2, 88, 41, 0);
        red2 = gdImageColorAllocate(im, 255, 0, 0);
        orange3= gdImageColorAllocate(im2, 205, 88, 0);
        blue3 = gdImageColorAllocate(im2, 0, 0, 150);
        marron3=gdImageColorAllocate(im2, 44, 20, 0);
        red3 = gdImageColorAllocate(im, 150, 0, 0);
        char titre2[]={"Differences % consommations pour les residents temperature reelle et normale par semaine"};

        gdImageString(im2, fontptr,
                     400,
                     30,
                     (unsigned char*)titre2, foreground1);
        gdImageLine(im2,390,50,1120,50,black2);

  /**
  *Fonctionnement du morceau de code qui suit:\n
  *1)On va balayé sur les valeurs de la population choisie.\n
  *2)Voulant utilisé les dates triées on va récupérer notre vector mydate et le remettre au format string. \n
  *  Cela va permettre à l'aide d'une condition avec nos dates provenant du parse JSON de récupérer les infos au fur et à mesure dans le bon ordre.\n
  *3)Effectuer le calcul de %.\n
  *  On va se servir de ces valeurs pour tracer nos rectangles par la suite et on va également les passer au format string pour annoter notre image.\n
  *4)On écrit les annotations en fonction de si notre résultats de % est positifs ou négatifs.\n
  *5)On trace les rectangles avec leurs contours, ici le choix a été fait de différentier les saisons de l'année avec un code couleur.\n
  *  Exemple pour l'hiver:
  *                               if(i>=17 && i<=27)
                               {  gdImageFilledRectangle(im2,40+(i*30),400-(c1*10),70+(i*30),400,blue2);
                                  gdImageRectangle(im2,40+(i*30),400-(c1*10),70+(i*30),400,blue3);}
  *\n
  *6)On va également rajouter quelques dates reliées à l'axe avec des pointillés.\n
  *  Les dates sont choisies pour avoir une semaine par mois et pour la lisibilité.\n
  */

               for(i=0; i<=44;i++)
               {string date3 = HistoryCache::getTimeStamp(mydate[i]);    //passage de mydate au format string maintenant qu'il est trié

                 for(i2=0; i2<=131;i2++){
                     if(data[i2]["fields"]["segment_client"]=="Residentiels")  //Travail sur les Résidentiels
                      {
                      string datejson=data[i2]["fields"]["date_debut"];     //Récupération date_debut dans le JSON
                        if(datejson==date3){                                //Condition qui va nous permettre+les boucles d'avoir les dates dans le bon ordre

                          //Calcul %
                          float a1=data[i]["fields"]["conso_a_tr"];
                          float b1=data[i]["fields"]["conso_a_tn"];
                          float c1=(a1/b1)*100-100;

                          //Récupération valeurs % en string et on ne garde qu'un chiffre après la virgule ,utilisation de floor pour arrondir au préalable
                          string c2string=to_string(floor(10*c1)/10);
                          if(!c2string.empty()){c2string.resize(c2string.size()-5);}

                              //Notation des valeurs en % au bout des barres de l'histogramme
                              if(c1>=0)
                              {gdImageString(im2, fontptr,
                                             40+(i*30),
                                             380-(c1*10),
                                          (unsigned char*)c2string.c_str(), foreground1);}
                              if(c1<=0)
                              {gdImageString(im2, fontptr,
                                    32+(i*30),
                                    410-(c1*10),
                                    (unsigned char*)c2string.c_str(), foreground1);}

                              //Tracage de l'histogramme avec des rectangles remplits en fonctions des couleurs+contours
                              if(i<=3 or i>=41)
                               {  gdImageFilledRectangle(im2,40+(i*30),400-(c1*10),70+(i*30),400,red2);
                                  gdImageRectangle(im2,40+(i*30),400-(c1*10),70+(i*30),400,red3);}
                              if(i>=4 && i<=16)
                               {  gdImageFilledRectangle(im2,40+(i*30),400-(c1*10),70+(i*30),400,marron2);
                                  gdImageRectangle(im2,40+(i*30),400-(c1*10),70+(i*30),400,marron3);}
                              if(i>=17 && i<=27)
                               {  gdImageFilledRectangle(im2,40+(i*30),400-(c1*10),70+(i*30),400,blue2);
                                  gdImageRectangle(im2,40+(i*30),400-(c1*10),70+(i*30),400,blue3);}
                              if(i>=28 && i<=40)
                               {  gdImageFilledRectangle(im2,40+(i*30),400-(c1*10),70+(i*30),400,orange2);
                                  gdImageRectangle(im2,40+(i*30),400-(c1*10),70+(i*30),400,orange3);}

                              //Tracage pointillé + affichagee de plusieurs dates
                              if(i==0 or i==4 or i==8 or i==12 or i==16 or i==24 or i==28 or i==39 or i==43)
                              {gdImageString(im2, fontptr,
                                      20+(i*30),
                                      700,
                                      (unsigned char*)datejson.c_str(), foreground1);
                                  for(int i3=0;i3<=14;i3++)
                                  {gdImageLine(im2,55+(i*30),400+(i3*20),55+(i*30),410+(i3*20),black2);}}

                              if(i==20 or i==31 or i==36)
                              {gdImageString(im2, fontptr,
                                      20+(i*30),
                                      100,
                                      (unsigned char*)datejson.c_str(), foreground1);
                                  for(int i3=0;i3<=13;i3++)
                                  {gdImageLine(im2,55+(i*30),390-(i3*20),55+(i*30),400-(i3*20),black2);}}

                       cout<<datejson<<" à la conso diff    "<<c2string<<endl;}
                     }
                }}

               // Axe ordonnée
               gdImageLine(im2,40,100,40,680,black2);
               gdImageLine(im2,40,100,30,110,black2);
               gdImageLine(im2,40,100,50,110,black2);
               gdImageLine(im2,40,680,30,670,black2);
               gdImageLine(im2,40,680,50,670,black2);
               char axeordo[50]="% Difference consomation en TWh enntre tr et tn";
               gdImageString(im2, fontptr,
                              60,
                              100,
                           (unsigned char*)axeordo, foreground1);

               // Axe abcisse
               gdImageLine(im2,40,400,1410,400,black2);
               gdImageLine(im2,1410,400,1400,410,black2);
               gdImageLine(im2,1410,400,1400,390,black2);
               char axeabs[50]="Date debut_semaine";
               gdImageString(im2, fontptr,
                              1350,
                              420,
                           (unsigned char*)axeabs, foreground1);

               //Légende
               char legende[9]="Legende:";
               char ete[4]="Ete";
               char automne[8]="Automne";
               char hiver[6]="Hiver";
               char printemps[10]="Printemps";
                gdImageFilledRectangle(im2,1300,50,1350,70,red2);
                gdImageFilledRectangle(im2,1300,80,1350,100,marron2);
                gdImageFilledRectangle(im2,1300,110,1350,130,blue2);
                gdImageFilledRectangle(im2,1300,140,1350,160,orange2);
                gdImageRectangle(im2,1300,50,1350,70,red3);
                gdImageRectangle(im2,1300,80,1350,100,marron3);
                gdImageRectangle(im2,1300,110,1350,130,blue3);
                gdImageRectangle(im2,1300,140,1350,160,orange3);
                gdImageString(im2, fontptr, 1300, 25, (unsigned char*)legende,foreground1);
                gdImageLine(im2,1300,40,1360,40,black2);
                gdImageString(im2, fontptr, 1360, 55, (unsigned char*)ete,foreground1);
                gdImageString(im2, fontptr, 1360, 85, (unsigned char*)automne,foreground1);
                gdImageString(im2, fontptr, 1360, 115, (unsigned char*)hiver,foreground1);
                gdImageString(im2, fontptr, 1360, 145, (unsigned char*)printemps,foreground1);

               pngout2 = fopen("/home/edouard/projet_linux_embarque/testprojethisto.png", "wb");
               gdImagePng(im2, pngout2);

    return (EXIT_SUCCESS);
}
