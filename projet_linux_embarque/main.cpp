#include"loader.h"
#include"nlohmann/json.hpp"
#include<fstream>
#include<iostream>
#include"gd.h"
#include <gdfontl.h>

using json = nlohmann::json;
using namespace std;

int main(int argc, char** argv) {


    Loader loader;
    loader.load("https://www.data.gouv.fr/fr/datasets/r/ecb94e56-22d4-40ee-bf40-c7890569d5bd");

    //récupère le fichier et crée un parse JSON

    json data = json::parse(loader.readBuffer);

    int i;
    //Lectuure dans le JSON
    float d=0;
    int profinal=0,entfinal=0,resfinal=0;

    for(i=0; i<=131;i++){
    float a=data[i]["fields"]["conso_a_tr"];
    float b=data[i]["fields"]["conso_a_tn"];
    float c=(a/b)*100-100;
    d+=c;
    //cout<<"le pourcentage de différence la semaine du "<<data[i]["fields"]["date_debut"]<<" est: "<<c<<" %"<<endl;
    }
    float difftotal=d/132;
    //cout<<difftotal<<"%"<<endl;

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

            for(i=0; i<=131;i++){
           cout<<setw(4)<<data[i]["fields"]["date_debut"]<<endl;}
/*
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

               // 2 IMAGES HISTOGRAMME
               gdImagePtr im2;
               FILE *pngout2;
               im2 = gdImageCreate(1400,500);
               int black2,white2,red2,green2,blue2,orange2,gris2,marron2;
               white2 = gdImageColorAllocate(im2, 255, 255, 255);
               black2 = gdImageColorAllocate(im2, 0, 0, 0);
               red2 = gdImageColorAllocate(im2, 255, 0, 0);
               green2 = gdImageColorAllocate(im2, 0, 255, 0);
               blue2 = gdImageColorAllocate(im2, 0, 0, 255);
               orange2= gdImageColorAllocate(im2, 255, 128, 0);
               gris2=gdImageColorAllocate(im2, 128, 128, 128);
               marron2=gdImageColorAllocate(im2, 88, 41, 0);

               gdImageLine(im2,40,20,40,400,black2);


               for(i=0; i<=131;i++){
               float a1=data[i]["fields"]["conso_a_tr"];
               float b1=data[i]["fields"]["conso_a_tn"];
               float c1=(a1/b1)*100-100;
                  if(c1<0){c1=c1*(-1);}

               gdImageRectangle(im2,40+(i*10),400-(c1*10),50+(i*10),400,orange2);}
               gdImageLine(im2,40,400,1370,400,black2);
               pngout2 = fopen("/home/edouard/projet_linux_embarque/testprojethisto.png", "wb");
               gdImagePng(im2, pngout2);
*/
    return (EXIT_SUCCESS);
}
