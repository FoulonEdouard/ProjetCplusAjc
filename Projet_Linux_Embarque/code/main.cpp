/**
*@file main.cpp
*Fichier principal du Projet_Linux_Embarqué sur un OpenData
*@author Foulon Edouard
*@date 13/09/2023
*@Version 1.0
*/

/**
* @mainpage
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
*\n
* La commandes de compilation utilisée pour la qemu:\n
* Pour la qemu:/home/edouard/buildroot-2023.08/output/host/bin/aarch64-linux-g++ -o progarmgd main.cpp loader.cpp convertepoch.cpp -lcurl -lgd -lpng -lm -lstdc++fs
*/


#include"loader.h"                //Classe contenant l'appel avec Curl
#include"nlohmann/json.hpp"       //Librairie utilisée pour utilisé les fichiers JSON
#include"gd.h"                    //Librairie graphique utilisée
#include <gdfontl.h>              //Va avec GD
#include"convertepoch.h"          //Classe contennant 2 méthodes pour passer une date en epoch et inversement
#include<algorithm>               //Librairie pour utiliser sort()
#include<vector>                  //Connteneur vector qui sera utilisé conjointement avec le JSON
#include<math.h>                  //Pour l'arrondi sur les floats


//Utilisations namespaces pour simplifié le programme
using namespace std;
using json = nlohmann::json;

/**
 *@fn void secteur(string temperature)
 *@param temperature va permettre le choix entre une étude à température réelle ou normale.\n
 *@brief Va créer un diagramme en secteur. \n
 *Un premier graphique en secteur avec la librairie GD indiquant le % de consommation à Tr par population.\n
 *Ce graphique comme les suivants sera au format png.\n
 *\n
 *On va regardé plus loin dans un diagramme en secteur la consommation utilisée par les trois population donc on fait les calculs de % totaux.\n
 *On va reproduire une boucle avec la population comme contidion pour avoir la consommation total à temps réel:\n
 * for(i=0; i<=131;i++)    \n
 *    {                     \n
 * if(data[i]["fields"]["segment_client"]=="Professionnels")  \n
 *       {int j=data[i]["fields"]["conso_a_tr"];   \n
 *        profinal+=j;}   \n
 *     } \n
 */

