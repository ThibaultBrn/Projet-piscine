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
        std::string m_typeAvionMin;
        std::pair<int, int> m_coordonnees;
        int m_identification; ///Pour Dijkstra

        int m_nbPistes;
        int m_nbPlacesAuSol;
        int m_delaiAttenteSol;
        int m_tempsAccesPistes;
        int m_delaiAnticollision;
        int m_tempsDecollageAtterrissage;
        int m_dureeBoucleAttente;

        std::vector<std::pair<int,Aeroport*>>m_successeurs;
        std::vector<std::pair<int,Aeroport*>>m_successeurs_moyen;
        std::vector<std::pair<int,Aeroport*>>m_successeurs_court;
        std::vector<std::pair<Avion*, std::string>>m_Avions;
        std::queue<Avion*>m_fileAttentePistes;
        std::queue<Avion*>m_fileAttentePistesEnVol;
        std::queue<Avion*>m_AvionsTransitionPistes;
        std::queue<Avion*>m_AvionsTransitionAireStationnement;
    public :
        Aeroport(std::string _nom, std::pair<int,int>_coordonnees,int _nbPistes,int _nbPlacesAuSol,int _delaiAttenteSol,int _tempsAccesPistes,int _delaiAnticollision,int _tempsDecollageAtterrissage,int _dureeBoucleAttente, int _identification, std::string _typeMin);
        std::string getNom() const {return m_nom;};
        std::string getTypeAvionMin() const {return m_typeAvionMin;};
        int getIdentification() const {return m_identification;};
        std::vector<std::pair<int,Aeroport*>> getSuccesseurs() const {return m_successeurs;};
        std::vector<std::pair<int,Aeroport*>> getSuccesseursMoyen() const {return m_successeurs_moyen;};
        std::vector<std::pair<int,Aeroport*>> getSuccesseursCourt() const {return m_successeurs_court;};
        int getNbPlacesSol(){return m_nbPlacesAuSol;};
        std::vector<std::pair<Avion*,std::string>> getAvions(){return m_Avions;};
        int getNbPistes(){return m_nbPistes;};
        int getTempsDecollageAtterrissage(){return m_tempsDecollageAtterrissage;};
        int getDelaiAnticollision(){return m_delaiAnticollision;};
        int getTempsAccesPistes(){return m_tempsAccesPistes;};
        int getDelaiAttenteSol(){return m_delaiAttenteSol;};
        void incrNbPistes(){m_nbPistes++;};
        void decrNbPiste(){m_nbPistes--;};
        void incrNbPlacesAuSol(){m_nbPlacesAuSol++;};
        void decrNbPlacesAuSol(){m_nbPlacesAuSol--;};
        void effacerAvion(int position){m_Avions.erase(m_Avions.begin()+position);};
        std::queue<Avion*> getAvionsTransitionPistes(){return m_AvionsTransitionPistes;};
        std::queue<Avion*> getAvionsTransitionAireStationnement(){return m_AvionsTransitionAireStationnement;};
        std::queue<Avion*> getFileAttentePistes(){return m_fileAttentePistes;};
        std::queue<Avion*> getFileAttentePistesEnVol(){return m_fileAttentePistesEnVol;};
        void ajouterAvionTransiAireStationnement(Avion* _avion){m_AvionsTransitionAireStationnement.push(_avion);};
        void ajouterAvion(Avion* nouvelAvion, std::string action){m_Avions.push_back({nouvelAvion, action});};
        void ajouterAvionTransiPiste(Avion* nouvelAvion){m_AvionsTransitionPistes.push(nouvelAvion);};
        void retirerAvionTransiPiste(){m_AvionsTransitionPistes.pop();};
        void ajouterAvionFileAttPistesEnVol(Avion* nouvelAvion){m_fileAttentePistesEnVol.push(nouvelAvion);};
        void ajouterAvionFileAttPistes(Avion* nouvelAvion){m_fileAttentePistes.push(nouvelAvion);};
        void AjouterSucc(int poids, Aeroport* s);
        void AjouterSuccCourt(int poids, Aeroport* s){m_successeurs_court.push_back({poids, s});};
        void AjouterSuccMoyen(int poids, Aeroport* s){m_successeurs_moyen.push_back({poids, s});};;
        int getDAS(){return m_delaiAttenteSol;};
        int getTAP(){return m_tempsAccesPistes;};
        int getDAC(){return m_delaiAnticollision;};
        int getTDA(){return m_tempsDecollageAtterrissage;};
        int getDBA(){return m_dureeBoucleAttente;};
        std::pair<int, int> getCoordonnees(){return m_coordonnees;};
        void setCoordonnees(std::pair<int, int> _coordonnees){m_coordonnees=_coordonnees;};
        void setNbPlacesSol(int _nbPlacesSol){m_nbPlacesAuSol=_nbPlacesSol;};
        void setNbPistes(int _nbPistes){m_nbPistes=_nbPistes;};
        void SetAvions(Avion* _unAvion, std::string etat){m_Avions.push_back({_unAvion, etat});if(etat == "Stockage"){_unAvion->setStationnement(true);}else{_unAvion->setStationnement(false);}};
        void setIdentification(int identi){m_identification = identi;};
        void AfficherAeroport();
        ///----------------METHODES DE GESTION DE LA SIMULATION----------------///
        bool autorisationAtterrissage();
        void modifieAction(Avion* _avion, std::string nouvelleAction);
        void incrTempsTousLesAvions();
};

#endif // AEROPORT_H_INCLUDED
