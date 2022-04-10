#include "../Classes/Avion.h"

Avion::Avion(std::string _nom, std::string _type,int _consommation,int _consommationParam,int _capacite_carburant, std::pair<int, int> _coordonneesAv, bool stationnement)
{
    BITMAP* imageChargee;
    imageChargee=load_bitmap("images_allegro/avion.bmp",NULL);
    m_volatil=imageChargee;
    if(!m_volatil)
    {
        allegro_message("Impossible de trouver la carte !");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    m_nom=_nom;
    m_type = _type;
    m_consommation = _consommation;
    m_consommationParam = _consommationParam;
    m_capacite_carburant = _capacite_carburant;
    m_carburant=_capacite_carburant;
    m_coordonneesAv = _coordonneesAv;
    m_stationnement = stationnement;
}

void Avion::AfficherAvions()
{
    std::cout<<"    > Avion "<<m_nom<<" : " << std::endl;
    std::cout<<"     > Type "<<m_type<<" : " << std::endl;
    std::cout<<"        Coordonnees : "<<"X = "<<m_coordonneesAv.first<<" || Y = "<<m_coordonneesAv.second<<std::endl;
    std::cout<<"        Consommation : "<<m_consommation<<" "<< std::endl;
    std::cout<<"        Capacite de carburant : "<<m_capacite_carburant<<" "<< std::endl;
    std::cout<<"        En stationnement : ";
    if(m_stationnement){
        std::cout << "Oui" << std::endl;
        std::cout << "        Aeroport : " << m_aeroportActuel << std::endl;}
    else
        std::cout << "Non" << std::endl;
}
