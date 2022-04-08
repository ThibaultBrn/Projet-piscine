#include <iostream>
#include <time.h>
#include "Classes/Avion.h"
#include "Classes/Aeroport.h"
#include "Classes/Monde.h"


using namespace std;

int main()
{
    int compteur = 0;
    srand(time(NULL));
    Monde m("Fichiers_txt/Monde.txt");
    m.initialisationAeroport();
    m.afficherMonde();
    m.plansDeVolsAlea();
    m.afficheNouveauxVols();
    std::cout << "#############################################################################" << std::endl;
    std::cout << "##################################GESTION####################################" << std::endl;
    std::cout << "#############################################################################" << std::endl;
    while(compteur != 15)
    {
        std::cout << "E1" << std::endl;
        m.gestionMondialeAeroports();
        for(auto it : m.getAeroports())
        {
            for(auto it2 : it->getAvions())
            {
                std::cout << "-----------Avion : " << it2.first->getNom() << std::endl;
                std::cout << "---------------Action : " << it2.second << std::endl;
            }
        }
        compteur++;
    }
    return 0;
}
