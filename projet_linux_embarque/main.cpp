#include"loader.h"
#include"nlohmann/json.hpp"
#include<fstream>
#include<iostream>
#include"gd.h"
#include <gdfontl.h>
#include"convertepoch.h"
#include<algorithm>
#include<vector>
#include<math.h>


using json = nlohmann::json;
using namespace std;

int main(int argc, char** argv) {


    Loader loader;
    loader.load("https://www.data.gouv.fr/fr/datasets/r/ecb94e56-22d4-40ee-bf40-c7890569d5bd");

    //récupère le fichier et crée un parse JSON

    json data = json::parse(loader.readBuffer);

    int i;
    int i2;
    vector<int> mydate;

    //travail date

    for(i=0; i<=131;i++){
      if(data[i]["fields"]["segment_client"]=="Residentiels")
      {
      string date1=(data[i]["fields"]["date_debut"]);
      // convert string time to an epoch time
      const time_t date2 = HistoryCache::convertTimeToEpoch(date1.c_str());
      mydate.push_back(date2);}}

    sort(mydate.begin(),mydate.end());

    //Lectuure dans le JSON
    float d=0;
    int profinal=0,entfinal=0,resfinal=0;

   /* for(i=0; i<=131;i++){
    float a=data[i]["fields"]["conso_a_tr"];
    float b=data[i]["fields"]["conso_a_tn"];
    float c=(a/b)*100-100;
    d+=c;
    //cout<<"le pourcentage de différence la semaine du "<<data[i]["fields"]["date_debut"]<<" est: "<<c<<" %"<<endl;
    }
    float difftotal=d/132;
    //cout<<difftotal<<"%"<<endl;
*/
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
            // Calcul % conso à Temps rèelle
            float  consotrtotale=resfinal+entfinal+profinal;
            float  pourcentage_conso_tr_ent=(entfinal/consotrtotale)*100;
            float  pourcentage_conso_tr_pro=profinal/consotrtotale*100;
            float  pourcentage_conso_tr_res=resfinal/consotrtotale*100;


           //cout<<"Les entreprises consomment à température rèelle: "<<pourcentage_conso_tr_ent<<"% de la consommation française"<<endl;
           //cout<<"Les professionnels consomment à température rèelle: "<<pourcentage_conso_tr_pro<<"% de la consommation française"<<endl;
           //cout<<"Les résidents consomment à température rèelle: "<<pourcentage_conso_tr_res<<"% de la consommation française"<<endl;

           // variables pour graphiques
           float convres=(pourcentage_conso_tr_res/100)*360;
           float convent=(pourcentage_conso_tr_ent/100)*360;
           float convpro=(pourcentage_conso_tr_pro/100)*360;


  //1ERE IMAGE : PIE CHART
           gdImagePtr im;
           FILE *pngout;
           char titre[]={"Conso population"};
           char s1[]={"conso residents a tr"};
           char s2[]={"conso profesionnels a tr"};
           char s3[]={"conso entreprises a tr"};
           im = gdImageCreate(500, 500);
           int black,white,red,green,blue,orange,gris,marron;
           white = gdImageColorAllocate(im, 255, 255, 255);
           black = gdImageColorAllocate(im, 0, 0, 0);
           red = gdImageColorAllocate(im, 255, 0, 0);
           green = gdImageColorAllocate(im, 0, 255, 0);
           blue = gdImageColorAllocate(im, 0, 0, 255);
           orange= gdImageColorAllocate(im, 255, 128, 0);
           gris=gdImageColorAllocate(im, 128, 128, 128);
           marron=gdImageColorAllocate(im, 88, 41, 0);

           int foreground,foreground1,foreground2,foreground3;;
           foreground=foreground1=foreground2=foreground3=0;
           gdFontPtr fontptr = gdFontGetLarge();

       //Choix des couleurs pour l'écriture des variables dans la légende
           foreground = gdImageColorAllocate(im, 0, 0, 0);
           foreground1 =gdImageColorAllocate(im, 255, 0, 0);
           foreground2 =gdImageColorAllocate(im, 0, 0, 255);
           foreground3=gdImageColorAllocate(im, 255, 128, 0);
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

           int cor_rad = 150;

             gdImageFilledArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, 360, 360, white, gdPie);

             gdImageFilledArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, 360, convent, red, gdPie);
             gdImageFilledArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, convent, convent+convres, blue, gdPie);
             gdImageFilledArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, convent+convres, convent+convpro+convres, orange, gdPie);


             gdImageEllipse(im,250,250,300,300,black);

             pngout = fopen("/home/edouard/projet_linux_embarque/testprojet.png", "wb");
               gdImagePng(im, pngout);

              //Fermeture et nettoyage mémoire
               fclose(pngout);
               gdImageDestroy(im);

      // 2EME IMAGE:  HISTOGRAMME
               gdImagePtr im2;
               FILE *pngout2;
               im2 = gdImageCreate(1500,730);
               int grisfonce2;
               grisfonce2=gdImageColorAllocate(im, 230, 230, 230);
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



               for(i=0; i<=44;i++)
               {string date3 = HistoryCache::getTimeStamp(mydate[i]);

                 for(i2=0; i2<=131;i2++){
                     if(data[i2]["fields"]["segment_client"]=="Residentiels")
                      {
                      string datejson=data[i2]["fields"]["date_debut"];
                        if(datejson==date3){
                          float a1=data[i]["fields"]["conso_a_tr"];
                          float b1=data[i]["fields"]["conso_a_tn"];
                          float c1=(a1/b1)*100-100;
                          string c2string=to_string(floor(10*c1)/10);
                          if(!c2string.empty()){c2string.resize(c2string.size()-5);}



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
