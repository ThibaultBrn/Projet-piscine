#ifndef AEROPORT_H_INCLUDED
#define AEROPORT_H_INCLUDED

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
        std::vector <map<Aeroport*,int>> m_distance;
    public :


};

#endif // AEROPORT_H_INCLUDED
