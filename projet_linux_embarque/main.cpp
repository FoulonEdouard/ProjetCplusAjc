#include"loader.h"
#include"nlohmann/json.hpp"
#include<fstream>
#include<iostream>


using json = nlohmann::json;
using namespace std;

int main(int argc, char** argv) {

    Loader loader;
    loader.load("https://www.data.gouv.fr/fr/datasets/r/ecb94e56-22d4-40ee-bf40-c7890569d5bd","/home/edouard/projet_linux_embarque/data.JSON");

    //récupère le fichier et crée un parse JSON

    ifstream f("/home/edouard/projet_linux_embarque/data.JSON");
    json data = json::parse(f);

    //Lectuure dans le JSON

    int i;
    /*for(i=0; i<=131;i++)
    {
    cout << "la population est:  "<<setw(4)<<data[i]["fields"]["segment_client"]<<endl;
    }

    for(i=0; i<=131;i++)
    {
    cout << "La conso a trla semaine numéro "<<i<<" est: "<<setw(4)<<data[i]["fields"]["conso_a_tr"]<<endl;
    }
    */
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

    return (EXIT_SUCCESS);
}
