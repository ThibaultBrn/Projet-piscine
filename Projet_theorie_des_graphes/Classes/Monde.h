#ifndef MONDE_H_INCLUDED
#define MONDE_H_INCLUDED
#include "Aeroport.h"
#include <vector>
#include <map>
#include <fstream>
#include <allegro.h>
#include "Avion.h"
#include "Vol.h"
#include "Nuage.h"

class Monde
{
    private:
        std::vector<Aeroport*>m_aeroports;
        std::vector<Avion*>m_avion;
        BITMAP* m_carte;
        std::map<Avion*, Vol*>m_trajets;
        std::vector<Nuage*>m_nuages;
        double m_tempsDebut;
    public:
        Monde(std::string nomFichier);
        std::vector<Aeroport*> getAeroports() const{return m_aeroports;};
        std::vector<Avion*> getAvions() const{return m_avion;};
        std::map<Avion*, Vol*> getTrajets() const{return m_trajets;};
        void addAvion(Avion* nouveauAvion){m_avion.push_back(nouveauAvion);};
        int trouveIdentification(std::string nomAeroport);
        void afficherMonde();
        double getTempsDebut(){return m_tempsDebut;};
        void setTempsDebut(double temps){m_tempsDebut = temps;};
        Vol* CreationPlanDeVol_LONG(std::string _Depart, std::string _Arrivee);
        Vol* CreationPlanDeVol_MOYEN(std::string _Depart, std::string _Arrivee, size_t tailleCC);
        Vol* CreationPlanDeVol_COURT(std::string _Depart, std::string _Arrivee, size_t tailleCC);
        Vol* getTrajet(Avion* _avion){return m_trajets[_avion];};
        void addTrajet(Avion* _avion, Vol* _vol){m_trajets[_avion] = _vol;};
        void initialisationAeroport();
        void melangerAvion();
        void creationAvion();
        void afficherAeroport();
        BITMAP* getBitmap(){return m_carte;};
        void deplacementAvion(Aeroport* _depart,Aeroport* _arrivee,Avion* _avion);
        void afficherMondeAllegro(BITMAP * monde);
        void afficherMondeAllegroTEST(BITMAP * monde, Aeroport* _depart, Aeroport* _arrivee, int coulNuage);
        void afficherAvionAllegro(Aeroport* _depart,Aeroport* _arrivee,Avion* _avion,BITMAP* monde);
        void afficherNuageAllegro(BITMAP* monde, Nuage* _nuage);
        void afficherNuageAllegroTEST(BITMAP* monde, Nuage* _nuage, Aeroport* _depart, Aeroport* _arrivee, int coulNuage);



        void fuiteReservoir(Aeroport* _depart,Aeroport* _arrivee,Avion* _avion,int poid);
        Aeroport* trouveAeroport(std::string nom);
        ///----------------METHODES DE GESTION DE LA SIMULATION----------------///
        std::vector<Aeroport*> BFS(Aeroport* depart, Avion* avion);
        bool isArrivee(Avion* _avion, Aeroport* destination);
        void gestionAeroport(Aeroport* aeroport);
        void initPlansDeVolsAlea();
        void planDeVolAlea(Avion* _avion);
        void planDeVolTest(Avion* _avion, Aeroport* arrivee);
        void afficheNouveauxVols();
        void gestionMondialeAeroports();
        void afficheVol(Avion* unAvion);
        void initNuages();
        void initNuagesReglable(int nbNuages);


        void csqNuage(Nuage* _nuage, Avion* _avion);
        ///-----------------------TEST AVEC UN SEUL AVION---------------------///
        void testAvion();
};

#endif // MONDE_H_INCLUDED
