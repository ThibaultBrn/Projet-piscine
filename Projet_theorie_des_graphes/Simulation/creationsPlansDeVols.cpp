#include "../Classes/Monde.h"

void Monde::initPlansDeVolsAlea()
{
    for(auto it : m_avion)
    {
        planDeVolAlea(it);
    }
}

void Monde::planDeVolTest(Avion* _avion, Aeroport* arrivee)
{
    Vol* vol;
    std::string dep;
    std::string arr;
    Aeroport* aerop = trouveAeroport(_avion->getAeroportActuel());
    std::vector<Aeroport*> compoConnexe = BFS(aerop, _avion);
    if(_avion->getStationnement())
    {
        dep = _avion->getAeroportActuel();
        if((aerop->getTypeAvionMin() == "long") && (_avion->getType() != "long"))
        {
            arr = dep;
            vol = new Vol(aerop, trouveAeroport(arr), 0, {});
        }
        else if((aerop->getTypeAvionMin() == "moyen") && (_avion->getType() == "court"))
        {
            arr = dep;
            vol = new Vol(aerop, trouveAeroport(arr), 0, {});
        }
        else{
            arr = arrivee->getNom();
            if(_avion->getType() == "court")
                vol = CreationPlanDeVol_COURT(dep, arr, compoConnexe.size());
            else if(_avion->getType() == "moyen")
                vol = CreationPlanDeVol_MOYEN(dep, arr, compoConnexe.size());
            else if(_avion->getType() == "long")
                vol = CreationPlanDeVol_LONG(dep, arr);
        }
        addTrajet(_avion, vol);
    }
}

void Monde::planDeVolAlea(Avion* _avion)
{
    Vol* vol;
    std::string dep;
    std::string arr;
    Aeroport* aerop = trouveAeroport(_avion->getAeroportActuel());
    std::vector<Aeroport*> compoConnexe = BFS(aerop, _avion);
    if(_avion->getStationnement())
    {
        dep = _avion->getAeroportActuel();
        if((aerop->getTypeAvionMin() == "long") && (_avion->getType() != "long"))
        {
            arr = dep;
            vol = new Vol(aerop, trouveAeroport(arr), 0, {});
        }
        else if((aerop->getTypeAvionMin() == "moyen") && (_avion->getType() == "court"))
        {
            arr = dep;
            vol = new Vol(aerop, trouveAeroport(arr), 0, {});
        }
        else{
            arr = compoConnexe[rand()%(compoConnexe.size())]->getNom();
            if(_avion->getType() == "court")
                vol = CreationPlanDeVol_COURT(dep, arr, compoConnexe.size());
            else if(_avion->getType() == "moyen")
                vol = CreationPlanDeVol_MOYEN(dep, arr, compoConnexe.size());
            else if(_avion->getType() == "long")
                vol = CreationPlanDeVol_LONG(dep, arr);
        }
        addTrajet(_avion, vol);
    }
}

