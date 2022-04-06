#ifndef AVION_H_INCLUDED
#define AVION_H_INCLUDED
#include <iostream>

class Avion
{
    private :
        std::string m_nom;
        std::string m_type;
        float m_consommation;
        int m_capacite_carburant;
        std::pair<int, int> m_coordonneesAv;

        int m_tempsTraitement = 0;

        int m_transition; ///DEPUIS COMBIEN DE TEMPS L'AVION EST IL EN TRANSITION VERS L'AIRE DE STATIONNEMENT OU LES PISTES. 0 = PAS EN TRANSITION
        bool m_stationnement; ///L'AVION EST IL EN STATIONNEMENT ?

    public :
        Avion(std::string _nom, std::string _type,int _consommation,int _capacite_carburant, std::pair<int, int> _coordonneesAv);
        std::string getNom(){return m_nom;};
        int getTempsTraitement(){return m_tempsTraitement;};
        void AfficherAvions();
        void resetTempsTraitement(){m_tempsTraitement = 0;};
        void incrTempsTraitement(){m_tempsTraitement += 1;};
        void setCoordonnees(std::pair<int,int> _coordonneesAv){m_coordonneesAv=_coordonneesAv;};
        std::pair<int,int> getCoordonnees(){return m_coordonneesAv;};
};

#endif // AVION_H_INCLUDED
