#include "../Classes/Monde.h"
#include <queue>
#include <vector>
#include <fstream>


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
        m_aeroports.push_back( new Aeroport(nom,std::make_pair(coorX,coorY),nbPistes,nbPlacesAuSol, delaiAttenteSol, tempsAccesPistes, delaiAntiCollision, tempsDecollageAtterrissage, dureeBoucleAttente,i));
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

        m_aeroports[num1]->AjouterSucc(poids, m_aeroports[num2]);
        m_aeroports[num2]->AjouterSucc(poids, m_aeroports[num1]);
    }
}

void Monde::afficherMonde()
{
    std::cout<<std::endl<<"Voici notre monde :" << std::endl;
    std::cout<<"Taille : "<<m_aeroports.size()<<std::endl;
    std::cout<<"Liste des aeroports :"<<std::endl<<std::endl;
    for (auto s : m_aeroports){
            s->AfficherAeroport();
            std::cout<<std::endl;
    }
}

int Monde::trouveIdentification(std::string nomAeroport)
{
    for(auto it : m_aeroports)
    {
        if(nomAeroport == it->getNom())
            return it->getIdentification();
    }
    std::cout << "Aucun aeroport trouve..." << std::endl;
    return -1; ///SI ON EST ARRIVE JUSQU'ICI C'EST QUE L'AEROPORT QUE L'ON RECHERCHE N'EXISTE PAS.
}

void Monde::Dijkstra(const Aeroport* Depart, const Aeroport* Arrivee)
{
    /// INITIALISATION DE L'ALGO ///
    const Aeroport* AeroportActuel = Depart;
    std::priority_queue<std::pair<int, const Aeroport*>, std::vector<std::pair<int, const Aeroport*>>, std::greater<std::pair<int, const Aeroport*>>> distanceSuccesseurs; ///PRIORITY QUEUE D'ARCS POUR FACILEMENT RETROUVER LE PROCHAIN SOMMET LE PLUS PROCHE DU SOMMET INTIAL.
    std::vector<bool> aeroportsMarques(m_aeroports.size(), false);
    std::vector<const Aeroport*> predecesseurs(m_aeroports.size(), NULL);
    std::vector<int> distancePlusCourtChemin(m_aeroports.size(), -1); ///-1 = PAS ENCORE TRAITE. SERAIT EGAL A INFINI SI ON SUIVAIT LE COURS.
    size_t nbAeroportsMarques = 0;
    distancePlusCourtChemin[Depart->getIdentification()] = 0;       ///LA DISTANCE DU SOMMET INTIAL AU SOMMET INITIAL EST DE 0
    aeroportsMarques[Depart->getIdentification()] = true;
    nbAeroportsMarques++;
    for(auto it : Depart->getSuccesseurs())
    {
        distancePlusCourtChemin[it.second->getIdentification()] = it.first; ///LA DISTANCE AU SOMMET DE DEPART EST JUSQU'A PRESENT LA PLUS COURTE
        predecesseurs[it.second->getIdentification()] = Depart;       ///LE SOMMET DE DEPART EST JUSQU'A PRESENT LE PREDECESSEUR
        distanceSuccesseurs.push(it);                            ///ON REMPLIT LA PRIORITY QUEUE AVEC LES SUCCESSEURS DU SOMMET D'ORIGINE
    }

    while(nbAeroportsMarques != m_aeroports.size())
    {
        if(aeroportsMarques[distanceSuccesseurs.top().second->getIdentification()] == false) ///SI LE PROCHAIN SUCCESSEUR AUQUEL ON COMPTE ACCEDER N'EST PAS MARQUE
        {
            AeroportActuel = distanceSuccesseurs.top().second; ///ON CHANGE LE SOMMET ACTUEL AVEC LE SUCCESSEUR LE PLUS PROCHE
            aeroportsMarques[AeroportActuel->getIdentification()] = true; ///ON MARQUE LE SOMMET
            nbAeroportsMarques++; ///ON INCREMENTE LE COMPTEUR DE SOMMETS MARQUES
            for(auto it : AeroportActuel->getSuccesseurs()) ///ON PARCOURT LES SUCCESSEURS DU SOMMET ACTUEL
            {
                if((distancePlusCourtChemin[AeroportActuel->getIdentification()] + it.first < distancePlusCourtChemin[it.second->getIdentification()]) && (distancePlusCourtChemin[it.second->getIdentification()] != 1)) ///SI ON VIENT DE TROUVER UN CHEMIN PLUS RAPIDE POUR ATTEINDRE LE SUCCESSEUR
                {
                    distancePlusCourtChemin[it.second->getIdentification()] = distancePlusCourtChemin[AeroportActuel->getIdentification()] + it.first; ///ALORS ON ACTUALISE LA DISTANCE
                    predecesseurs[it.second->getIdentification()] = AeroportActuel; ///ET ON NOTE LE PREDECESSEUR
                }
                else if(distancePlusCourtChemin[it.second->getIdentification()] == -1) ///-1 = INFINI
                {
                    distancePlusCourtChemin[it.second->getIdentification()] = distancePlusCourtChemin[AeroportActuel->getIdentification()] + it.first; ///DONC COMME LA DISTANCE ETAIT INFINIE, ON AVAIT PAS ENCORE TROUVE DE CHEMIN POUR ATTEINDRE LE SOMMET. ON VIENT PAR CONSEQUENT DE TROUVER LE CHEMIN LE PLUS EFFICACE (JUSQU'A PRESENT) POUR ATTEINDRE CE NOUVEAU SOMMET
                    predecesseurs[it.second->getIdentification()] = AeroportActuel;
                }
                distanceSuccesseurs.push(std::make_pair(distancePlusCourtChemin[it.second->getIdentification()], it.second)); ///ON AJOUTE LE PREDECESSEUR QU'ON VIENT D'EXLPORER A LA PRIORITY QUEUE
            }
        }
        else ///SI LE SUCCESSEUR LE PLUS PROCHE EST DEJA MARQUE, ON TESTE LE SUIVANT APRES AVOIR DEFILE LE PREMIER...
        {
            distanceSuccesseurs.pop();
        }
    }
    if(Depart == NULL || Arrivee == NULL)
    {
        std::cout << "Taille du plus court chemin : 0... Le sommet de depart et le sommet d'arrivee sont les memes." << std::endl;
    }
    else
    {
        AeroportActuel = Arrivee;
        std::cout << "Taille du plus court chemin : " << distancePlusCourtChemin[AeroportActuel->getIdentification()] << std::endl;
        while(AeroportActuel != Depart)
        {
            std::cout << AeroportActuel->getIdentification() << " <- ";
            AeroportActuel = predecesseurs[AeroportActuel->getIdentification()];
        }
        std::cout << Depart->getIdentification() << std::endl << std::endl;
    }
}
