#ifndef AVION_H_INCLUDED
#define AVION_H_INCLUDED
#include <iostream>
#include "allegro.h"


class Avion
{
    private :
        std::string m_nom;
        std::string m_type;
        float m_consommation;
        int m_capacite_carburant;
        int m_carburant;
        std::pair<int, int> m_coordonneesAv;
        BITMAP* m_volatil;
    public :
        Avion(std::string _nom, std::string _type,int _consommation,int _capacite_carburant, std::pair<int, int> _coordonneesAv);
        std::string getNom(){return m_nom;};
        void AfficherAvions();
        void setCoordonnees(std::pair<int,int> _coordonneesAv){m_coordonneesAv=_coordonneesAv;};
        std::pair<int,int> getCoordonnees(){return m_coordonneesAv;};
        void setCarburant(int _carburant){m_carburant=_carburant;};
        int getCarburant(){return m_carburant;};
        int getConsomation(){return m_consommation;};
        int getCapacite(){return m_capacite_carburant;};
        std::string getType(){return m_type;};
        BITMAP* getImage(){return m_volatil;};


};

#endif // AVION_H_INCLUDED
