#include "../Classes/Monde.h"

bool Monde::arrivee(Avion* _avion, Aeroport* _destination)
{
    if((_avion->getCoordonnees().first  == _destination->getCoordonnes().first) && (_avion->getCoordonnees().second == _destination->getCoordonnes().second))
    {
        _destination->getFileAttentePistesEnVol().push(_avion);
        _destination->getFileAttentePistes().push(_avion); ///UN AVION EST ARRIVE A L'AEROPORT, IL ATTEND DONC UNE PLACE EN PISTE
        if((int(_destination->getFileAttentePistesEnVol().size()) > _destination->getNbPlacesSol()) || (int(_destination->getFileAttentePistes().size()) > _destination->getNbPistes())) ///L'AVION NE POURRA PAS ETRE STATIONNE, INUTILE DE L'ENVOYER
        {
            _destination->getAvions().push_back({_avion, "Attente"});
            _avion->resetTempsTraitement();
            std::cout << "L'avion " << _avion->getNom() << " entre en boucle d'attente en vol..." << std::endl;
            /**----LANCER LA BOUCLE D'ATTENTE EN VOL----///
            >>
            >>
            >>
            **/
        }
        return true;
    }
    else
    {
        std::cout << "L'avion " << _avion->getNom() << " n'est pas encore arrive a destination." << std::endl;
        return false;
    }
}

void Aeroport::modifieAction(Avion* _avion, std::string nouvelleAction)
{
    for(auto it : m_Avions)
    {
        if(it.first == _avion)
            it.second = nouvelleAction;

        _avion->resetTempsTraitement();
    }
}

bool Aeroport::autorisationAtterrissage()
{
    Avion* avionTraitement = m_fileAttentePistesEnVol.front();
    if((m_nbPlacesAuSol == 0) || (m_nbPistes == 0) || (int(m_AvionsTransitionPistes.size()) == m_nbPistes)) ///L'AVION NE POURRA PAS ETRE STATIONNE OU N'AURA PAS DE PISTE, INUTILE DE L'ENVOYER
    {
        std::cout << "L'avion " << avionTraitement->getNom() << " reste en boucle d'attente en vol." << std::endl;
        return false;
    }
    else if((m_nbPlacesAuSol > 0) && (m_nbPistes > 0) && (m_AvionsTransitionPistes.size() == 0)) ///IL Y A DE LA PLACE DANS L'AIRE DE STATIONNEMENT ET UNE PISTE LIBRE, ON PEUT ENVOYER L'AVION
    {
        m_nbPlacesAuSol -= 1; ///IL Y A DONC UNE PLACE EN MOINS AU SOL CAR ON VA FORCEMMENT STOCKER L'AVION
        m_nbPistes -= 1; ///L'AVION UTILISE DONC UNE PISTE
        m_fileAttentePistesEnVol.pop();
        m_fileAttentePistes.pop();
        modifieAction(avionTraitement, "Atterrissage");

        /**-------------------------CHANGER LE MODE DE CONSOMMATION----------------------///
        <<
        <<
        <<
        **/
        std::cout << "L'avion " << avionTraitement->getNom() << " entre en phase d'atterrissage." << std::endl;
        return true;
    }
    else
    {
        std::cout << "Cas indetermine" << std::endl;
        return false;
    }
}

void Aeroport::gestionAeroport()
{
    int pos = 0;
    Avion* avionTraitement;
    for(auto it : m_Avions) ///ON PARCOURT TOUS LES AVIONS DE L'AEROPORT
    {
        avionTraitement = it.first;
        avionTraitement->incrTempsTraitement(); ///UNE UNITE DE TEMPS EST ECOULEE
        if(it.second == "Attente")
        {
            autorisationAtterrissage();
        }
        else if(it.second == "Atterrissage")
        {
            if(avionTraitement->getTempsTraitement() >= m_tempsDecollageAtterrissage)
            {
                /**-------------TESTER S'IL Y A DE LA PLACE POUR QUITTER LA PISTE-----------///
                >>
                >>
                >>
                **/
            }
        }
        else if(it.second == "Decollage")
        {
            if(avionTraitement->getTempsTraitement() >= m_tempsDecollageAtterrissage)
            {
                m_nbPistes++;
                m_Avions.erase(m_Avions.begin()+pos);
                /**--------------FAIRE CE QU'IL FAUT LORSQUE L'AVION PASSE EN MODE VITESSE DE CROISIERE-----------///
                >>
                >>
                >>
                **/
            }
        }
        else if(it.second == "TransitionStockage")
        {
            if(avionTraitement->getTempsTraitement() >= m_tempsAccesPistes)
            {
                m_nbPistes--;
                modifieAction(avionTraitement, "Dechargement");
            }
        }
        else if(it.second == "TransitionPistes")
        {

        }
        else if(it.second == "Dechargement")
        {

        }
        else if(it.second == "Stockage")
        {

        }
        pos++;
    }
}
