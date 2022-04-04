#ifndef MONDE_H_INCLUDED
#define MONDE_H_INCLUDED

#include "Aeroport.h"
#include <vector>

class Monde
{
    private:
        std::vector<Aeroport*>m_aeroports;
        std::vector<Avion*>m_avions;
    public:
        int trouveIdentification(std::string nomAeroport);
        void Dijkstra(const Aeroport* Depart, const Aeroport* Arrivee);
};

#endif // MONDE_H_INCLUDED