void secteur(string temperature)
{
    int i=0;
    int i2=0;

    //Appel classe Loader pour récupérer l'Url
    Loader loader;
    loader.load("https://www.data.gouv.fr/fr/datasets/r/ecb94e56-22d4-40ee-bf40-c7890569d5bd");
    json data = json::parse(loader.readBuffer);

    //récupère le fichier et crée un parse JSON



    //Variables de stockages des résultats pour les consommations totales
    int profinal=0,entfinal=0,resfinal=0;
    int profinaln=0,entfinaln=0,resfinaln=0;

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

         // Pour température normale
         for(i=0; i<=131;i++)
            {
             if(data[i]["fields"]["segment_client"]=="Professionnels")
             {int j=data[i]["fields"]["conso_a_tn"];
             profinaln+=j;}
            }

         for(i=0; i<=131;i++)
            {
              if(data[i]["fields"]["segment_client"]=="Entreprises")
              {int j=data[i]["fields"]["conso_a_tn"];
              entfinaln+=j;}
            }
          for(i=0; i<=131;i++)
            {
                if(data[i]["fields"]["segment_client"]=="Residentiels")
                {int j=data[i]["fields"]["conso_a_tn"];
                resfinaln+=j;}
            }

 // passage des totaux en %
 float  consotrtotale=resfinal+entfinal+profinal;
 float  consotrtotalen=resfinaln+entfinaln+profinaln;
 float  pourcentage_conso_tr_ent=(entfinal/consotrtotale)*100;
 float  pourcentage_conso_tr_pro=(profinal/consotrtotale)*100;
 float  pourcentage_conso_tr_res=(resfinal/consotrtotale)*100;
 float  pourcentage_conso_tn_ent=(entfinaln/consotrtotalen)*100;
 float  pourcentage_conso_tn_pro=(profinaln/consotrtotalen)*100;
 float  pourcentage_conso_tn_res=(resfinaln/consotrtotalen)*100;

 //Préparation des variables pour le diagramme en secteur
 float convres=(pourcentage_conso_tr_res/100)*360;
 float convent=(pourcentage_conso_tr_ent/100)*360;
 float convpro=(pourcentage_conso_tr_pro/100)*360;
 float convresn=(pourcentage_conso_tn_res/100)*360;
 float conventn=(pourcentage_conso_tn_ent/100)*360;
 float convpron=(pourcentage_conso_tn_pro/100)*360;



    gdImagePtr im;  // Création de l'image
    FILE *pngout;   // Création du fichier.png

//Chaîne de caratères titre et légende

char s1[30]={"conso residents :"};
char s2[30]={"conso profesionnels :"};
char s3[30]={"conso entreprises :"};


    im = gdImageCreate(500, 500);  //Image de taille 500x500

    //Création couleurs
    int black,white,red,blue,orange,redcont,bluecont,orangecont;
    white = gdImageColorAllocate(im, 255, 255, 255);
    black = gdImageColorAllocate(im, 0, 0, 0);
    red = gdImageColorAllocate(im, 255, 0, 0);
    blue = gdImageColorAllocate(im, 0, 0, 255);
    orange= gdImageColorAllocate(im, 255, 128, 0);
    orangecont= gdImageColorAllocate(im, 205, 88, 0);
    bluecont = gdImageColorAllocate(im, 0, 0, 150);
    redcont = gdImageColorAllocate(im, 150, 0, 0);

    //Implementation du fontptr et du foreground pour les couleurs arrière/avant plan pour les couleurs de chaînes de string par la suite
    int foreground,foreground1,foreground2,foreground3;
    foreground=foreground1=foreground2=foreground3=0;
    gdFontPtr fontptr = gdFontGetLarge();

    //Choix des couleurs pour l'écriture du titre et la légende
    foreground = gdImageColorAllocate(im, 0, 0, 0);
    foreground1 =gdImageColorAllocate(im, 255, 0, 0);
    foreground2 =gdImageColorAllocate(im, 0, 0, 255);
    foreground3=gdImageColorAllocate(im, 255, 128, 0);

    if(temperature=="reelle"){
    char titre[40]={"Consommation selon la population a tr"};
    gdImageString(im, fontptr,
                100,
                40,
                (unsigned char*)titre, foreground);
        gdImageLine(im,100,60,400,60,black);

    string trrstring=to_string(floor(10*pourcentage_conso_tr_res)/10);
    if(!trrstring.empty()){trrstring.resize(trrstring.size()-5);}
    string ftrr=trrstring+" %";

        gdImageString(im, fontptr,
                250 ,
                410 ,
                (unsigned char*)s1, foreground1);
        gdImageString(im, fontptr,
                390 ,
                410 ,
                (unsigned char*)ftrr.c_str(), foreground1);
        gdImageLine(im,230,420,240,420,red);

        string tprstring=to_string(floor(10*pourcentage_conso_tr_pro)/10);
        if(!tprstring.empty()){tprstring.resize(tprstring.size()-5);}
        string ftpr=tprstring+" %";
        gdImageString(im, fontptr,
                250,
                430,
                (unsigned char*)s2, foreground2);
        gdImageString(im, fontptr,
                420,
                430,
                (unsigned char*)ftpr.c_str(), foreground2);

        string terstring=to_string(floor(10*pourcentage_conso_tr_ent)/10);
        if(!terstring.empty()){terstring.resize(terstring.size()-5);}
        string fepr=terstring+" %";
        gdImageLine(im,230,440,240,440,blue);
        gdImageString(im, fontptr,
                250,
                450 ,
                (unsigned char*)s3, foreground3);
        gdImageString(im, fontptr,
                410,
                450 ,
                (unsigned char*)fepr.c_str(), foreground3);
        gdImageLine(im,230,460,240,460,orange);}



    if(temperature=="normale"){
    char titre[40]={"Consommation selon la population a tn"};
    gdImageString(im, fontptr,
                100,
                40,
                (unsigned char*)titre, foreground);
        gdImageLine(im,100,60,400,60,black);

    string trnstring=to_string(floor(10*pourcentage_conso_tn_res)/10);
    if(!trnstring.empty()){trnstring.resize(trnstring.size()-5);}
    string ftrn=trnstring+" %";

        gdImageString(im, fontptr,
                250 ,
                410 ,
                (unsigned char*)s1, foreground1);
        gdImageString(im, fontptr,
                390 ,
                410 ,
                (unsigned char*)ftrn.c_str(), foreground1);
        gdImageLine(im,230,420,240,420,red);

        string tpnstring=to_string(floor(10*pourcentage_conso_tn_pro)/10);
        if(!tpnstring.empty()){tpnstring.resize(tpnstring.size()-5);}
        string ftpn=tpnstring+" %";
        gdImageString(im, fontptr,
                250,
                430,
                (unsigned char*)s2, foreground2);
        gdImageString(im, fontptr,
                420,
                430,
                (unsigned char*)ftpn.c_str(), foreground2);

        string tenstring=to_string(floor(10*pourcentage_conso_tn_ent)/10);
        if(!tenstring.empty()){tenstring.resize(tenstring.size()-5);}
        string fepn=tenstring+" %";
        gdImageLine(im,230,440,240,440,blue);
        gdImageString(im, fontptr,
                250,
                450 ,
                (unsigned char*)s3, foreground3);
        gdImageString(im, fontptr,
                410,
                450 ,
                (unsigned char*)fepn.c_str(), foreground3);
        gdImageLine(im,230,460,240,460,orange);}


        int cor_rad = 150;
        if(temperature=="reelle")
        {
                    //cercle blanc si cercle n'est pas remplit puis arc de cercle successifs
                     gdImageFilledArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, 360, 360, white, gdPie);
                     gdImageFilledArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, 360, convres, red, gdPie);
                     gdImageFilledArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, convres, convpro+convent, blue, gdPie);
                     gdImageFilledArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, convpro+convres, convent+convpro+convres, orange, gdPie);
                     gdImageArc(im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, 360, convres, redcont);
                     gdImageArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, convres, convpro+convres, bluecont);
                     gdImageArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, convpro+convres, convent+convpro+convres, orangecont);

                     //Ouverture image.png avec l'image tracé précédemment
                    pngout = fopen("/home/edouard/projet_linux_embarque/Secteur_Consommation_Totale_tr.png", "wb");
                     gdImagePng(im, pngout);

                     //Fermeture et nettoyage mémoire
                     fclose(pngout);
                     gdImageDestroy(im);}
        if(temperature=="normale")
        {
                    //cercle blanc si cercle n'est pas remplit puis arc de cercle successifs
                     gdImageFilledArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, 360, 360, white, gdPie);
                     gdImageFilledArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, 360, convresn, red, gdPie);
                     gdImageFilledArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, convresn, convpron+convresn, blue, gdPie);
                     gdImageFilledArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, convpron+convresn, conventn+convpron+convresn, orange, gdPie);
                     gdImageArc(im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, 360, convresn, redcont);
                     gdImageArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, convresn, convpron+convresn, bluecont);
                     gdImageArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, convpron+convresn, conventn+convpron+convresn, orangecont);

                     //Ouverture image.png avec l'image tracé précédemment
                    pngout = fopen("/home/edouard/projet_linux_embarque/Secteur_Consommation_Totale_tn.png", "wb");
                     gdImagePng(im, pngout);

                     //Fermeture et nettoyage mémoire
                     fclose(pngout);
                     gdImageDestroy(im);}

}

