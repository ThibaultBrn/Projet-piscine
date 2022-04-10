#include "../Classes/Aeroport.h"
#include "../Classes/Avion.h"
#include "../Classes/Nuage.h"
#include <vector>

Nuage::Nuage()
{
    int xN1, xN2, yN1, yN2;
    xN1 = rand()% 1050; ///nuage de taille maximale 50 en x donc depasserait la taille du monde si 1100

    //std::cout<<"\nxN1 = "<<xN1<<std::endl;
    xN2 = rand()% (xN1+50 - xN1+10+1)+ (xN1+10); /// (max - min+1) + min -> max = xN1+50 ; min = xN1+10

    //std::cout<<"xN2 = "<<xN2<<std::endl;

    yN1 = rand()% 500; ///nuage de taille maximale 50 en y donc depasserait la taille du monde si 550

    //std::cout<<"yN1 = "<<yN1<<std::endl;

    yN2 = rand()% (yN1+50 - yN1+10+1)+ (yN1+10);

    //std::cout<<"yN2 = "<<yN2<<std::endl<<std::endl;


    m_nuageX=std::make_pair(xN1,xN2);
    m_nuageY=std::make_pair(yN1,yN2);
    m_couleurNuage=rand()%(230-85)+85;
    m_vitNuage=std::make_pair((rand()%10-10+9),(rand()%10-10+9));
}

void Nuage::afficherNuage()
{
    std::cout<<"Coordonnees x1 : "<<m_nuageX.first<<std::endl;
    std::cout<<"Coordonnees x2 : "<<m_nuageX.second<<std::endl;
    std::cout<<"Coordonnees y1 : "<<m_nuageY.first<<std::endl;
    std::cout<<"Coordonnees y2 : "<<m_nuageY.second<<std::endl;
    std::cout<<"Couleur du nuage : "<<m_couleurNuage<<std::endl;
    std::cout<<"Deplacement en x : "<<m_vitNuage.first<<std::endl;
    std::cout<<"Deplacement en y : "<<m_vitNuage.second<<std::endl;
}
