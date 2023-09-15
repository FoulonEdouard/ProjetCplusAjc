#include"loader.h"
#include"nlohmann/json.hpp"
#include<fstream>
#include<iostream>
#include"gd.h"

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
    cout<<difftotal<<"%"<<endl;

        for(i=0; i<=131;i++)
        {
            if(data[i]["fields"]["segment_client"]=="Professionnels")
            {int j=data[i]["fields"]["conso_a_tr"];
            profinal+=j;}
        }
         cout<<"La consommation total a temps rèelle par les profesionnels est:"<<profinal<<endl;

        for(i=0; i<=131;i++)
         {
             if(data[i]["fields"]["segment_client"]=="Entreprises")
             {int j=data[i]["fields"]["conso_a_tr"];
             entfinal+=j;}
         }
          cout<<"La consommation total a temps rèelle par les entreprises est:"<<entfinal<<endl;


          for(i=0; i<=131;i++)
           {
               if(data[i]["fields"]["segment_client"]=="Residentiels")
               {int j=data[i]["fields"]["conso_a_tr"];
               resfinal+=j;}
           }
            cout<<"La consommation total a temps rèelle par les entreprises est:"<<resfinal<<endl;

            float  consotrtotale=resfinal+entfinal+profinal;
            float  pourcentage_conso_tr_ent=(entfinal/consotrtotale)*100;
            float  pourcentage_conso_tr_pro=profinal/consotrtotale*100;
            float  pourcentage_conso_tr_res=resfinal/consotrtotale*100;


           cout<<"Les entreprises consomment à température rèelle: "<<pourcentage_conso_tr_ent<<"% de la consommation française"<<endl;
           cout<<"Les professionnels consomment à température rèelle: "<<pourcentage_conso_tr_pro<<"% de la consommation française"<<endl;
           cout<<"Les résidents consomment à température rèelle: "<<pourcentage_conso_tr_res<<"% de la consommation française"<<endl;

           // variables pour graphiques
           float convres=(pourcentage_conso_tr_res/100)*360;
           float convent=(pourcentage_conso_tr_ent/100)*360;
           float convpro=(pourcentage_conso_tr_pro/100)*360;

           gdImagePtr im;
           FILE *pngout;
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


           int cor_rad = 150;

             gdImageFilledArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, 360, 360, white, gdPie);

             gdImageFilledArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, 360, convent, red, gdPie);
             gdImageFilledArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, convent, convent+convres, blue, gdPie);
             gdImageFilledArc (im,100 + cor_rad, 400 - cor_rad, cor_rad *2, cor_rad *2, convent+convres, convent+convpro+convres, orange, gdPie);


             gdImageEllipse(im,250,250,300,300,black);

             pngout = fopen("testprojet.png", "wb");
               gdImagePng(im, pngout);

              //Fermeture et nettoyage mémoire
               fclose(pngout);
               gdImageDestroy(im);

    return (EXIT_SUCCESS);
}
