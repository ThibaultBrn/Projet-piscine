#ifndef MONDE_H_INCLUDED
#define MONDE_H_INCLUDED

#include "Aeroport.h"
#include <vector>

class Monde
{
    private:
        std::vector<Aeroport*>m_aeroports;
        std::vector<Avion*>m_avion;
    public:
        //Monde(std::vector<Aeroport*>_aeroport, std::vector<Avion*>_avion);
        Monde(std::string nomFichier);
        std::vector<Aeroport*> getAeroports() const{return m_aeroports;};
        int trouveIdentification(std::string nomAeroport);
        void afficherMonde();
        void Dijkstra(const Aeroport* Depart, const Aeroport* Arrivee);
};

#endif // MONDE_H_INCLUDED
