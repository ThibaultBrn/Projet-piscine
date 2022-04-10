#include "../Classes/Monde.h"

void Monde::gestionMondialeAeroports()
{
    for(auto it : m_avion)
    {
        if(!m_trajets[it]->getPlanDeVol().empty())
        {
            if(it->getAeroportActuel() != m_trajets[it]->getPlanDeVol()[0]->getNom())
                isArrivee(it, m_trajets[it]->getPlanDeVol()[0]);
        }
    }
    for(auto it : m_aeroports)
    {
        gestionAeroport(it);
    }
}

bool Monde::isArrivee(Avion* _avion, Aeroport* _destination)
{
    if((_avion->getCoordonnees().first  == _destination->getCoordonnees().first) && (_avion->getCoordonnees().second == _destination->getCoordonnees().second))
    {
        std::cout<<"Arrivee "<<_destination->getNom()<<std::endl;
        _avion->setEnVol(false);
        _avion->setAeroportActuel(_destination->getNom());
        m_trajets[_avion]->setActuel(_destination);
        _destination->ajouterAvion(_avion, "Attente");
        _destination->ajouterAvionFileAttPistes(_avion);
        _destination->ajouterAvionFileAttPistesEnVol(_avion); ///UN AVION EST ARRIVE A L'AEROPORT, IL ATTEND DONC UNE PLACE EN PISTE
        m_trajets[_avion]->retirerEtapePlanDeVol();
        if((int(_destination->getFileAttentePistesEnVol().size()) > _destination->getNbPlacesSol())) ///L'AVION NE POURRA PAS ETRE STATIONNE, INUTILE DE L'ENVOYER
        {
            std::cout << "              >>L'avion " << _avion->getNom() << " entre en boucle d'attente en vol..." << std::endl;
            ///----MODIFIER LA CONSOMMATION DE L'AVION (MODE BOUCLE EN VOL)----///
            _avion->setConsommation(_avion->getConsomationParam()-1);
        }
        return true;
    }
    else
    {
        std::cout<<"Pas Arrivee "<<_destination->getNom()<<std::endl;
        std::cout << "              >>L'avion " << _avion->getNom() << " n'est pas encore arrive a destination." << std::endl;
        return false;
    }
}

void Aeroport::modifieAction(Avion* _avion, std::string nouvelleAction)
{
    for(std::vector<std::pair<Avion*, std::string>>::iterator it = std::begin(m_Avions); it != std::end(m_Avions); ++it)
    {
        if(it->first == _avion)
        {
            if(it->second == "Attente" && m_fileAttentePistes.front() == _avion) ///SI ON PASSE DE L'ATTENTE A L'ATTERRISSAGE
            {
                std::cout << "              >>L'avion " << it->first->getNom() << " etait en attente dans le ciel et au sommet de la file d'attente d'acces aux pistes" << std::endl;
                m_fileAttentePistes.pop();
                m_fileAttentePistesEnVol.pop();
            }
            else if(it->second == "TransitionPistes" && m_fileAttentePistes.front() == _avion) ///SI ON PASSE DE LA TRANSITION VERS LES PISTES AU DECOLLAGE
            {
                std::cout << "              >>L'avion " << it->first->getNom() << " etait en zone de transition et au sommet de la file d'attente d'acces aux pistes" << std::endl;
                m_fileAttentePistes.pop();
                m_AvionsTransitionPistes.pop();
            }
            else if(it->second == "TransitionStockage" && m_AvionsTransitionAireStationnement.front() == _avion) ///SI ON PASSE DE LA TRANSITION VERS LES AIRES DE STATIONNEMENT AU DECHARGEMENT
            {
                std::cout << "              >>L'avion " << it->first->getNom() << " etait en transition vers l'aire de stationnement et au sommet de la file d'attente" << std::endl;
                m_AvionsTransitionAireStationnement.pop();
            }
            else if(it->second == "Dechargement") ///L'AVION ETAIT EN PHASE DE DECHARGEMENT ET PASSE EN PHASE DE STOCKAGE
            {
                _avion->setStationnement(true);
            }
            it->second = nouvelleAction;
            _avion->resetTempsTraitement();
        }
    }
}

bool Aeroport::autorisationAtterrissage()
{
    Avion* avionTraitement = m_fileAttentePistesEnVol.front();
    if((m_nbPlacesAuSol <= 0) || (m_nbPistes <= 0)) ///L'AVION NE POURRA PAS ETRE STATIONNE OU N'AURA PAS DE PISTE, INUTILE DE L'ENVOYER
    {
        std::cout << "              >>L'avion " << avionTraitement->getNom() << " reste en boucle d'attente en vol." << std::endl;
        return false;
    }
    else if((m_nbPlacesAuSol > 0) && (m_nbPistes > 0)) ///IL Y A DE LA PLACE DANS L'AIRE DE STATIONNEMENT ET UNE PISTE LIBRE, ON PEUT ENVOYER L'AVION
    {
        m_nbPlacesAuSol -= 1; ///IL Y A DONC UNE PLACE EN MOINS AU SOL CAR ON VA FORCEMMENT STOCKER L'AVION
        m_nbPistes -= 1; ///L'AVION UTILISE DONC UNE PISTE
        modifieAction(avionTraitement, "Atterrissage");

        ///-------------------------CHANGER LE MODE DE CONSOMMATION----------------------///
        avionTraitement->setConsommation(avionTraitement->getConsomationParam()-2);

        std::cout << "              >>L'avion " << avionTraitement->getNom() << " entre en phase d'atterrissage." << std::endl;
        return true;
    }
    else
    {
        std::cout << "              >>Cas indetermine" << std::endl;
        return false;
    }
}