std::vector<Aeroport*> Monde::BFS(Aeroport* depart, Avion* avion)
{
    int sommetInitial = depart->getIdentification();
    int typeAvion;
    if(avion->getType() == "court")
        typeAvion = 1;
    else if(avion->getType() == "moyen")
        typeAvion = 2;
    else if(avion->getType() == "long")
        typeAvion = 3;
    std::queue<Aeroport*> file_bfs; ///DECLARATION DE LA FILE
    std::vector<bool> sommets_explores (m_aeroports.size(), false); /// VECTEUR QUI STOCKERA LES SOMMETS GRIS ET NOIRS (DONC DEJA VISITES) POUR EVITER LES DOUBLONS LORS DU PARCOURS. ON INITIALISE TOUS SES CHAMPS A FALSE
    std::vector<Aeroport*> ordre_exploration; ///ON STOCKERA DANS CE VECTEUR LE RESULTAT (DANS L'ORDRE) DU PARCOURS EN LARGEUR

    ordre_exploration.push_back(depart);
    sommets_explores[sommetInitial] = true;
    for(auto it : depart->getSuccesseurs()) ///ON VA PARCOURIR LES SUCCESSEURS DU SOMMET INITIAL POUR LES ENFILER
    {
        if((it.first > 5) && (typeAvion == 1))
        {
            continue;
        }
        else if((it.first > 13) && (typeAvion == 2))
        {
            continue;
        }
        sommets_explores[it.second->getIdentification()] = true; ///ON NOTE COMME EXPLORE
        file_bfs.push(it.second); ///...ET ON ENFILE
    }
    if(file_bfs.empty())
    {
        ordre_exploration.clear();
        return ordre_exploration;
    }
    while(ordre_exploration.size() != m_aeroports.size()) ///TANT QUE TOUS LES SOMMETS NE SONT PAS EXPLORES...
    {
        if(file_bfs.front()->getSuccesseurs().size() != 0){
            for(auto it : file_bfs.front()->getSuccesseurs()) ///PARCOURS DES SUCCESSEURS DU SOMMET EN TETE DE FILE
            {
                if((it.first > 5) && (typeAvion == 1))
                {
                    continue;
                }
                else if((it.first > 13) && (typeAvion == 2))
                {
                    continue;
                }
                if(!sommets_explores[it.second->getIdentification()]) ///ON FAIT BIEN ATTENTION A NE PAS ENFILER UN SUCCESSEUR DEJA EXPLORE
                {
                    sommets_explores[it.second->getIdentification()] = true;
                    file_bfs.push(it.second); ///SI CA N'EST PAS LE CAS, ON L'ENFILE ET ON LE NOTE COMME EXPLORE
                }
            }
        }
        ordre_exploration.push_back(file_bfs.front()); ///ACTUALISATION DU VECTEUR CONTENANT LE RESULTAT DU PARCOURS
        file_bfs.pop(); ///...ET ON DEFILE LE SOMMET EN TETE DE FILE
        if(file_bfs.empty())
            break;
    }
    return ordre_exploration;
}

Vol* Monde::CreationPlanDeVol_LONG(std::string _Depart, std::string _Arrivee)
{
    Aeroport* Depart = m_aeroports[trouveIdentification(_Depart)];
    Aeroport* Arrivee = m_aeroports[trouveIdentification(_Arrivee)];
    /// INITIALISATION DE L'ALGO ///
    Aeroport* AeroportActuel = Depart;
    std::priority_queue<std::pair<int, Aeroport*>, std::vector<std::pair<int, Aeroport*>>, std::greater<std::pair<int, Aeroport*>>> distanceSuccesseurs; ///PRIORITY QUEUE D'ARCS POUR FACILEMENT RETROUVER LE PROCHAIN SOMMET LE PLUS PROCHE DU SOMMET INTIAL.
    std::vector<bool> aeroportsMarques(m_aeroports.size(), false);
    std::vector<Aeroport*> predecesseurs(m_aeroports.size(), NULL);
    std::vector<int> distancePlusCourtChemin(m_aeroports.size(), -1); ///-1 = PAS ENCORE TRAITE. SERAIT EGAL A INFINI SI ON SUIVAIT LE COURS.
    std::vector<Aeroport*> planDeVolTmp;
    std::vector<Aeroport*> planDeVol;
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
    if(Depart == NULL || Arrivee == NULL || Depart == Arrivee)
    {
        planDeVol.clear();
        distancePlusCourtChemin[Arrivee->getIdentification()] = 0;
    }
    else
    {
        AeroportActuel = Arrivee;
        while(AeroportActuel != Depart)
        {
            planDeVolTmp.push_back(AeroportActuel);
            AeroportActuel = predecesseurs[AeroportActuel->getIdentification()];
        }
    }
    while(!planDeVolTmp.empty()){
        planDeVol.push_back(planDeVolTmp.back());
        planDeVolTmp.pop_back();}
    Vol* VolCree = new Vol(Depart, Arrivee, distancePlusCourtChemin[Arrivee->getIdentification()], planDeVol);
    return VolCree;
}

