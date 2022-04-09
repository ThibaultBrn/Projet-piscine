#include <iostream>
#include <time.h>
#include "Classes/Avion.h"
#include "Classes/Aeroport.h"
#include "Classes/Monde.h"


using namespace std;

int main()
{
        Avion* avionTst = new Avion("Chicots", "Long courrier", 10, 10, {15,1}, false);
        Avion* avionTst2 = new Avion("2Chicots", "Long courrier", 10, 10, {15,1}, false);
    int compteur = 0;
    srand(time(NULL));
    Monde m("Fichiers_txt/Monde.txt");
    m.initialisationAeroport();
        Vol* nvVol = new Vol(m.getAeroports()[m.trouveIdentification("Tokyo")], m.getAeroports()[m.trouveIdentification("Paris")], 15, {m.getAeroports()[m.trouveIdentification("Paris")]});
        Vol* nvVol2 = new Vol(m.getAeroports()[m.trouveIdentification("Tokyo")], m.getAeroports()[m.trouveIdentification("Paris")], 15, {m.getAeroports()[m.trouveIdentification("Paris")]});
        m.addTrajet(avionTst, nvVol);
        m.addAvion(avionTst);
        m.addTrajet(avionTst2, nvVol2);
        m.addAvion(avionTst2);
    m.afficherMonde();
    m.initPlansDeVolsAlea();
    m.afficheNouveauxVols();
    std::cout << " #############################################################################" << std::endl;
    std::cout << "################################## GESTION ####################################" << std::endl;
    std::cout << " #############################################################################" << std::endl;
    while(compteur != 25)
    {
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
        compteur++;
    }
    return 0;
}
