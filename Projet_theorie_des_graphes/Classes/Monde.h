#ifndef MONDE_H_INCLUDED
#define MONDE_H_INCLUDED
#include "Aeroport.h"
#include <vector>
#include <fstream>
#include "Avion.h"


class Monde
{
    private:
        std::vector<Aeroport*>m_aeroports;
        std::vector<Avion*>m_avion;
    public:
        Monde(std::string nomFichier);
        std::vector<Aeroport*> getAeroports() const{return m_aeroports;};
        int trouveIdentification(std::string nomAeroport);
        void afficherMonde();
        void Dijkstra(const Aeroport* Depart, const Aeroport* Arrivee);
        void initialisationAeroport();
        void melangerAvion();
        void creationAvion();
        void afficherAeroport();
        ///----------------METHODES DE GESTION DE LA SIMULATION----------------///
        bool arrivee(Avion* _avion, Aeroport* destination);
};

#endif // MONDE_H_INCLUDED
