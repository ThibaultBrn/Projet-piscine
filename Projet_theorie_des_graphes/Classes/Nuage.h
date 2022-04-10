#ifndef NUAGE_H_INCLUDED
#define NUAGE_H_INCLUDED
#include "Aeroport.h"
#include <vector>
#include <map>
#include <fstream>
#include <allegro.h>
#include "Avion.h"
#include "Vol.h"


class Nuage
{
private:
    std::pair<int, int> m_nuageX;
    std::pair<int, int>m_nuageY;
    int m_couleurNuage;
    std::pair<int, int>m_vitNuage;

public :
    Nuage();

    std::pair<int, int> getNX(){return m_nuageX;};
    std::pair<int, int> getNY(){return m_nuageY;};
    int getCouleurN(){return m_couleurNuage;};
    std::pair<int, int> getVitesseN(){return m_vitNuage;};

    void setNX( std::pair<int ,int> _nuageX){m_nuageX=_nuageX;};
    void setNY( std::pair<int ,int> _nuageY){m_nuageY=_nuageY;};
    void setCouleurN(int _couleurNuage){m_couleurNuage=_couleurNuage;};

    void afficherNuage();
};

#endif // NUAGE_H_INCLUDED
