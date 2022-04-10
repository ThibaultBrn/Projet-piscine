#include "../Classes/Aeroport.h"
#include "../Classes/Avion.h"
#include <vector>

Aeroport::Aeroport(std::string _nom, std::pair<int,int>_coordonnees,int _nbPistes,int _nbPlacesAuSol,int _delaiAttenteSol,int _tempsAccesPistes,int _delaiAnticollision,int _tempsDecollageAtterrissage,int _dureeBoucleAttente, int _identification, std::string _typeMin)
{
    m_nom = _nom;
    m_coordonnees = _coordonnees;
    m_nbPistes = _nbPistes;
    m_nbPlacesAuSol = _nbPlacesAuSol;
    m_delaiAttenteSol = _delaiAttenteSol;
    m_tempsAccesPistes = _tempsAccesPistes;
    m_delaiAnticollision = _delaiAnticollision;
    m_tempsDecollageAtterrissage = _tempsDecollageAtterrissage;
    m_dureeBoucleAttente = _dureeBoucleAttente;
    m_identification = _identification;
    m_typeAvionMin = _typeMin;
}

void Aeroport::AjouterSucc(int poids, Aeroport* s)
{
    m_successeurs.push_back(std::make_pair(poids,s));
}

void Aeroport::AfficherAeroport()
{
    int i = 0;
    std::cout <<"/----------------------------/"<<std::endl;
    std::cout<<"Aeroport "<<m_nom<<" : " << std::endl;
    std::cout<<"    Coordonnees : "<<"X = "<<m_coordonnees.first<<" || Y = "<<m_coordonnees.second<<std::endl;
    std::cout<<"    Nombre de pistes : "<<m_nbPistes<<" "<< std::endl;
    std::cout<<"    Nombre de places au sol : "<<m_nbPlacesAuSol<<" "<< std::endl;
    std::cout<<"    Delai d'attente au sol : "<<m_delaiAttenteSol<<" "<< std::endl;
    std::cout<<"    Temps decollage-atterrissage : "<<m_tempsDecollageAtterrissage<<" "<< std::endl;
    std::cout<<"    Duree Boucle Attente : "<<m_dureeBoucleAttente<<" "<< std::endl;
    std::cout<<"    Delai transition stationnement / pistes : "<<m_tempsAccesPistes<<std::endl;
    for (auto s : m_successeurs)
    {
        i++;
        std::cout<<"        Successeur " << i <<  " -->"<<s.second->getNom()<<" "<< std::endl;
        std::cout<<"        Distance : "<<s.first<< std::endl;
    }
    std::cout<<"    /-----------AVIONS-----------/"<<std::endl<<"    Les avions au sol sont"<<std::endl;
    for(auto it : m_Avions)
    {
        it.first->AfficherAvions();
    }
}
