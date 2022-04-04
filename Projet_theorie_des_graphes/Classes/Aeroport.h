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
        int m_identification;
        std::vector <std::map<Aeroport*,int>> m_distance;
        std::vector<std::pair<int,Aeroport*>>m_successeurs;
    public :
        Aeroport(std::string _nom,std::pair<int,int>_coordonnees,int _nbPistes,int _nbPlacesAuSol,int _delaiAttenteSol,int _tempsAccesPistes,int _delaiAnticollision,int _tempsDecollageAtterrissage,int _dureeBoucleAttente, int _identification);

        void AjouterSucc(int poids, Aeroport*s);

        int getId() const {return m_identification;};
        std::string getNom() const {return m_nom;};
        std::pair<int, int> getCoor() const{return m_coordonnees;};
        int getNbPistes() const{return m_nbPistes;};
        int getNbPlacesAuSol() const{return m_nbPlacesAuSol;};
        int getDelaiAttenteSol() const{return m_delaiAttenteSol;};
        int getTDA() const{return m_tempsDecollageAtterrissage;};
        int getDureeBA() const{return m_dureeBoucleAttente;};

        std::vector<std::pair<int,Aeroport*>> getSuccesseurs () const {return m_successeurs;};

        void AfficherAeroport();

};

#endif // AEROPORT_H_INCLUDED
