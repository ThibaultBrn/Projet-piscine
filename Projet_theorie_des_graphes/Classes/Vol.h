#ifndef VOL_H_INCLUDED
#define VOL_H_INCLUDED

#include "Aeroport.h"

class Vol
{
private :
    Aeroport* m_depart;
    Aeroport* m_arrivee;
    int m_dureeTrajet;
    std::vector<Aeroport*> m_planDeVol;

public :
    Vol(Aeroport* _depart, Aeroport* _arrivee, int _duree, std::vector<Aeroport*> _pdv):m_depart(_depart), m_arrivee(_arrivee), m_dureeTrajet(_duree), m_planDeVol(_pdv){};
    Aeroport* getDepart(){return m_depart;};
    Aeroport* getArrivee(){return m_arrivee;};
    int getDuree(){return m_dureeTrajet;};
    std::vector<Aeroport*> getPlanDeVol(){return m_planDeVol;};
    void afficheVol();
};

#endif // VOL_H_INCLUDED
