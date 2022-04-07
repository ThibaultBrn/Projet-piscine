#include <iostream>
#include "Classes/Avion.h"
#include "Classes/Aeroport.h"
#include "Classes/Monde.h"


using namespace std;

int main()
{
    std::vector<Aeroport*> aeroports;
    Monde m("Fichiers_txt/Monde.txt");

    m.initialisationAeroport();

    m.afficherMonde();

    m.Dijkstra(m.getAeroports()[m.trouveIdentification("NYC")], m.getAeroports()[m.trouveIdentification("DUB")]);
    return 0;
}
