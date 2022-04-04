#include "../Classes/Monde.h"
#include <queue>
#include <vector>

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
