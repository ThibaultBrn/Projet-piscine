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
        std::pair<int, int> m_coordonneesAv;
        BITMAP* m_volatil;
    public :
        Avion(std::string _nom, std::string _type,int _consommation,int _capacite_carburant, std::pair<int, int> _coordonneesAv);
        std::string getNom(){return m_nom;};
        void AfficherAvions();
        void setCoordonnees(std::pair<int,int> _coordonneesAv){m_coordonneesAv=_coordonneesAv;};
};

#endif // AVION_H_INCLUDED
