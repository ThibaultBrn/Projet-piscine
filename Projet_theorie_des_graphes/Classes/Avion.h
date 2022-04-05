#ifndef AVION_H_INCLUDED
#define AVION_H_INCLUDED
#include <iostream>

class Avion
{
    private :
        std::string m_nom;
        int m_type;
        float m_consommation;
        int m_capacite_carburant;
    public :
        Avion(std::string _nom,int _type,int _consommation,int _capacite_carburant);
        std::string getNom();


};

#endif // AVION_H_INCLUDED
