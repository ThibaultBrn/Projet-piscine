#include "../Classes/Monde.h"


int Monde::trouveIdentification(std::string nomAeroport)
{
    for(auto it : m_aeroport)
    {
        if(nomAeroport == it->getNom())
            return it->getId();
    }
    std::cout << "Aucun aeroport trouve..." << std::endl;
    return -1; ///SI ON EST ARRIVE JUSQU'ICI C'EST QUE L'AEROPORT QUE L'ON RECHERCHE N'EXISTE PAS.
}

Monde::Monde(std::string nomFichier)///Recuperation du graphe
{
    std::ifstream ifs{nomFichier};
    if (!ifs)
        throw std::runtime_error( "Impossible d'ouvrir en lecture " + nomFichier );
    int ordre;
    ifs >> ordre;
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture ordre du graphe");

    std::string nom;
    int coorX, coorY, nbPistes, nbPlacesAuSol, delaiAttenteSol, tempsAccesPistes, delaiAntiCollision, tempsDecollageAtterrissage, dureeBoucleAttente;
    for (int i=0; i<ordre; ++i)///récupération des sommets
    {
        ifs>>nom>>coorX>>coorY>>nbPistes>>nbPlacesAuSol>>delaiAttenteSol>>tempsAccesPistes>>delaiAntiCollision>>tempsDecollageAtterrissage>>dureeBoucleAttente;
        m_aeroport.push_back( new Aeroport(nom,std::make_pair(coorX,coorY),nbPistes,nbPlacesAuSol, delaiAttenteSol, tempsAccesPistes, delaiAntiCollision, tempsDecollageAtterrissage, dureeBoucleAttente,i));
    }


    int taille;
    ifs >> taille;///récupération de la taille
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture taille du graphe");

    std::string aer1,aer2;
    int num1, num2, poids;
    for (int i=0;i<taille;++i){///taille
        ifs>>aer1>>aer2>>poids;
        if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture arc");

        ///fonction qui va chercher le numero correspondant au nom de l'aeroport
        num1 = trouveIdentification(aer1);
        num2 = trouveIdentification(aer2);

        m_aeroport[num1]->AjouterSucc(poids, m_aeroport[num2]);
        m_aeroport[num2]->AjouterSucc(poids, m_aeroport[num1]);
    }
}

void Monde::afficherMonde()
{
    std::cout<<std::endl<<"Voila notre monde ";
    std::cout<<"de taille = "<<m_aeroport.size()<<std::endl<<"  ";
    std::cout<<"liste des aeroports :"<<std::endl;
    for (auto s : m_aeroport){
            s->AfficherAeroport();
            std::cout<<std::endl;
    }
}