void Aeroport::incrTempsTousLesAvions()
{
    for(auto it : m_Avions)
        it.first->incrTempsTraitement();
}

void Monde::gestionAeroport(Aeroport* aeroport)
{
    int pos = 0;
    Avion* avionTraitement;
    std::cout << "--->Avant traitement, l'aeroport " << aeroport->getNom() << " a " << aeroport->getNbPlacesSol() << " places au sol. Et " << aeroport->getNbPistes() << " pistes." << std::endl;
    for(auto it : aeroport->getAvions()) ///ON PARCOURT TOUS LES AVIONS DE L'AEROPORT
    {
        avionTraitement = it.first;
        if(it.second == "Attente")
        {
            aeroport->autorisationAtterrissage();
        }
        else if(it.second == "Atterrissage")
        {
            if(avionTraitement->getTempsTraitement() >= aeroport->getTempsDecollageAtterrissage())
            {
                ///--------------COUPER LA CONSOMMATION-----------///
                avionTraitement->setConsommation(0);

                if((aeroport->getAvionsTransitionAireStationnement().empty()) || aeroport->getAvionsTransitionAireStationnement().back()->getTempsTraitement() >= aeroport->getDelaiAnticollision())
                {
                    std::cout << "              >>L'avion " << avionTraitement->getNom() << " a atteri a l'aeroport de " << aeroport->getNom() <<" et entre en transit vers la zone de stationnement." << std::endl;
                    aeroport->incrNbPistes();
                    aeroport->ajouterAvionTransiAireStationnement(avionTraitement);
                    aeroport->modifieAction(avionTraitement, "TransitionStockage");
                }
            }
        }
        else if(it.second == "Decollage")
        {
            if(avionTraitement->getTempsTraitement() >= aeroport->getTempsDecollageAtterrissage())
            {
                aeroport->incrNbPistes();
                aeroport->effacerAvion(pos);
                avionTraitement->setEnVol(true);
                avionTraitement->resetTempsTraitement();
                std::cout << "              >>L'avion " << avionTraitement->getNom() << " a quitte l'aeroport de " << aeroport->getNom() << " pour " << m_trajets[avionTraitement]->getPlanDeVol()[0]->getNom() << std::endl;
                ///--------------FAIRE CE QU'IL FAUT LORSQUE L'AVION PASSE EN MODE VITESSE DE CROISIERE ET COMMENCE A PARTIR VERS SON AEROPORT DESTINATION-----------///
                avionTraitement->setConsommation(avionTraitement->getConsomationParam());
            }
        }
        else if(it.second == "TransitionStockage")
        {
            if(avionTraitement->getTempsTraitement() >= aeroport->getTempsAccesPistes())
            {
                std::cout << "              >>L'avion " << avionTraitement->getNom() << " commence son dechargement a " << aeroport->getNom() << std::endl;
                aeroport->modifieAction(avionTraitement, "Dechargement");
            }
        }
        else if(it.second == "TransitionPistes")
        {
            if(avionTraitement->getTempsTraitement() >= aeroport->getTempsAccesPistes())
            {
                aeroport->retirerAvionTransiPiste();
                std::cout << "              >>L'avion " << avionTraitement->getNom() << " entre sur la piste de l'aeroport de " << aeroport->getNom() << " et se prepare a decoller." << std::endl;
                aeroport->modifieAction(avionTraitement, "Decollage");
                ///--------------MODIFIER LA CONSO DE L'AVION-----------///
                avionTraitement->setConsommation(avionTraitement->getConsomationParam()+2);

            }
        }
        else if(it.second == "Dechargement")
        {
            if(avionTraitement->getTempsTraitement() >= aeroport->getDelaiAttenteSol())
            {
                ///--------------RECHARGER LE CARBURANT DE L'AVION-----------///
                avionTraitement->setCarburant(avionTraitement->getCapacite());

                std::cout << "              >>L'avion " << avionTraitement->getNom() << " est stationne a l'aeroport de " << aeroport->getNom() << std::endl;
                aeroport->modifieAction(avionTraitement, "Stockage");
            }
        }
        else if(it.second == "Stockage")
        {
            if(!m_trajets[avionTraitement]->getPlanDeVol().empty()) ///SI L'AVION A UN AEROPORT A REJOINDRE...
            {
                if(aeroport->getNbPistes() > 0 && (aeroport->getAvionsTransitionPistes().empty() || (aeroport->getAvionsTransitionPistes().back()->getTempsTraitement() >= aeroport->getDelaiAnticollision())))
                {
                    std::cout << "              >>L'avion " << avionTraitement->getNom() << " entre dans la file d'acces aux pistes de " << aeroport->getNom() << std::endl;
                    aeroport->decrNbPiste();
                    aeroport->incrNbPlacesAuSol();
                    aeroport->modifieAction(avionTraitement, "TransitionPistes");
                    aeroport->ajouterAvionTransiPiste(avionTraitement);
                }
            }
            else
            {
                planDeVolAlea(avionTraitement);
                afficheVol(avionTraitement);
            }
        }
        pos++;
    }
}
