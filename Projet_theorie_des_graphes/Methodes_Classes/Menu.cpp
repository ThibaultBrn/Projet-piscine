#include "../Classes/Menu.h"
#include "../Classes/Monde.h"
#include <fstream>
#include <iostream>
#include <allegro.h>
#include <cstdlib>

Menu::Menu()
{

}

char Menu::afficherMenu()
{
    char choix='0';
    std::cout<<"--------------------Menu--------------------"<<std::endl;
    std::cout<<std::endl;
    std::cout<<std::endl;
    std::cout<<"1. Afficher les avions"<<std::endl;
    std::cout<<"2. Afficher les aeroports"<<std::endl;
    std::cout<<"3. Trouver le chemin le plus court entre deux aeroports"<<std::endl;
    std::cout<<"4. Lancer Une simulation"<<std::endl;
    std::cout<<"5. Lancer une simultation avec un nuage sur une trajectoire precise"<<std::endl;
    std::cout<<"6. Lancer une simulation en reglant le nombre de nuages"<<std::endl;
    std::cout<<"7. Quitter"<<std::endl;
    while(choix<'1' || choix>'7')
    {
        std::cout<<"Saisir une action"<<std::endl;
        std::cin>>choix;
    }
    return choix;
}

char Menu::gestionMenu()
{
    char choix='0';
    int compteur = 0;
    int nbNuages=0;
    Aeroport* depart;
    Aeroport* arrivee;
    BITMAP* buffer;
    buffer=create_bitmap(SCREEN_W,SCREEN_H);
    srand(time(NULL));
    Monde m("Fichiers_txt/Monde.txt");

    m.initialisationAeroport();
//    m.afficherMondeAllegro(aeroports[4],aeroports[10],avion[0]);
    ///s'arrete ici

    m.initPlansDeVolsAlea();
    blit(m.getBitmap(),screen,0,0,0,0,m.getBitmap()->w,m.getBitmap()->h);
    choix=afficherMenu();



    if(choix=='1')
    {
        for(auto it : m.getAvions())
        {
            it->AfficherAvions();

        }
    }
    else if(choix=='2')
    {
        for(auto it : m.getAeroports())
        {
            it->AfficherAeroport();
        }
    }
    else if(choix=='3')
    {
        ///djikstra
    }
    else if(choix=='4')
    {
        while(compteur != 100)
        {
            for(auto elem : m.getAvions())
            {
                elem->incrTempsTraitement();
            }
            std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << compteur << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
            m.gestionMondialeAeroports();
            for(auto it : m.getAeroports())
            {
                for(auto it2 : it->getAvions())
                {
                    std::cout << "-----------Avion : " << it2.first->getNom() << std::endl;
                    std::cout << "---------------Action : " << it2.second << std::endl;
                }
            }
            m.initNuages();
            m.afficherMondeAllegro(buffer);
            compteur++;
        }
    }
    else if(choix=='5')
    {
        int i=0;
        unsigned int a=0;

        ///saisir couleur, aeroport arrive et depart
        a=m.getAeroports().size();
        int j=0;

        do
        {
            i=0;
            std::cout<<"Saisir le nom de l'aeroport de depart"<<std::endl;
            for(auto elem : m.getAeroports())
            {
                std::cout<<i<<". "<<elem->getNom()<<std::endl;
                i++;
            }
            std::cin>>j;
        }while(j<0 || j>a);
        depart=m.getAeroports()[j];

        int k=0;
        i=0;
        a=depart->getSuccesseurs().size();
        do
        {
            i=0;
            std::cout<<"Saisir le nom d'un aeroport d'arrivee"<<std::endl;
            for(auto elem : depart->getSuccesseurs())
            {
                std::cout<<i<<". "<<elem.second->getNom()<<std::endl;
                i++;
            }
            std::cin>>k;
        }while(k<0 || k>a || k==j);
        arrivee=depart->getSuccesseurs()[k].second;

        int couleurNuage;
        do
        {
            std::cout<<"Saisir l'intensite du nuage (entre 85 et 230)"<<std::endl;
            std::cin>>couleurNuage;

        }while(couleurNuage<85 || couleurNuage>230);

        m.initNuages();

        while(compteur != 100)
        {
            for(auto elem : m.getAvions())
            {
                elem->incrTempsTraitement();
            }
            std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << compteur << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
            m.gestionMondialeAeroports();
            for(auto it : m.getAeroports())
            {
                for(auto it2 : it->getAvions())
                {
                    std::cout << "-----------Avion : " << it2.first->getNom() << std::endl;
                    std::cout << "---------------Action : " << it2.second << std::endl;
                }
            }
            m.afficherMondeAllegroTEST(buffer, depart, arrivee, couleurNuage);
            compteur++;
        }
    }
    else if(choix=='6')
    {
        unsigned int a=0;
        do
        {
            std::cout<<"Saisir le nombre de nuages entre (1 et 9)"<<std::endl;
            std::cin>>nbNuages;

        }while(nbNuages<1 || nbNuages>9);


        a=m.getAeroports().size();
        m.initNuagesReglable(nbNuages);
        while(compteur != 100)
        {
            for(auto elem : m.getAvions())
            {
                elem->incrTempsTraitement();
            }
            std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << compteur << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
            m.gestionMondialeAeroports();
            for(auto it : m.getAeroports())
            {
                for(auto it2 : it->getAvions())
                {
                    std::cout << "-----------Avion : " << it2.first->getNom() << std::endl;
                    std::cout << "---------------Action : " << it2.second << std::endl;
                }
            }


            m.afficherMondeAllegro(buffer);
            compteur++;
        }

    }

    return choix;
}
