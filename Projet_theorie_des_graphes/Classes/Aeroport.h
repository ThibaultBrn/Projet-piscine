#ifndef AEROPORT_H_INCLUDED
#define AEROPORT_H_INCLUDED
#include <iostream>
#include "Avion.h"
#include <vector>
#include <queue>
#include <map>

class Aeroport
{
    private :
        std::string m_nom;
        std::pair<int, int> m_coordonnees;
        int m_identification;
        int m_nbPistes;
        int m_nbPlacesAuSol;
        int m_delaiAttenteSol;
        int m_tempsAccesPistes;
        int m_delaiAnticollision;
        int m_tempsDecollageAtterrissage;
        int m_dureeBoucleAttente;
        std::vector<std::pair<int,Aeroport*>>m_successeurs;
        std::vector<Avion*>m_AvionSol;
        std::queue<Avion*>m_AvionsTransitionDepart;
        std::queue<Avion*>m_AvionsTransitionArrivee;
    public :
        Aeroport(std::string _nom, std::pair<int,int>_coordonnees,int _nbPistes,int _nbPlacesAuSol,int _delaiAttenteSol,int _tempsAccesPistes,int _delaiAnticollision,int _tempsDecollageAtterrissage,int _dureeBoucleAttente, int _identification);
        std::string getNom() const {return m_nom;};
        int getIdentification() const {return m_identification;};
        std::vector<std::pair<int,Aeroport*>> getSuccesseurs() const {return m_successeurs;};
        int getNbPlacesSol(){return m_nbPlacesAuSol;};
        std::vector<Avion*> getAvionSol(){return m_AvionSol;};
        int getNbPistes(){return m_nbPistes;};
        void AjouterSucc(int poids, Aeroport* s);
        void setNbPlacesSol(int _nbPlacesSol){m_nbPlacesAuSol=_nbPlacesSol;};
        void setNbPistes(int _nbPistes){m_nbPistes=_nbPistes;};
        void SetAvionSol(Avion* _unAvion){m_AvionSol.push_back(_unAvion);};
        std::pair<int, int> getCoordonnes(){return m_coordonnees;};
        void AfficherAeroport();
};

#endif // AEROPORT_H_INCLUDED
