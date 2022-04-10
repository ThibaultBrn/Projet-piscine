#include <iostream>
#include <time.h>
#include "Classes/Avion.h"
#include "Classes/Aeroport.h"
#include "Classes/Monde.h"
#include <allegro.h>

using namespace std;

int main()
{

    allegro_init();
    install_keyboard();
    install_mouse();

    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,1100,550,0,0)!=0)
    {
        allegro_message("prb gfx mode");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    ///Le code allegro
    int compteur = 0;
    BITMAP* buffer;
    buffer=create_bitmap(SCREEN_W,SCREEN_H);
    srand(time(NULL));
    Monde m("Fichiers_txt/Monde.txt");

    m.initialisationAeroport();
    //m.afficherMondeAllegro(aeroports[4],aeroports[10],avion[0]);
    ///s'arrete ici

    /*m.afficherMonde();
    m.initPlansDeVolsAlea();
    m.afficheNouveauxVols();
    std::cout << " #############################################################################" << std::endl;
    std::cout << "################################## GESTION ####################################" << std::endl;
    std::cout << " #############################################################################" << std::endl;
    while(compteur != 50)
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
    return 0;
    //m.BFS(m.getAeroports()[m.trouveIdentification("PAR")], m.getAvions()[0]);*/
    m.testAvion(m.trouveAeroport("NYC"), m.trouveAeroport("SAN"));
}END_OF_MAIN();
