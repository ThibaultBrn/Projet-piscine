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
    m.Dijkstra(m.getAeroports()[m.trouveIdentification("CharlesDeGaulle")], m.getAeroports()[m.trouveIdentification("NewYork")]);
    return 0;
}
