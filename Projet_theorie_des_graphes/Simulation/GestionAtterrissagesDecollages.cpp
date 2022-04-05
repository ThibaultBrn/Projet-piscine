#include "../Classes/Monde.h"

bool Monde::arrivee(Avion* _avion, Aeroport* _destination)
{
    if((_avion->getCoordonnees().first  == _destination->getCoordonnes().first) && (_avion->getCoordonnees().second == _destination->getCoordonnes().second))
    {
        return true;
    }
    else
    {
        std::cout << "L'avion " << _avion->getNom() << " n'est pas encore arrive a destination." << std::endl;
        return false;
    }
}

bool Monde::atterrissage(Avion* _avion, Aeroport* _destination)
{
    if(arrivee(_avion, _destination))
    {
        if(_destination->getNbPlacesSol() == 0) ///L'AVION NE POURRA PAS ETRE STATIONNE, INUTILE DE L'ENVOYER
        {
            ///----LANCER LA BOUCLE D'ATTENTE EN VOL----///
        }
        else if((_destination->getNbPlacesSol() != 0) && (_destination->getNbPistes() != 0)) ///IL Y A DE LA PLACE DANS L'AIRE DE STATIONNEMENT MAIS PAS SUR LES PISTES, INUTILE D'ENVOYER L'AVION
        {
            _destination->setNbPlacesSol((_destination->getNbPlacesSol())-1); ///IL Y A DONC UNE PLACE EN MOINS AU SOL
            _destination->setNbPistes((_destination->getNbPistes())-1);

        }
    }
}