Vol* Monde::CreationPlanDeVol_MOYEN(std::string _Depart, std::string _Arrivee, size_t tailleCC)
{
    Aeroport* Depart = m_aeroports[trouveIdentification(_Depart)];
    Aeroport* Arrivee = m_aeroports[trouveIdentification(_Arrivee)];
    /// INITIALISATION DE L'ALGO ///
    Aeroport* AeroportActuel = Depart;
    std::priority_queue<std::pair<int, Aeroport*>, std::vector<std::pair<int, Aeroport*>>, std::greater<std::pair<int, Aeroport*>>> distanceSuccesseurs; ///PRIORITY QUEUE D'ARCS POUR FACILEMENT RETROUVER LE PROCHAIN SOMMET LE PLUS PROCHE DU SOMMET INTIAL.
    std::vector<bool> aeroportsMarques(m_aeroports.size(), false);
    std::vector<Aeroport*> predecesseurs(m_aeroports.size(), NULL);
    std::vector<int> distancePlusCourtChemin(m_aeroports.size(), -1); ///-1 = PAS ENCORE TRAITE. SERAIT EGAL A INFINI SI ON SUIVAIT LE COURS.
    std::vector<Aeroport*> planDeVolTmp;
    std::vector<Aeroport*> planDeVol;
    size_t nbAeroportsMarques = 0;
    distancePlusCourtChemin[Depart->getIdentification()] = 0;       ///LA DISTANCE DU SOMMET INTIAL AU SOMMET INITIAL EST DE 0
    aeroportsMarques[Depart->getIdentification()] = true;
    nbAeroportsMarques++;
    for(auto it : Depart->getSuccesseursMoyen())
    {
        distancePlusCourtChemin[it.second->getIdentification()] = it.first; ///LA DISTANCE AU SOMMET DE DEPART EST JUSQU'A PRESENT LA PLUS COURTE
        predecesseurs[it.second->getIdentification()] = Depart;       ///LE SOMMET DE DEPART EST JUSQU'A PRESENT LE PREDECESSEUR
        distanceSuccesseurs.push(it);                            ///ON REMPLIT LA PRIORITY QUEUE AVEC LES SUCCESSEURS DU SOMMET D'ORIGINE
    }
    while(nbAeroportsMarques != tailleCC)
    {
        if(aeroportsMarques[distanceSuccesseurs.top().second->getIdentification()] == false) ///SI LE PROCHAIN SUCCESSEUR AUQUEL ON COMPTE ACCEDER N'EST PAS MARQUE
        {
            AeroportActuel = distanceSuccesseurs.top().second; ///ON CHANGE LE SOMMET ACTUEL AVEC LE SUCCESSEUR LE PLUS PROCHE
            aeroportsMarques[AeroportActuel->getIdentification()] = true; ///ON MARQUE LE SOMMET
            nbAeroportsMarques++; ///ON INCREMENTE LE COMPTEUR DE SOMMETS MARQUES
            for(auto it : AeroportActuel->getSuccesseursMoyen()) ///ON PARCOURT LES SUCCESSEURS DU SOMMET ACTUEL
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
    if(Depart == NULL || Arrivee == NULL || Depart == Arrivee)
    {
        planDeVol.clear();
        distancePlusCourtChemin[Arrivee->getIdentification()] = 0;
    }
    else
    {
        AeroportActuel = Arrivee;
        while(AeroportActuel != Depart)
        {
            planDeVolTmp.push_back(AeroportActuel);
            AeroportActuel = predecesseurs[AeroportActuel->getIdentification()];
        }
    }
    while(!planDeVolTmp.empty()){
        planDeVol.push_back(planDeVolTmp.back());
        planDeVolTmp.pop_back();}
    Vol* VolCree = new Vol(Depart, Arrivee, distancePlusCourtChemin[Arrivee->getIdentification()], planDeVol);
    return VolCree;
}

Vol* Monde::CreationPlanDeVol_COURT(std::string _Depart, std::string _Arrivee, size_t tailleCC)
{
    Aeroport* Depart = m_aeroports[trouveIdentification(_Depart)];
    Aeroport* Arrivee = m_aeroports[trouveIdentification(_Arrivee)];
    /// INITIALISATION DE L'ALGO ///
    Aeroport* AeroportActuel = Depart;
    std::priority_queue<std::pair<int, Aeroport*>, std::vector<std::pair<int, Aeroport*>>, std::greater<std::pair<int, Aeroport*>>> distanceSuccesseurs; ///PRIORITY QUEUE D'ARCS POUR FACILEMENT RETROUVER LE PROCHAIN SOMMET LE PLUS PROCHE DU SOMMET INTIAL.
    std::vector<bool> aeroportsMarques(m_aeroports.size(), false);
    std::vector<Aeroport*> predecesseurs(m_aeroports.size(), NULL);
    std::vector<int> distancePlusCourtChemin(m_aeroports.size(), -1); ///-1 = PAS ENCORE TRAITE. SERAIT EGAL A INFINI SI ON SUIVAIT LE COURS.
    std::vector<Aeroport*> planDeVolTmp;
    std::vector<Aeroport*> planDeVol;
    size_t nbAeroportsMarques = 0;
    distancePlusCourtChemin[Depart->getIdentification()] = 0;       ///LA DISTANCE DU SOMMET INTIAL AU SOMMET INITIAL EST DE 0
    aeroportsMarques[Depart->getIdentification()] = true;
    nbAeroportsMarques++;
    for(auto it : Depart->getSuccesseursCourt())
    {
        distancePlusCourtChemin[it.second->getIdentification()] = it.first; ///LA DISTANCE AU SOMMET DE DEPART EST JUSQU'A PRESENT LA PLUS COURTE
        predecesseurs[it.second->getIdentification()] = Depart;       ///LE SOMMET DE DEPART EST JUSQU'A PRESENT LE PREDECESSEUR
        distanceSuccesseurs.push(it);                            ///ON REMPLIT LA PRIORITY QUEUE AVEC LES SUCCESSEURS DU SOMMET D'ORIGINE
    }
    while(nbAeroportsMarques != tailleCC)
    {
        if(aeroportsMarques[distanceSuccesseurs.top().second->getIdentification()] == false) ///SI LE PROCHAIN SUCCESSEUR AUQUEL ON COMPTE ACCEDER N'EST PAS MARQUE
        {
            AeroportActuel = distanceSuccesseurs.top().second; ///ON CHANGE LE SOMMET ACTUEL AVEC LE SUCCESSEUR LE PLUS PROCHE
            aeroportsMarques[AeroportActuel->getIdentification()] = true; ///ON MARQUE LE SOMMET
            nbAeroportsMarques++; ///ON INCREMENTE LE COMPTEUR DE SOMMETS MARQUES
            for(auto it : AeroportActuel->getSuccesseursCourt()) ///ON PARCOURT LES SUCCESSEURS DU SOMMET ACTUEL
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
    if(Depart == NULL || Arrivee == NULL || Depart == Arrivee)
    {
        planDeVol.clear();
        distancePlusCourtChemin[Arrivee->getIdentification()] = 0;
    }
    else
    {
        AeroportActuel = Arrivee;
        while(AeroportActuel != Depart)
        {
            planDeVolTmp.push_back(AeroportActuel);
            AeroportActuel = predecesseurs[AeroportActuel->getIdentification()];
        }
    }
    while(!planDeVolTmp.empty()){
        planDeVol.push_back(planDeVolTmp.back());
        planDeVolTmp.pop_back();}
    Vol* VolCree = new Vol(Depart, Arrivee, distancePlusCourtChemin[Arrivee->getIdentification()], planDeVol);
    return VolCree;
}