/**
*@fn void histogramme(string population)
*@param population permet de choisir la population entre Résidentiels , Entreprises , Professionnels\n
*/
/**
*@fn void histogramme(string population)
*@brief C'est une fonction pour construire un histogramme.\n
*On aura donc plusieurs  histogrammes de la différence en % entre la consommation à Tr et à Tn en fonction de la date.\n
*De la même manière on paramètre taille de l'image/couleurs/foreground/format et position du fichier.\n
*L'histogramme sera construit sur une succession de rectangle avec gdImageFilledRectangle(im2,x1,y1,x2,y2,color); \n
* De la même manière on paramètre taille de l'image/couleurs/foreground/format et position du fichier.\n
*L'histogramme sera construit sur une succession de rectangle avec gdImageFilledRectangle(im2,x1,y1,x2,y2,color); \n
*\n
* Une fois le Json récupéré on voit à sa lecture que les dates ne sont pas dans le bons ordres et font des doublons.\n
*On va stocké la colonne date_debut convertie au format epoch dans un vector mydate que l'on pourra trier.\n
*L'idée sera pas la suite de se servir de ce vector pour faire correspondre les dates du JSON pour l'obtenir dans l'ordre.\n
*\n
*\n
*Le morceau principal du programme effectue plusieurs étapes:\n
*-->PREMIERE ETAPE:On récupère le vector mydate que l'on remet au format string pour  le comparer avec les dates du JSON \n
*On met également la condition sur la population pour le reste de la boucle.\n
*for(i=0; i<=44;i++)\n
*               {string date3 = HistoryCache::getTimeStamp(mydate[i]);   \n
*                for(i2=0; i2<=131;i2++){ \n
*                    if(data[i2]["fields"]["segment_client"]==population)  \n
*                     {\n
*                     string datejson=data[i2]["fields"]["date_debut"];     \n
*                        if(datejson==date3){                               \n
*\n
*-->DEUXIEME ETAPE:\n
*On effectue les calculs souhaités pour obtenir les % ici et on prépare leur écriture sous formmat string pour les mettre sur l''histogramme (1 chiffres après la virgule)\n
*a1=data[i2]["fields"]["conso_a_tr"];\n
*b1=data[i2]["fields"]["conso_a_tn"];\n
*c1=(a1/b1)*100-100\n
*string c2string=to_string(floor(10*c1)/10);\n
*if(!c2string.empty()){c2string.resize(c2string.size()-5);}\n
*\n
*-->TROISIEME ETAPE:\n
*On trace l'histogramme avec une suite de rectangle + des couleurs selon les saisons. Ici exemple avec l'hiver:\n
* if(i>=17 && i<=27)\n
*                     {  gdImageFilledRectangle(im2,40+(i*30),400-(c1*10),70+(i*30),400,blue2);\n
*                        gdImageRectangle(im2,40+(i*30),400-(c1*10),70+(i*30),400,blue3);}\n
*\n
*-->QUATRIEME ETAPE:\n
*On remplit les annotations et on ajoute des pointillés pour les dates en faisant attention à ne pas les mettre du même côté que les barres de l'histogramme(environ 1 date par mois).\n
*Exemple annotations %:\n
*                             if(c1>=0)\n
*                             {gdImageString(im2, fontptr,\n
*                                             40+(i*30),\n
*                                            380-(c1*10),\n
*                                         (unsigned char*)c2string.c_str(), foreground);}\n
*Exemple annotation dates:\n
* if(c1>0 && (i==1 or i==4 or i==8 or i==12 or i==16 or i==20 or i==24 or i==28 or i==31 or i==36 or i==40))\n
*                              {gdImageString(im2, fontptr,\n
*                                      20+(i*30),\n
*                                     700,\n
*                                      (unsigned char*)datejson.c_str(), foreground);\n
*                                 for(int i3=0;i3<=14;i3++)\n
*                                 {gdImageLine(im2,55+(i*30),400+(i3*20),55+(i*30),410+(i3*20),black2);}}  \n
* \n
* Les axes + légendes sont à la suite.\n
*/
void histogramme(string population)
{

    int i=0;
    int i2=0;

    //Appel classe Loader pour récupérer l'Url
    Loader loader;
    loader.load("https://www.data.gouv.fr/fr/datasets/r/ecb94e56-22d4-40ee-bf40-c7890569d5bd");
    json data = json::parse(loader.readBuffer);

    //récupère le fichier et crée un parse JSON

    vector<int> mydate;

    //travail date
    for(i=0; i<=131;i++){  //Boucle sur les 132 valeurs du fichiers JSON
      if(data[i]["fields"]["segment_client"]=="Residentiels")   //Conditions sur la population Residentiels
       {
        string date1=(data[i]["fields"]["date_debut"]);  // On récupère la data_debut que l'on met au format string
        const time_t date2 = HistoryCache::convertTimeToEpoch(date1.c_str()); //Convertit la date string au format epoch
        mydate.push_back(date2);}} //Remplit le vector mydate en rajoutant les dates epoch au fur et à mesure

    sort(mydate.begin(),mydate.end());//Tri les dates epoch dans l'ordre croissant vector<int> mydate;


    gdImagePtr im2;
    FILE *pngout2;


im2 = gdImageCreate(1500,730);
int grisfonce2;
grisfonce2=gdImageColorAllocate(im2, 230, 230, 230); //Pour l'arrière plan de l'image qui prend la première couleur définie
int black2,white2,orange2,blue2,marron2,red2,red3,orange3,blue3,marron3;
white2 = gdImageColorAllocate(im2, 255, 255, 255);
black2 = gdImageColorAllocate(im2, 0, 0, 0);
orange2= gdImageColorAllocate(im2, 255, 128, 0);
blue2 = gdImageColorAllocate(im2, 0, 0, 255);
marron2=gdImageColorAllocate(im2, 88, 41, 0);
red2 = gdImageColorAllocate(im2, 255, 0, 0);
orange3= gdImageColorAllocate(im2, 205, 88, 0);
blue3 = gdImageColorAllocate(im2, 0, 0, 150);
marron3=gdImageColorAllocate(im2, 44, 20, 0);
red3 = gdImageColorAllocate(im2, 150, 0, 0);
int foreground,foreground1,foreground2,foreground3;
foreground=foreground1=foreground2=foreground3=0;
gdFontPtr fontptr = gdFontGetLarge();

//Choix des couleurs pour l'écriture du titre et la légende
foreground = gdImageColorAllocate(im2, 0, 0, 0);
foreground1 =gdImageColorAllocate(im2, 255, 0, 0);
foreground2 =gdImageColorAllocate(im2, 0, 0, 255);
foreground3=gdImageColorAllocate(im2, 255, 128, 0);
char titre2[]={"Differences % consommations a temperature reelle et normale par semaine"};

gdImageString(im2, fontptr,
          400,
          30,
          (unsigned char*)titre2, foreground);
gdImageLine(im2,390,50,1000,50,black2);

float a1=0;
float b1=0;
float c1=0;
               for(i=0; i<=44;i++)
               {string date3 = HistoryCache::getTimeStamp(mydate[i]);    //passage de mydate au format string maintenant qu'il est trié

                 for(i2=0; i2<=131;i2++){
                     if(data[i2]["fields"]["segment_client"]==population)  //Travail sur les Résidentiels
                      {
                      string datejson=data[i2]["fields"]["date_debut"];     //Récupération date_debut dans le JSON
                        if(datejson==date3){                                //Condition qui va nous permettre+les boucles d'avoir les dates dans le bon ordre

                          //Calcul %
                          a1=data[i2]["fields"]["conso_a_tr"];
                          b1=data[i2]["fields"]["conso_a_tn"];
                          c1=(a1/b1)*100-100;

                          //Récupération valeurs % en string et on ne garde qu'un chiffre après la virgule ,utilisation de floor pour arrondir au préalable
                          string c2string=to_string(floor(10*c1)/10);
                          if(!c2string.empty()){c2string.resize(c2string.size()-5);}

                              //Notation des valeurs en % au bout des barres de l'histogramme
                              if(c1>=0)
                              {gdImageString(im2, fontptr,
                                             40+(i*30),
                                             380-(c1*10),
                                          (unsigned char*)c2string.c_str(), foreground);}
                              if(c1<=0)
                              {gdImageString(im2, fontptr,
                                    32+(i*30),
                                    410-(c1*10),
                                    (unsigned char*)c2string.c_str(), foreground);}

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
                              if(c1>0 && (i==1 or i==4 or i==8 or i==12 or i==16 or i==20 or i==24 or i==28 or i==31 or i==36 or i==40))
                              {gdImageString(im2, fontptr,
                                      20+(i*30),
                                      700,
                                      (unsigned char*)datejson.c_str(), foreground);
                                  for(int i3=0;i3<=14;i3++)
                                  {gdImageLine(im2,55+(i*30),400+(i3*20),55+(i*30),410+(i3*20),black2);}}

                              if(c1<0 && (i==1 or i==4 or i==8 or i==12 or i==16 or i==20 or i==24 or i==28 or i==32 or i==36 or i==40))
                              {gdImageString(im2, fontptr,
                                      20+(i*30),
                                      100,
                                      (unsigned char*)datejson.c_str(), foreground);
                                  for(int i3=0;i3<=13;i3++)
                                  {gdImageLine(im2,55+(i*30),390-(i3*20),55+(i*30),400-(i3*20),black2);}}

                      // cout<<datejson<<" à la conso diff    "<<c2string<<endl;
                        }
                     }
                }}

               // Axe ordonnée
               gdImageLine(im2,40,80,40,680,black2);
               gdImageLine(im2,40,80,30,90,black2);
               gdImageLine(im2,40,80,50,90,black2);
               gdImageLine(im2,40,680,30,670,black2);
               gdImageLine(im2,40,680,50,670,black2);
               char axeordo[50]="% Difference consomation en TWh enntre tr et tn";
               gdImageString(im2, fontptr,
                              10,
                              65,
                           (unsigned char*)axeordo, foreground);

               // Axe abcisse
               gdImageLine(im2,40,400,1410,400,black2);
               gdImageLine(im2,1410,400,1400,410,black2);
               gdImageLine(im2,1410,400,1400,390,black2);
               char axeabs[50]="Date debut semaine";
               gdImageString(im2, fontptr,
                              1330,
                              450,
                           (unsigned char*)axeabs, foreground);

               //Légende
               char legende[9]="Legende:";
               char ete[4]="Ete";
               char automne[8]="Automne";
               char hiver[6]="Hiver";
               char printemps[10]="Printemps";
                gdImageFilledRectangle(im2,1320,50,1370,70,red2);
                gdImageFilledRectangle(im2,1320,80,1370,100,marron2);
                gdImageFilledRectangle(im2,1320,110,1370,130,blue2);
                gdImageFilledRectangle(im2,1320,140,1370,160,orange2);
                gdImageRectangle(im2,1320,50,1370,70,red3);
                gdImageRectangle(im2,1320,80,1370,100,marron3);
                gdImageRectangle(im2,1320,110,1370,130,blue3);
                gdImageRectangle(im2,1320,140,1370,160,orange3);
                gdImageString(im2, fontptr, 1320, 25, (unsigned char*)legende,foreground);
                gdImageLine(im2,1320,40,1380,40,black2);
                gdImageString(im2, fontptr, 1380, 55, (unsigned char*)ete,foreground);
                gdImageString(im2, fontptr, 1380, 85, (unsigned char*)automne,foreground);
                gdImageString(im2, fontptr, 1380, 115, (unsigned char*)hiver,foreground);
                gdImageString(im2, fontptr, 1380, 145, (unsigned char*)printemps,foreground);

               if(population=="Residentiels")
               pngout2 = fopen("/home/edouard/projet_linux_embarque/Consommation_Residelentiels.png", "wb");
               if(population=="Entreprises")
               pngout2 = fopen("/home/edouard/projet_linux_embarque/Consommation_Entreprises.png", "wb");
               if(population=="Professionnels")
               pngout2 = fopen("/home/edouard/projet_linux_embarque/Consommation_Professionnels.png", "wb");
               gdImagePng(im2, pngout2);
return;
}


/**
 * @fn int main(int argc, char** argv)
 * @brief entrée du programme  \n
 * @return EXIT_SUCCES arrêt normal du programme\n
 */


int main(int argc, char** argv) {


    string r="Residentiels";
    string e="Entreprises";
    string p="Professionnels";

    histogramme(r);
    histogramme(p);
    histogramme(e);
    secteur("reelle");
    secteur("normale");


    return (EXIT_SUCCESS);
}
