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
    std::cout<<"5. Lancer une simultation avec des nuages"<<std::endl;
    std::cout<<"6. Quitter"<<std::endl;
    while(choix<'1' || choix>'6')
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
    m.initPlansDeVolsAlea();
    m.initNuages();

    show_mouse(screen);
    blit(m.getBitmap(),screen,0,0,0,0,m.getBitmap()->w,m.getBitmap()->h);
    system("cls");
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
        m.testAvion(m.trouveAeroport("NYC"), m.trouveAeroport("SAN"));
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
            m.afficherMondeAllegro(buffer);
            compteur++;
        }
    }
    else if(choix=='5')
    {
        ///saisir nb nuages,couleur, aeroport arrive et depart
        do
        {
            std::cout<<"Saisir le nombre de nuages entre (1 et 9)"<<std::endl;
            std::cin>>nbNuages;

        }while(nbNuages<1 || nbNuages>9);
        int i=0;
        unsigned int a=0;
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
        }while(j<0 || j>int(a));
        depart=m.getAeroports()[j];
        int k=0;
        i=0;
        do
        {
            i=0;
            std::cout<<"Saisir le nom d'un aeroport d'arrivee"<<std::endl;
            for(auto elem : m.getAeroports())
            {
                std::cout<<i<<". "<<elem->getNom()<<std::endl;
                i++;
            }
            std::cin>>k;
        }while(k<0 || k>int(a) || k==j);
        arrivee=m.getAeroports()[k];

        int couleurNuage;
        do
        {
            std::cout<<"Saisir l'intensite du nuage"<<std::endl;
            std::cin>>couleurNuage;

        }while(couleurNuage<85 || couleurNuage>230);

        m.initNuagesTest(nbNuages);
    }

    return choix;
}
