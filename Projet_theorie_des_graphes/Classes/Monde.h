#ifndef MONDE_H_INCLUDED
#define MONDE_H_INCLUDED
#include "Aeroport.h"
#include <vector>
#include <fstream>


class Monde
{
    private:
        std::vector<Aeroport*>m_aeroport;
        std::vector<Avion*>m_avion;
    public:
        //Monde(std::vector<Aeroport*>_aeroport, std::vector<Avion*>_avion);
        Monde(std::string nomFichier);

    int trouveIdentification(std::string nomAeroport);
    void afficherMonde();
};

#endif // MONDE_H_INCLUDED
