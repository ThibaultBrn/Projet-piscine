#include "../Classes/Aeroport.h"

Aeroport::Aeroport(std::string _nom, std::pair<int,int>_coordonnees,int _nbPistes,int _nbPlacesAuSol,int _delaiAttenteSol,int _tempsAccesPistes,int _delaiAnticollision,int _tempsDecollageAtterrissage,int _dureeBoucleAttente,std::vector <std::map<Aeroport*,int>> _distance,std::vector<Avion*>_avionSole)
{
    m_avionSole = _avionSole;
    m_nom = _nom;
    m_coordonnees = _coordonnees;
    m_nbPistes = _nbPistes;
    m_nbPlacesAuSol = _nbPlacesAuSol;
    m_delaiAttenteSol = _delaiAttenteSol;
    m_tempsAccesPistes = _tempsAccesPistes;
    m_delaiAnticollision = _delaiAnticollision;
    m_tempsDecollageAtterrissage = _tempsDecollageAtterrissage;
    m_dureeBoucleAttente = _dureeBoucleAttente;
    m_distance = _distance;
}
