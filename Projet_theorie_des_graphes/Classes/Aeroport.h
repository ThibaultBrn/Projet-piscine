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
    public :
        Aeroport(std::string _nom, std::pair<int,int>_coordonnees,int _nbPistes,int _nbPlacesAuSol,int _delaiAttenteSol,int _tempsAccesPistes,int _delaiAnticollision,int _tempsDecollageAtterrissage,int _dureeBoucleAttente, int _identification);
        std::string getNom() const {return m_nom;};
        int getIdentification() const {return m_identification;};
        std::vector<std::pair<int,Aeroport*>> getSuccesseurs() const {return m_successeurs;};
        void AjouterSucc(int poids, Aeroport* s);
        void AfficherAeroport();
        int getNbPlacesSol(){return m_nbPlacesAuSol;};
        int getNbPistes(){return m_nbPistes;};
        int getDAS(){return m_delaiAttenteSol;};
        int getTAP(){return m_tempsAccesPistes;};
        int getDAC(){return m_delaiAnticollision;};
        int getTDA(){return m_tempsDecollageAtterrissage;};
        int getDBA(){return m_dureeBoucleAttente;};

        void setNbPlacesSol(int _nbPlacesSol);
        std::vector<Avion*> getAvionSol();
        void SetAvionSol(Avion* _unAvion);
        std::pair<int, int> getCoordonnes(){return m_coordonnees;};
};

#endif // AEROPORT_H_INCLUDED
