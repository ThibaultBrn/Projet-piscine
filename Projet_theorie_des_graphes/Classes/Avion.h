#ifndef AVION_H_INCLUDED
#define AVION_H_INCLUDED
#include <iostream>

class Avion
{
    private :
        int m_type;
        float m_consommation;
        int m_capacite_carburant;
    public :
        Avion(int _type,int _consommation,int _capacite_carburant);


};

#endif // AVION_H_INCLUDED
