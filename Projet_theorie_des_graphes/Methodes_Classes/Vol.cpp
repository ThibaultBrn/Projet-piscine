#include "../Classes/Vol.h"

void Vol::afficheVol()
{
    std::cout << "------] Vol de " << m_depart->getNom() << " a destination de " << m_arrivee->getNom() << " [------" << std::endl;
    std::cout << "     Distance (UT) : " << m_dureeTrajet << std::endl;
    std::cout << "     Le vol passera par les aeroports suivants : " << std::endl;
    if(!m_planDeVol.empty()){
        for(auto it : m_planDeVol){
            if(it != NULL)
                std::cout << "              ->" << it->getNom() << std::endl;
            else
                std::cout << "       Pas d'aeroport, nous restons a " << m_depart->getNom() << std::endl;
        }
    }
    else
        std::cout << "       Pas d'aeroport, nous restons a " << m_depart->getNom() << std::endl;
    std::cout << std::endl;
}
