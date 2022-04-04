#include "../Classes/Avion.h"

Avion::Avion(std::string _nom,int _type,int _consommation,int _capacite_carburant)
{
    m_nom=_nom;
    m_type = _type;
    m_consommation = _consommation;
    m_capacite_carburant = _capacite_carburant;
}

