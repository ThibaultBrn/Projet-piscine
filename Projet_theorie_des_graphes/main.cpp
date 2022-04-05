#include <iostream>
#include "Classes/Avion.h"
#include "Classes/Aeroport.h"
#include "Classes/Monde.h"

using namespace std;

int main()
{
    std::vector<Aeroport*> aeroports;
    Monde m("Fichiers_txt/Monde.txt");
    m.afficherMonde();
    m.Dijkstra(m.getAeroports()[m.trouveIdentification("NewYork")], m.getAeroports()[m.trouveIdentification("Dubai")]);
    return 0;
}
