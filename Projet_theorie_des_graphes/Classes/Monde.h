#ifndef MONDE_H_INCLUDED
#define MONDE_H_INCLUDED
#include "Aeroport.h"
#include <vector>
#include <map>
#include <fstream>
#include "Avion.h"
#include "Vol.h"

class Monde
{
    private:
        std::vector<Aeroport*>m_aeroports;
        std::vector<Avion*>m_avion;
        std::map<Avion*, Vol*>m_trajets;
    public:
        Monde(std::string nomFichier);
        std::vector<Aeroport*> getAeroports() const{return m_aeroports;};
        std::vector<Avion*> getAvions() const{return m_avion;};
        int trouveIdentification(std::string nomAeroport);
        void afficherMonde();
        Vol* CreationPlanDeVol(std::string _Depart, std::string _Arrivee);
        Vol* getVol(Avion* _avion){return m_trajets[_avion];};
        void addVol(Avion* _avion, Vol* _vol){m_trajets[_avion] = _vol;};
        void initialisationAeroport();
        void melangerAvion();
        void creationAvion();
        void afficherAeroport();

        ///----------------METHODES DE GESTION DE LA SIMULATION----------------///
        bool arrivee(Avion* _avion, Aeroport* destination);
        void gestionAeroport(Aeroport* aeroport);
        void plansDeVolsAlea();
        void afficheNouveauxVols();
        void gestionMondialeAeroports();

};

#endif // MONDE_H_INCLUDED
