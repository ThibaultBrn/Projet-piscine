#ifndef AVION_H_INCLUDED
#define AVION_H_INCLUDED
#include <iostream>
#include "allegro.h"


class Avion
{
    private :
        std::string m_nom;
        std::string m_type;
        int m_consommation;
        int m_consommationParam;
        int m_capacite_carburant;
        int m_carburant;
        std::pair<int, int> m_coordonneesAv;
        BITMAP* m_volatil;
        int m_tempsTraitement = 0;
        bool m_stationnement = true;
        std::string m_aeroportActuel = "";
        bool m_enVol=false;
    public :
        Avion(std::string _nom, std::string _type,int _consommation,int _consommationParam,int _capacite_carburant, std::pair<int, int> _coordonneesAv, bool stationnement);
        std::string getNom(){return m_nom;};
        std::string getAeroportActuel(){return m_aeroportActuel;};
        int getTempsTraitement(){return m_tempsTraitement;};
        bool getStationnement(){return m_stationnement;};
        void AfficherAvions();
        void resetTempsTraitement(){m_tempsTraitement = 0;};
        void incrTempsTraitement(){m_tempsTraitement += 1;};
        void setCoordonnees(std::pair<int,int> _coordonneesAv){m_coordonneesAv=_coordonneesAv;};
        std::pair<int,int> getCoordonnees(){return m_coordonneesAv;};
        void setCarburant(int _carburant){m_carburant=_carburant;};
        int getCarburant(){return m_carburant;};
        int getConsomation(){return m_consommation;};
        int getConsomationParam(){return m_consommationParam;};
        int getCapacite(){return m_capacite_carburant;};
        std::string getType(){return m_type;};
        BITMAP* getImage(){return m_volatil;};
        void setStationnement(bool stationnement){m_stationnement = stationnement;};
        void setAeroportActuel(std::string aeroport){m_aeroportActuel = aeroport;};
        bool getEnVol(){return m_enVol;};
        void setEnVol(bool _enVol){m_enVol=_enVol;};
        void setConsommation(int _consommation){m_consommation=_consommation;};
};

#endif // AVION_H_INCLUDED
