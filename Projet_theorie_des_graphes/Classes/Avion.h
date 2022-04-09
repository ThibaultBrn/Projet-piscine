#ifndef AVION_H_INCLUDED
#define AVION_H_INCLUDED
#include <iostream>

class Avion
{
    private :
        std::string m_nom;
        std::string m_type;
        float m_consommation;
        int m_capacite_carburant;
        std::pair<int, int> m_coordonneesAv;

        int m_tempsTraitement = 0;
        bool m_stationnement = true;
        std::string m_aeroportActuel = "";
    public :
        Avion(std::string _nom, std::string _type,int _consommation,int _capacite_carburant, std::pair<int, int> _coordonneesAv, bool stationnement);
        std::string getNom(){return m_nom;};
        std::string getAeroportActuel(){return m_aeroportActuel;};
        int getTempsTraitement(){return m_tempsTraitement;};
        bool getStationnement(){return m_stationnement;};
        void AfficherAvions();
        void resetTempsTraitement(){m_tempsTraitement = 0;};
        void incrTempsTraitement(){m_tempsTraitement += 1;};
        void setCoordonnees(std::pair<int,int> _coordonneesAv){m_coordonneesAv=_coordonneesAv;};
        void setStationnement(bool stationnement){m_stationnement = stationnement;};
        void setAeroportActuel(std::string aeroport){m_aeroportActuel = aeroport;};
        std::pair<int,int> getCoordonnees(){return m_coordonneesAv;};
};

#endif // AVION_H_INCLUDED
