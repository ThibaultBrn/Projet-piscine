#ifndef AEROPORT_H_INCLUDED
#define AEROPORT_H_INCLUDED
#include <iostream>
#include "Avion.h"
#include <vector>
#include <map>

class Aeroport
{
    private :
        std::string m_nom;
        std::pair<int, int> m_coordonnees;
        int m_nbPistes;
        int m_nbPlacesAuSol;
        int m_delaiAttenteSol;
        int m_tempsAccesPistes;
        int m_delaiAnticollision;
        int m_tempsDecollageAtterrissage;
        int m_dureeBoucleAttente;
        std::vector <std::map<Aeroport*,int>> m_distance;
        std::vector<Avion*>m_avionSole;
    public :
        Aeroport(std::string _nom,std::pair<int,int>_coordonnees,int _nbPistes,int _nbPlacesAuSol,int _delaiAttenteSol,int _tempsAccesPistes,int _delaiAnticollision,int _tempsDecollageAtterrissage,int _dureeBoucleAttente,std::vector <std::map<Aeroport*,int>> _distance,std::vector<Avion*>_avionSole);
};

#endif // AEROPORT_H_INCLUDED
