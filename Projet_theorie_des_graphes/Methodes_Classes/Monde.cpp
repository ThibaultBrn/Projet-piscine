#include "../Classes/Monde.h"
#include <queue>
#include <vector>
#include <fstream>
#include <math.h>
#include <time.h>


Monde::Monde(std::string nomFichier)///Recuperation du graphe
{
    BITMAP* imageChargee;

    std::ifstream ifs{nomFichier};
    if (!ifs)
        throw std::runtime_error( "Impossible d'ouvrir en lecture " + nomFichier );

    ///--------------RECUPERATION DES DONNEES DES AEROPORTS----------------------
    int ordre;
    ifs >> ordre;
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture ordre du graphe");

    std::string nom;
    int coorX, coorY, nbPistes, nbPlacesAuSol, delaiAttenteSol, tempsAccesPistes, delaiAntiCollision, tempsDecollageAtterrissage, dureeBoucleAttente;
    for (int i=0; i<ordre; ++i)///récupération des sommets
    {
        ifs>>nom>>coorX>>coorY>>nbPistes>>nbPlacesAuSol>>delaiAttenteSol>>tempsAccesPistes>>delaiAntiCollision>>tempsDecollageAtterrissage>>dureeBoucleAttente;
        m_aeroports.push_back( new Aeroport(nom,std::make_pair(coorX,coorY),nbPistes,nbPlacesAuSol, delaiAttenteSol, tempsAccesPistes, delaiAntiCollision, tempsDecollageAtterrissage, dureeBoucleAttente,i));
    }


    int taille;
    ifs >> taille;///récupération de la taille
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture taille du graphe");

    std::string aer1,aer2;
    int num1, num2, poids;
    for (int i=0;i<taille;++i){///taille
        ifs>>aer1>>aer2>>poids;
        if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture arc");

        ///fonction qui va chercher le numero correspondant au nom de l'aeroport

        num1 = trouveIdentification(aer1);
        num2 = trouveIdentification(aer2);
        m_aeroports[num1]->AjouterSucc(poids, m_aeroports[num2]);

        m_aeroports[num2]->AjouterSucc(poids, m_aeroports[num1]);

    }

    ///--------------RECUPERATION DES DONNEES DES AVIONS----------------------
    int nbAvions;
    ifs >> nbAvions;
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture nb d'avions");

    std::string nomAvion;
    std::string typeAvion;
    float consommation;
    int capacite_carburant;
    for (int i=0; i<nbAvions; ++i)
    {
        ifs>>nomAvion>>typeAvion>>consommation>>capacite_carburant;
        m_avion.push_back( new Avion(nomAvion,typeAvion, consommation,consommation, capacite_carburant, std::make_pair(0,0), false));
    }

    ///--------------RECUPERATION DES IMAGES----------------------


    imageChargee=load_bitmap("images_allegro/carte_du_monde_avec_villes_et_trajets.bmp",NULL);
    m_carte=imageChargee;

    if(!m_carte)
    {
        allegro_message("Impossible de trouver la carte !");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
}

void Monde::afficherMonde()
{

    std::cout<<"Voici notre monde :" << std::endl;
    std::cout<<"Nombre d'aeroports : "<<m_aeroports.size()<<std::endl;
    std::cout<<"Nombre d'avions : "<<m_avion.size()<<std::endl;
    std::cout<<"Liste des aeroports :"<<std::endl<<std::endl;
    for (auto s : m_aeroports){
            s->AfficherAeroport();
            std::cout<<std::endl;
    }

    std::cout<<"Liste des avions :"<<std::endl<<std::endl;
    for (auto s : m_avion){
            s->AfficherAvions();
            std::cout<<std::endl;
    }
}

int Monde::trouveIdentification(std::string nomAeroport)
{
    for(auto it : m_aeroports)
    {
        if(nomAeroport == it->getNom())
        {
            return it->getIdentification();

        }
    }
    std::cout << "Aucun aeroport trouve..." << std::endl;
    return -1; ///SI ON EST ARRIVE JUSQU'ICI C'EST QUE L'AEROPORT QUE L'ON RECHERCHE N'EXISTE PAS.
}

Vol* Monde::CreationPlanDeVol(std::string _Depart, std::string _Arrivee)
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

void Monde::melangerAvion()///melange du vecteur d'avion
{
    std::vector<Avion*>planeStocker;
    std::vector<Avion*>PlaneMelange;

    int alea=0;
    planeStocker=m_avion;
    unsigned int nombreAvion=planeStocker.size();
    for(unsigned int i=0;i<nombreAvion;i++)
    {
        alea=rand()%(planeStocker.size());
        PlaneMelange.push_back(planeStocker[alea]);
        planeStocker.erase(planeStocker.begin()+alea);
    }
    m_avion=PlaneMelange;
}

void Monde::initialisationAeroport()
{
    int nbAvions=m_avion.size();
    int nbAirports = m_aeroports.size();
    unsigned int compteur=0;
    int cmptAvions = 0;
    int compteAeroports = 0;
    std::vector<std::pair<Avion*,std::string>> lesAvions;
    std::vector<Aeroport*>airports;
    std::vector<int>placesAEffacer;
    airports=m_aeroports;
    melangerAvion();
    m_aeroports.clear();
    for(int j = 0 ; j < nbAirports ; j++)
    {
        for(int i = 0 ; i < int(airports.size()) ; i++)
        {
            if(airports[i]->getNbPlacesSol() == 0) ///GESTION DES AEROPORTS N'AYANT PLUS DE PLACE
            {
                airports[i]->setIdentification(compteAeroports);
                compteAeroports++;
                m_aeroports.push_back(airports[i]);
                airports.erase(airports.begin()+i);
            }
        }
    }
    for(int i=0;i<nbAvions;i++)
    {
        if(airports[compteur]->getNbPlacesSol()!=0)
        {
            airports[compteur]->SetAvions(m_avion[i],"Stockage");
            airports[compteur]->decrNbPlacesAuSol();
            m_avion[i]->setAeroportActuel(airports[compteur]->getNom());
        }
        else
        {
            m_aeroports.push_back(airports[compteur]);
            airports[compteur]->setIdentification(compteAeroports);
            compteAeroports++;
            airports.erase(airports.begin()+compteur);
        }
        compteur++;
        if(int(airports.size()) == 0) ///IL N'Y A PLUS D'AEROPORTS DISPONIBLES
        {
            for(auto it : m_avion)
            {
                if(it->getAeroportActuel() == "")
                {
                    cmptAvions = 0;
                    for(auto it2 : m_avion)
                    {
                        if(it2->getNom() == it->getNom())
                        {
                            m_avion.erase(m_avion.begin()+cmptAvions);
                        }
                        cmptAvions++;
                    }
                }
            }
            break;
        }
        compteur %= airports.size();
    }
    for(unsigned int i=0;i<airports.size();i++)
    {
        airports[i]->setIdentification(compteAeroports);
        compteAeroports++;
        m_aeroports.push_back(airports[i]);
    }
    for(unsigned int i=0;i<m_aeroports.size();i++)
    {
        lesAvions=m_aeroports[i]->getAvions();
        for(unsigned int j=0;j<lesAvions.size();j++)
        {
            lesAvions[j].first->setCoordonnees(m_aeroports[i]->getCoordonnees());
        }
    }
}

void Monde::initPlansDeVolsAlea()
{
    for(auto it : m_avion)
    {
        planDeVolAlea(it);
    }
}

void Monde::planDeVolAlea(Avion* _avion)
{
    std::string dep;
    std::string arr;
    if(_avion->getStationnement())
    {
        dep = _avion->getAeroportActuel();
        arr = m_aeroports[rand()%(int(m_aeroports.size()))]->getNom();
        Vol* vol = CreationPlanDeVol(dep, arr);
        addTrajet(_avion, vol);
    }
}

void Monde::afficheVol(Avion* unAvion)
{
    std::cout << "==============================================================" << std::endl;
    std::cout << "==============================================================" << std::endl;

    if(unAvion->getStationnement())
    {
        std::cout << "          >>" << unAvion->getNom() << "<<" << std::endl;
        m_trajets[unAvion]->afficheVol();
        std::cout << "==============================================================" << std::endl;
        std::cout << "==============================================================" << std::endl;
    }

}

void Monde::afficheNouveauxVols()
{
    std::cout << "==============================================================" << std::endl;
    std::cout << "==============================================================" << std::endl;
    for(auto it : m_avion)
    {
        if(it->getStationnement())
        {
            std::cout << "          >>" << it->getNom() << "<<" << std::endl;
            m_trajets[it]->afficheVol();
            std::cout << "==============================================================" << std::endl;
            std::cout << "==============================================================" << std::endl;
        }
    }
}

void Monde::deplacementAvion(Aeroport* _depart,Aeroport* _arrivee,Avion* _avion)
{
    float m=0.0;
    float p=0.0;
    int deplacement=0;
    int poid=0;
    std::pair<int,int>coordonneesAvion;
    coordonneesAvion=_avion->getCoordonnees();
    m=(float(_arrivee->getCoordonnees().second)-float(_depart->getCoordonnees().second))/(float(_arrivee->getCoordonnees().first)-float(_depart->getCoordonnees().first));
    p=_depart->getCoordonnees().second-m*_depart->getCoordonnees().first;
    for(auto elem : _depart->getSuccesseurs())
    {
        if(elem.second==_arrivee)
            poid=elem.first;
    }


    if(_depart->getCoordonnees().first<_arrivee->getCoordonnees().first)
    {

        deplacement=(_arrivee->getCoordonnees().first-_depart->getCoordonnees().first)/poid;
        coordonneesAvion.first=coordonneesAvion.first+deplacement;
        coordonneesAvion.second=m*coordonneesAvion.first+p;
    }
    else
    {
        deplacement=(_depart->getCoordonnees().first-_arrivee->getCoordonnees().first)/poid;
        coordonneesAvion.first=coordonneesAvion.first-deplacement;
        coordonneesAvion.second=m*coordonneesAvion.first+p;
    }
    _avion->setCoordonnees(coordonneesAvion);
    _avion->setCarburant(_avion->getCarburant() - 10 * _avion->getConsomation());///à regarder
    std::cout<<"le carburant par tour : "<<_avion->getCarburant()<<std::endl;
}

void Monde::afficherMondeAllegro(BITMAP * monde)
{
///-----------------------GRILLE-------------------------------
    clear_bitmap(monde);
    blit(m_carte,monde,0,0,0,0,m_carte->w,m_carte->h);

    int y=20;
    int x=20;
    if(key[KEY_SPACE])
    {
        for(int i=0; i<30; i++)
        {
            hline(monde, 0, y, 1100, makecol(200, 200, 200));
            y+=20;
        }
        for(int i=0; i<60; i++)
        {
            vline(monde, x, 0, 550, makecol(200, 200, 200));
            x+=20;
        }
    }

    for (auto elem:m_nuages)
    {
        Monde::afficherNuageAllegro(monde, elem );
    }
    for(auto it:m_avion)
    {
        std::cout<<"                                                        "<<it->getNom()<<" temps de traitement : "<<it->getTempsTraitement()<<std::endl;
        if(it->getEnVol())
        {
            std::cout<<"l'avion "<<it->getNom()<<" est en vol "<<std::endl;
            Monde::afficherAvionAllegro(m_trajets[it]->getActuel(),m_trajets[it]->getPlanDeVol()[0],it,monde);
            for(auto ite : m_nuages)
            {
                Monde::csqNuage(ite, it);
            }
        }
    }




    blit(monde,screen,0,0,0,0,monde->w,monde->h);
    rest(1000);
}

void Monde::afficherNuageAllegro(BITMAP* monde, Nuage* _nuage)
{
    ///creation du nuage
    rectfill(monde, _nuage->getNX().first, _nuage->getNY().first, _nuage->getNX().second, _nuage->getNY().second, makecol( _nuage->getCouleurN(), _nuage->getCouleurN(),  _nuage->getCouleurN()));

    ///déplacement du nuage
    _nuage->setNX(std::make_pair(_nuage->getNX().first + _nuage->getVitesseN().first, _nuage->getNX().second + _nuage->getVitesseN().first));///en x
    _nuage->setNY(std::make_pair(_nuage->getNY().first + _nuage->getVitesseN().second, _nuage->getNY().second + _nuage->getVitesseN().second));///en y
}

void Monde::afficherNuageAllegroTest(BITMAP* monde, Nuage* _nuage, Aeroport* _depart, Aeroport* _arrivee, int coulNuage)///test pour fonctionnalité météo
{
    ///creation des nuages fixes sur la trajectoire Paris - New York pour tester la consommation
    int xDep = _depart->getCoordonnees().first;
    int yDep = _depart->getCoordonnees().second;

    int xArr = _arrivee->getCoordonnees().first;
    int yArr = _arrivee->getCoordonnees().second;

    int xNuage = (xDep + xArr) /2;
    int yNuage = (yDep + yArr) /2;

    _nuage->setNY(std::make_pair(yNuage-10, yNuage+10));
    _nuage->setNX(std::make_pair(xNuage-10, xNuage+10));
    _nuage->setCouleurN(coulNuage);

    rectfill(monde, _nuage->getNX().first, _nuage->getNY().first, _nuage->getNX().second, _nuage->getNY().second, makecol(_nuage->getCouleurN(), _nuage->getCouleurN(), _nuage->getCouleurN())); ///nuage clair : consommation +1

    /*rectfill(monde, 410, 129, 440, 157, makecol(140, 140, 140)); ///nuage clair : consommation +2
    _nuage->setNX(std::make_pair(410, 440));
    _nuage->setCouleurN(140);*/

    /*rectfill(monde, 460, 129, 490, 157, makecol(90, 90, 90)); ///nuage foncé : consommation +3
    _nuage->setNX(std::make_pair(460, 490));
    _nuage->setCouleurN(90);*/

}

///------------------csqNuage avec differentes csq en fonction de la couleur du nuage------------------

void Monde::csqNuage(Nuage* _nuage, Avion* _avion)
{

    if( _avion->getCoordonnees().first>_nuage->getNX().first && _avion->getCoordonnees().first<_nuage->getNX().second && _avion->getCoordonnees().second>_nuage->getNY().first && _avion->getCoordonnees().second<_nuage->getNY().second)
    {
        if(_avion->getConsomation()-_avion->getConsomationParam()==0)
        {
            if(_nuage->getCouleurN() > 150)///nuage clair
            {
                _avion->setConsommation(_avion->getConsomation()+1);
            }
            else if(_nuage->getCouleurN()>100 && _nuage->getCouleurN() < 150)/// nuage moyen
            {
                _avion->setConsommation(_avion->getConsomation()+2);
            }
            else if(_nuage->getCouleurN() < 100)///nuage sombre
            {
                _avion->setConsommation(_avion->getConsomation()+3);
            }
        }
    }

    if(_avion->getCoordonnees().first < _nuage->getNX().first || _avion->getCoordonnees().first > _nuage->getNX().second || _avion->getCoordonnees().second < _nuage->getNY().first || _avion->getCoordonnees().second > _nuage->getNY().second)
    {
        if(_avion->getConsomation()-_avion->getConsomationParam()!=0)
        {
            _avion->setConsommation(_avion->getConsomationParam());
        }
    }
}




void Monde::afficherAvionAllegro(Aeroport* _depart,Aeroport* _arrivee,Avion* _avion,BITMAP* monde)
{
    float degreRot=0.0;
    int poid=0;
    for(auto elem : _depart->getSuccesseurs())
    {
        if(elem.second ==_arrivee)
            poid=elem.first;
    }

    if(_arrivee->getCoordonnees().first>_depart->getCoordonnees().first && _arrivee->getCoordonnees().second>_depart->getCoordonnees().second)
    {
        if(_arrivee->getCoordonnees().first-_depart->getCoordonnees().first>_arrivee->getCoordonnees().second-_depart->getCoordonnees().second)
        {
            degreRot=64;
        }
        else
        {
            degreRot=128;
        }
    }
    else if(_arrivee->getCoordonnees().first>_depart->getCoordonnees().first && _arrivee->getCoordonnees().second<_depart->getCoordonnees().second)
    {
        if(_arrivee->getCoordonnees().first-_depart->getCoordonnees().first>_depart->getCoordonnees().second-_arrivee->getCoordonnees().second)
        {
            degreRot=64;
        }
    }
    else if(_arrivee->getCoordonnees().first<_depart->getCoordonnees().first && _arrivee->getCoordonnees().second>_depart->getCoordonnees().second)
    {
        if(_depart->getCoordonnees().first-_arrivee->getCoordonnees().first>_arrivee->getCoordonnees().second-_depart->getCoordonnees().second)
        {
            degreRot=192;
        }
        else
        {
            degreRot=128;
        }
    }
    else if(_arrivee->getCoordonnees().first<_depart->getCoordonnees().first && _arrivee->getCoordonnees().second<_depart->getCoordonnees().second)
    {
        if(_depart->getCoordonnees().first-_arrivee->getCoordonnees().first>_depart->getCoordonnees().second-_arrivee->getCoordonnees().second)
        {
            degreRot=192;
        }
    }


///-------------------------------------AFFICHAGE INFOS AEROPORTS------------------------------------------------------------------------------
    if(mouse_x>545 && mouse_x<565 && mouse_y>110 && mouse_y<129)///PARIS
    {
        rectfill(monde,10, 430, 370, 535, makecol(255,100,0));
        textprintf_ex(monde,font,15,440, makecol(0,0,0), makecol(255,100,0),"INFORMATIONS SUR L'AEROPORT DE PARIS :");
        textprintf_ex(monde,font,20,460, makecol(0,0,0), makecol(255,100,0),"Nombre de pistes : %d",m_aeroports[0]->getNbPistes());
        textprintf_ex(monde,font,20,470, makecol(0,0,0), makecol(255,100,0),"Nombre de places au sol : %d",m_aeroports[0]->getNbPlacesSol());
        textprintf_ex(monde,font,20,480, makecol(0,0,0), makecol(255,100,0),"Duree d'attente au sol : %d",m_aeroports[0]->getDAS());
        textprintf_ex(monde,font,20,490, makecol(0,0,0), makecol(255,100,0),"Temps d'acces au pistes : %d",m_aeroports[0]->getTAP());
        textprintf_ex(monde,font,20,500, makecol(0,0,0), makecol(255,100,0),"Duree atterissage - decollage : %d",m_aeroports[0]->getTDA());
        textprintf_ex(monde,font,20,510, makecol(0,0,0), makecol(255,100,0),"Delai d'anti-collision : %d",m_aeroports[0]->getDAC());
        textprintf_ex(monde,font,20,520, makecol(0,0,0), makecol(255,100,0),"Boucle d'attente : %d",m_aeroports[0]->getDBA());
    }
    if(mouse_x>310 && mouse_x<330 && mouse_y>140 && mouse_y<157)///NYC
    {
        rectfill(monde,10, 430, 370, 535, makecol(255,100,0));
        textprintf_ex(monde,font,15,440, makecol(0,0,0), makecol(255,100,0),"INFORMATIONS SUR L'AEROPORT DE NEW YORK :");
        textprintf_ex(monde,font,20,460, makecol(0,0,0), makecol(255,100,0),"Nombre de pistes : %d",m_aeroports[1]->getNbPistes());
        textprintf_ex(monde,font,20,470, makecol(0,0,0), makecol(255,100,0),"Nombre de places au sol : %d",m_aeroports[1]->getNbPlacesSol());
        textprintf_ex(monde,font,20,480, makecol(0,0,0), makecol(255,100,0),"Duree d'attente au sol : %d",m_aeroports[1]->getDAS());
        textprintf_ex(monde,font,20,490, makecol(0,0,0), makecol(255,100,0),"Temps d'acces au pistes : %d",m_aeroports[1]->getTAP());
        textprintf_ex(monde,font,20,500, makecol(0,0,0), makecol(255,100,0),"Duree atterissage - decollage : %d",m_aeroports[1]->getTDA());
        textprintf_ex(monde,font,20,510, makecol(0,0,0), makecol(255,100,0),"Delai d'anti-collision : %d",m_aeroports[1]->getDAC());
        textprintf_ex(monde,font,20,520, makecol(0,0,0), makecol(255,100,0),"Boucle d'attente : %d",m_aeroports[1]->getDBA());
    }
    if(mouse_x>970 && mouse_x<990 && mouse_y>146 && mouse_y<166)///TOKYO
    {
        rectfill(monde,10, 430, 370, 535, makecol(255,100,0));
        textprintf_ex(monde,font,15,440, makecol(0,0,0), makecol(255,100,0),"INFORMATIONS SUR L'AEROPORT DE TOKYO :");
        textprintf_ex(monde,font,20,460, makecol(0,0,0), makecol(255,100,0),"Nombre de pistes : %d",m_aeroports[2]->getNbPistes());
        textprintf_ex(monde,font,20,470, makecol(0,0,0), makecol(255,100,0),"Nombre de places au sol : %d",m_aeroports[2]->getNbPlacesSol());
        textprintf_ex(monde,font,20,480, makecol(0,0,0), makecol(255,100,0),"Duree d'attente au sol : %d",m_aeroports[2]->getDAS());
        textprintf_ex(monde,font,20,490, makecol(0,0,0), makecol(255,100,0),"Temps d'acces au pistes : %d",m_aeroports[2]->getTAP());
        textprintf_ex(monde,font,20,500, makecol(0,0,0), makecol(255,100,0),"Duree atterissage - decollage : %d",m_aeroports[2]->getTDA());
        textprintf_ex(monde,font,20,510, makecol(0,0,0), makecol(255,100,0),"Delai d'anti-collision : %d",m_aeroports[2]->getDAC());
        textprintf_ex(monde,font,20,520, makecol(0,0,0), makecol(255,100,0),"Boucle d'attente : %d",m_aeroports[2]->getDBA());
    }
    if(mouse_x>706 && mouse_x<726 && mouse_y>183 && mouse_y<203)///DUBAI
    {
        rectfill(monde,10, 430, 370, 535, makecol(255,100,0));
        textprintf_ex(monde,font,15,440, makecol(0,0,0), makecol(255,100,0),"INFORMATIONS SUR L'AEROPORT DE DUBAI :");
        textprintf_ex(monde,font,20,460, makecol(0,0,0), makecol(255,100,0),"Nombre de pistes : %d",m_aeroports[3]->getNbPistes());
        textprintf_ex(monde,font,20,470, makecol(0,0,0), makecol(255,100,0),"Nombre de places au sol : %d",m_aeroports[3]->getNbPlacesSol());
        textprintf_ex(monde,font,20,480, makecol(0,0,0), makecol(255,100,0),"Duree d'attente au sol : %d",m_aeroports[3]->getDAS());
        textprintf_ex(monde,font,20,490, makecol(0,0,0), makecol(255,100,0),"Temps d'acces au pistes : %d",m_aeroports[3]->getTAP());
        textprintf_ex(monde,font,20,500, makecol(0,0,0), makecol(255,100,0),"Duree atterissage - decollage : %d",m_aeroports[3]->getTDA());
        textprintf_ex(monde,font,20,510, makecol(0,0,0), makecol(255,100,0),"Delai d'anti-collision : %d",m_aeroports[3]->getDAC());
        textprintf_ex(monde,font,20,520, makecol(0,0,0), makecol(255,100,0),"Boucle d'attente : %d",m_aeroports[3]->getDBA());
    }
    if(mouse_x>172 && mouse_x<192 && mouse_y>147 && mouse_y<167)///LAX
    {
        rectfill(monde,10, 430, 370, 535, makecol(255,100,0));
        textprintf_ex(monde,font,15,440, makecol(0,0,0), makecol(255,100,0),"INFORMATIONS SUR L'AEROPORT DE LOS ANGELES :");
        textprintf_ex(monde,font,20,460, makecol(0,0,0), makecol(255,100,0),"Nombre de pistes : %d",m_aeroports[4]->getNbPistes());
        textprintf_ex(monde,font,20,470, makecol(0,0,0), makecol(255,100,0),"Nombre de places au sol : %d",m_aeroports[4]->getNbPlacesSol());
        textprintf_ex(monde,font,20,480, makecol(0,0,0), makecol(255,100,0),"Duree d'attente au sol : %d",m_aeroports[4]->getDAS());
        textprintf_ex(monde,font,20,490, makecol(0,0,0), makecol(255,100,0),"Temps d'acces au pistes : %d",m_aeroports[4]->getTAP());
        textprintf_ex(monde,font,20,500, makecol(0,0,0), makecol(255,100,0),"Duree atterissage - decollage : %d",m_aeroports[4]->getTDA());
        textprintf_ex(monde,font,20,510, makecol(0,0,0), makecol(255,100,0),"Delai d'anti-collision : %d",m_aeroports[4]->getDAC());
        textprintf_ex(monde,font,20,520, makecol(0,0,0), makecol(255,100,0),"Boucle d'attente : %d",m_aeroports[4]->getDBA());
    }
    if(mouse_x>536 && mouse_x<556 && mouse_y>97 && mouse_y<117)///LONDON
    {
        rectfill(monde,10, 430, 370, 535, makecol(255,100,0));
        textprintf_ex(monde,font,15,440, makecol(0,0,0), makecol(255,100,0),"INFORMATIONS SUR L'AEROPORT DE LONDRES :");
        textprintf_ex(monde,font,20,460, makecol(0,0,0), makecol(255,100,0),"Nombre de pistes : %d",m_aeroports[5]->getNbPistes());
        textprintf_ex(monde,font,20,470, makecol(0,0,0), makecol(255,100,0),"Nombre de places au sol : %d",m_aeroports[5]->getNbPlacesSol());
        textprintf_ex(monde,font,20,480, makecol(0,0,0), makecol(255,100,0),"Duree d'attente au sol : %d",m_aeroports[5]->getDAS());
        textprintf_ex(monde,font,20,490, makecol(0,0,0), makecol(255,100,0),"Temps d'acces au pistes : %d",m_aeroports[5]->getTAP());
        textprintf_ex(monde,font,20,500, makecol(0,0,0), makecol(255,100,0),"Duree atterissage - decollage : %d",m_aeroports[5]->getTDA());
        textprintf_ex(monde,font,20,510, makecol(0,0,0), makecol(255,100,0),"Delai d'anti-collision : %d",m_aeroports[5]->getDAC());
        textprintf_ex(monde,font,20,520, makecol(0,0,0), makecol(255,100,0),"Boucle d'attente : %d",m_aeroports[5]->getDBA());
    }
    if(mouse_x>613 && mouse_x<633 && mouse_y>95 && mouse_y<115)///MOSCOU
    {
        rectfill(monde,10, 430, 370, 535, makecol(255,100,0));
        textprintf_ex(monde,font,15,440, makecol(0,0,0), makecol(255,100,0),"INFORMATIONS SUR L'AEROPORT DE MOSCOU :");
        textprintf_ex(monde,font,20,460, makecol(0,0,0), makecol(255,100,0),"Nombre de pistes : %d",m_aeroports[6]->getNbPistes());
        textprintf_ex(monde,font,20,470, makecol(0,0,0), makecol(255,100,0),"Nombre de places au sol : %d",m_aeroports[6]->getNbPlacesSol());
        textprintf_ex(monde,font,20,480, makecol(0,0,0), makecol(255,100,0),"Duree d'attente au sol : %d",m_aeroports[6]->getDAS());
        textprintf_ex(monde,font,20,490, makecol(0,0,0), makecol(255,100,0),"Temps d'acces au pistes : %d",m_aeroports[6]->getTAP());
        textprintf_ex(monde,font,20,500, makecol(0,0,0), makecol(255,100,0),"Duree atterissage - decollage : %d",m_aeroports[6]->getTDA());
        textprintf_ex(monde,font,20,510, makecol(0,0,0), makecol(255,100,0),"Delai d'anti-collision : %d",m_aeroports[6]->getDAC());
        textprintf_ex(monde,font,20,520, makecol(0,0,0), makecol(255,100,0),"Boucle d'attente : %d",m_aeroports[6]->getDBA());
    }
    if(mouse_x>548 && mouse_x<568 && mouse_y>143 && mouse_y<163)///ALGERS
    {
        rectfill(monde,10, 430, 370, 535, makecol(255,100,0));
        textprintf_ex(monde,font,15,440, makecol(0,0,0), makecol(255,100,0),"INFORMATIONS SUR L'AEROPORT DE ALGERS :");
        textprintf_ex(monde,font,20,460, makecol(0,0,0), makecol(255,100,0),"Nombre de pistes : %d",m_aeroports[7]->getNbPistes());
        textprintf_ex(monde,font,20,470, makecol(0,0,0), makecol(255,100,0),"Nombre de places au sol : %d",m_aeroports[7]->getNbPlacesSol());
        textprintf_ex(monde,font,20,480, makecol(0,0,0), makecol(255,100,0),"Duree d'attente au sol : %d",m_aeroports[7]->getDAS());
        textprintf_ex(monde,font,20,490, makecol(0,0,0), makecol(255,100,0),"Temps d'acces au pistes : %d",m_aeroports[7]->getTAP());
        textprintf_ex(monde,font,20,500, makecol(0,0,0), makecol(255,100,0),"Duree atterissage - decollage : %d",m_aeroports[7]->getTDA());
        textprintf_ex(monde,font,20,510, makecol(0,0,0), makecol(255,100,0),"Delai d'anti-collision : %d",m_aeroports[7]->getDAC());
        textprintf_ex(monde,font,20,520, makecol(0,0,0), makecol(255,100,0),"Boucle d'attente : %d",m_aeroports[7]->getDBA());
    }
    if(mouse_x>527 && mouse_x<547 && mouse_y>234 && mouse_y<254)///YAMASSOUKRO
    {
        rectfill(monde,10, 430, 370, 535, makecol(255,100,0));
        textprintf_ex(monde,font,15,440, makecol(0,0,0), makecol(255,100,0),"INFORMATIONS SUR L'AEROPORT DE YAMASSOUKRO :");
        textprintf_ex(monde,font,20,460, makecol(0,0,0), makecol(255,100,0),"Nombre de pistes : %d",m_aeroports[8]->getNbPistes());
        textprintf_ex(monde,font,20,470, makecol(0,0,0), makecol(255,100,0),"Nombre de places au sol : %d",m_aeroports[8]->getNbPlacesSol());
        textprintf_ex(monde,font,20,480, makecol(0,0,0), makecol(255,100,0),"Duree d'attente au sol : %d",m_aeroports[8]->getDAS());
        textprintf_ex(monde,font,20,490, makecol(0,0,0), makecol(255,100,0),"Temps d'acces au pistes : %d",m_aeroports[8]->getTAP());
        textprintf_ex(monde,font,20,500, makecol(0,0,0), makecol(255,100,0),"Duree atterissage - decollage : %d",m_aeroports[8]->getTDA());
        textprintf_ex(monde,font,20,510, makecol(0,0,0), makecol(255,100,0),"Delai d'anti-collision : %d",m_aeroports[8]->getDAC());
        textprintf_ex(monde,font,20,520, makecol(0,0,0), makecol(255,100,0),"Boucle d'attente : %d",m_aeroports[8]->getDBA());
    }
    if(mouse_x>404 && mouse_x<424 && mouse_y>325 && mouse_y<345)///SAO PAULO
    {
        rectfill(monde,10, 430, 370, 535, makecol(255,100,0));
        textprintf_ex(monde,font,15,440, makecol(0,0,0), makecol(255,100,0),"INFORMATIONS SUR L'AEROPORT DE SAO PAULO :");
        textprintf_ex(monde,font,20,460, makecol(0,0,0), makecol(255,100,0),"Nombre de pistes : %d",m_aeroports[9]->getNbPistes());
        textprintf_ex(monde,font,20,470, makecol(0,0,0), makecol(255,100,0),"Nombre de places au sol : %d",m_aeroports[9]->getNbPlacesSol());
        textprintf_ex(monde,font,20,480, makecol(0,0,0), makecol(255,100,0),"Duree d'attente au sol : %d",m_aeroports[9]->getDAS());
        textprintf_ex(monde,font,20,490, makecol(0,0,0), makecol(255,100,0),"Temps d'acces au pistes : %d",m_aeroports[9]->getTAP());
        textprintf_ex(monde,font,20,500, makecol(0,0,0), makecol(255,100,0),"Duree atterissage - decollage : %d",m_aeroports[9]->getTDA());
        textprintf_ex(monde,font,20,510, makecol(0,0,0), makecol(255,100,0),"Delai d'anti-collision : %d",m_aeroports[9]->getDAC());
        textprintf_ex(monde,font,20,520, makecol(0,0,0), makecol(255,100,0),"Boucle d'attente : %d",m_aeroports[9]->getDBA());
    }
    if(mouse_x>324 && mouse_x<342 && mouse_y>362 && mouse_y<382)///SANTIAGO
    {
        rectfill(monde,10, 430, 370, 535, makecol(255,100,0));
        textprintf_ex(monde,font,15,440, makecol(0,0,0), makecol(255,100,0),"INFORMATIONS SUR L'AEROPORT DE SANTIAGO :");
        textprintf_ex(monde,font,20,460, makecol(0,0,0), makecol(255,100,0),"Nombre de pistes : %d",m_aeroports[10]->getNbPistes());
        textprintf_ex(monde,font,20,470, makecol(0,0,0), makecol(255,100,0),"Nombre de places au sol : %d",m_aeroports[10]->getNbPlacesSol());
        textprintf_ex(monde,font,20,480, makecol(0,0,0), makecol(255,100,0),"Duree d'attente au sol : %d",m_aeroports[10]->getDAS());
        textprintf_ex(monde,font,20,490, makecol(0,0,0), makecol(255,100,0),"Temps d'acces au pistes : %d",m_aeroports[10]->getTAP());
        textprintf_ex(monde,font,20,500, makecol(0,0,0), makecol(255,100,0),"Duree atterissage - decollage : %d",m_aeroports[10]->getTDA());
        textprintf_ex(monde,font,20,510, makecol(0,0,0), makecol(255,100,0),"Delai d'anti-collision : %d",m_aeroports[10]->getDAC());
        textprintf_ex(monde,font,20,520, makecol(0,0,0), makecol(255,100,0),"Boucle d'attente : %d",m_aeroports[10]->getDBA());
    }
    if(mouse_x>622 && mouse_x<642 && mouse_y>348 && mouse_y<368)///JOHANNESBOURG
    {
        rectfill(monde,10, 430, 370, 535, makecol(255,100,0));
        textprintf_ex(monde,font,15,440, makecol(0,0,0), makecol(255,100,0),"INFORMATIONS SUR L'AEROPORT DE JOHANNESBOURG");
        textprintf_ex(monde,font,20,460, makecol(0,0,0), makecol(255,100,0),"Nombre de pistes : %d",m_aeroports[11]->getNbPistes());
        textprintf_ex(monde,font,20,470, makecol(0,0,0), makecol(255,100,0),"Nombre de places au sol : %d",m_aeroports[11]->getNbPlacesSol());
        textprintf_ex(monde,font,20,480, makecol(0,0,0), makecol(255,100,0),"Duree d'attente au sol : %d",m_aeroports[11]->getDAS());
        textprintf_ex(monde,font,20,490, makecol(0,0,0), makecol(255,100,0),"Temps d'acces au pistes : %d",m_aeroports[11]->getTAP());
        textprintf_ex(monde,font,20,500, makecol(0,0,0), makecol(255,100,0),"Duree atterissage - decollage : %d",m_aeroports[11]->getTDA());
        textprintf_ex(monde,font,20,510, makecol(0,0,0), makecol(255,100,0),"Delai d'anti-collision : %d",m_aeroports[11]->getDAC());
        textprintf_ex(monde,font,20,520, makecol(0,0,0), makecol(255,100,0),"Boucle d'attente : %d",m_aeroports[11]->getDBA());
    }
    if(mouse_x>1000 && mouse_x<1020 && mouse_y>360 && mouse_y<380)///SYDNEY
    {
        rectfill(monde,10, 430, 370, 535, makecol(255,100,0));
        textprintf_ex(monde,font,15,440, makecol(0,0,0), makecol(255,100,0),"INFORMATIONS SUR L'AEROPORT DE SYDNEY :");
        textprintf_ex(monde,font,20,460, makecol(0,0,0), makecol(255,100,0),"Nombre de pistes : %d",m_aeroports[12]->getNbPistes());
        textprintf_ex(monde,font,20,470, makecol(0,0,0), makecol(255,100,0),"Nombre de places au sol : %d",m_aeroports[12]->getNbPlacesSol());
        textprintf_ex(monde,font,20,480, makecol(0,0,0), makecol(255,100,0),"Duree d'attente au sol : %d",m_aeroports[12]->getDAS());
        textprintf_ex(monde,font,20,490, makecol(0,0,0), makecol(255,100,0),"Temps d'acces au pistes : %d",m_aeroports[12]->getTAP());
        textprintf_ex(monde,font,20,500, makecol(0,0,0), makecol(255,100,0),"Duree atterissage - decollage : %d",m_aeroports[12]->getTDA());
        textprintf_ex(monde,font,20,510, makecol(0,0,0), makecol(255,100,0),"Delai d'anti-collision : %d",m_aeroports[12]->getDAC());
        textprintf_ex(monde,font,20,520, makecol(0,0,0), makecol(255,100,0),"Boucle d'attente : %d",m_aeroports[12]->getDBA());
    }
    if(mouse_x>764 && mouse_x<784 && mouse_y>196 && mouse_y<216)///MUMBAI
    {
        rectfill(monde,10, 430, 370, 535, makecol(255,100,0));
        textprintf_ex(monde,font,15,440, makecol(0,0,0), makecol(255,100,0),"INFORMATIONS SUR L'AEROPORT DE MUMBAI :");
        textprintf_ex(monde,font,20,460, makecol(0,0,0), makecol(255,100,0),"Nombre de pistes : %d",m_aeroports[13]->getNbPistes());
        textprintf_ex(monde,font,20,470, makecol(0,0,0), makecol(255,100,0),"Nombre de places au sol : %d",m_aeroports[13]->getNbPlacesSol());
        textprintf_ex(monde,font,20,480, makecol(0,0,0), makecol(255,100,0),"Duree d'attente au sol : %d",m_aeroports[13]->getDAS());
        textprintf_ex(monde,font,20,490, makecol(0,0,0), makecol(255,100,0),"Temps d'acces au pistes : %d",m_aeroports[13]->getTAP());
        textprintf_ex(monde,font,20,500, makecol(0,0,0), makecol(255,100,0),"Duree atterissage - decollage : %d",m_aeroports[13]->getTDA());
        textprintf_ex(monde,font,20,510, makecol(0,0,0), makecol(255,100,0),"Delai d'anti-collision : %d",m_aeroports[13]->getDAC());
        textprintf_ex(monde,font,20,520, makecol(0,0,0), makecol(255,100,0),"Boucle d'attente : %d",m_aeroports[13]->getDBA());
    }
    if(mouse_x>1075 && mouse_x<1095 && mouse_y>182 && mouse_y<402)///WELLINGTON
    {
        rectfill(monde,10, 430, 370, 535, makecol(255,100,0));
        textprintf_ex(monde,font,15,440, makecol(0,0,0), makecol(255,100,0),"INFORMATIONS SUR L'AEROPORT DE WELLINGTON :");
        textprintf_ex(monde,font,20,460, makecol(0,0,0), makecol(255,100,0),"Nombre de pistes : %d",m_aeroports[14]->getNbPistes());
        textprintf_ex(monde,font,20,470, makecol(0,0,0), makecol(255,100,0),"Nombre de places au sol : %d",m_aeroports[14]->getNbPlacesSol());
        textprintf_ex(monde,font,20,480, makecol(0,0,0), makecol(255,100,0),"Duree d'attente au sol : %d",m_aeroports[14]->getDAS());
        textprintf_ex(monde,font,20,490, makecol(0,0,0), makecol(255,100,0),"Temps d'acces au pistes : %d",m_aeroports[14]->getTAP());
        textprintf_ex(monde,font,20,500, makecol(0,0,0), makecol(255,100,0),"Duree atterissage - decollage : %d",m_aeroports[14]->getTDA());
        textprintf_ex(monde,font,20,510, makecol(0,0,0), makecol(255,100,0),"Delai d'anti-collision : %d",m_aeroports[14]->getDAC());
        textprintf_ex(monde,font,20,520, makecol(0,0,0), makecol(255,100,0),"Boucle d'attente : %d",m_aeroports[14]->getDBA());
    }
    if(mouse_x>896 && mouse_x<916 && mouse_y>185 && mouse_y<205)///HONG KONG
    {
        rectfill(monde,10, 430, 370, 535, makecol(255,100,0));
        textprintf_ex(monde,font,15,440, makecol(0,0,0), makecol(255,100,0),"INFORMATIONS SUR L'AEROPORT DE HONG KONG :");
        textprintf_ex(monde,font,20,460, makecol(0,0,0), makecol(255,100,0),"Nombre de pistes : %d",m_aeroports[15]->getNbPistes());
        textprintf_ex(monde,font,20,470, makecol(0,0,0), makecol(255,100,0),"Nombre de places au sol : %d",m_aeroports[15]->getNbPlacesSol());
        textprintf_ex(monde,font,20,480, makecol(0,0,0), makecol(255,100,0),"Duree d'attente au sol : %d",m_aeroports[15]->getDAS());
        textprintf_ex(monde,font,20,490, makecol(0,0,0), makecol(255,100,0),"Temps d'acces au pistes : %d",m_aeroports[15]->getTAP());
        textprintf_ex(monde,font,20,500, makecol(0,0,0), makecol(255,100,0),"Duree atterissage - decollage : %d",m_aeroports[15]->getTDA());
        textprintf_ex(monde,font,20,510, makecol(0,0,0), makecol(255,100,0),"Delai d'anti-collision : %d",m_aeroports[15]->getDAC());
        textprintf_ex(monde,font,20,520, makecol(0,0,0), makecol(255,100,0),"Boucle d'attente : %d",m_aeroports[15]->getDBA());
    }

///-------------------------------------AFFICHAGE INFOS AVIONS------------------------------------------------------------------------------

    if(mouse_x>_avion->getCoordonnees().first-50 && mouse_x<_avion->getCoordonnees().first+50 && mouse_y>_avion->getCoordonnees().second-10 && mouse_y<_avion->getCoordonnees().second+10)///HONG KONG
    {
        rectfill(monde,10, 430, 370, 535, makecol(0,100,100));
        textprintf_ex(monde,font,15,440, makecol(0,0,0), makecol(0,100,100),"INFORMATIONS SUR L'AVION ");
        textprintf_ex(monde,font,220,440, makecol(0,0,0), makecol(0,100,100),"%s",_avion->getNom().c_str());
        textprintf_ex(monde,font,20,460, makecol(0,0,0), makecol(0,100,100),"Type : %s",_avion->getType().c_str());
        textprintf_ex(monde,font,20,470, makecol(0,0,0), makecol(0,100,100),"Consommation : %d",_avion->getConsomation());
        textprintf_ex(monde,font,20,480, makecol(0,0,0), makecol(0,100,100),"Capacite de carburant : %d",_avion->getCapacite());
        textprintf_ex(monde,font,20,490, makecol(0,0,0), makecol(0,100,100),"Carburant restant: %d",_avion->getCarburant());
        textprintf_ex(monde,font,20,500, makecol(0,0,0), makecol(0,100,100),"Coordonnees : %d",_avion->getCoordonnees().first);
        textprintf_ex(monde,font,160,500, makecol(0,0,0), makecol(0,100,100),"x | %d",_avion->getCoordonnees().second);
        textprintf_ex(monde,font,220,500, makecol(0,0,0), makecol(0,100,100),"y");
    }

    rotate_sprite(monde,_avion->getImage(),_avion->getCoordonnees().first-10,_avion->getCoordonnees().second-10,itofix(degreRot));
    deplacementAvion(_depart,_arrivee,_avion);
    show_mouse(monde);
    //blit(monde,screen,0,0,0,0,SCREEN_W,SCREEN_H);

    if(_avion->getTempsTraitement()==poid)
    {
        std::cout<<"                                                        "<<_avion->getNom()<<" temps de traitement : "<<_avion->getTempsTraitement()<<" poid : "<<poid<<std::endl;
        _avion->setCoordonnees(_arrivee->getCoordonnees());
    }
    std::cout<<"reservoir avion : " <<_avion->getCarburant()<<std::endl;
}

void Monde::initNuages()
{
    std::cout<<"\ninitNuages"<<std::endl;
    int nbNuagesAleatoire = rand()%(20-15)+14;

    for (int i=0; i<nbNuagesAleatoire; i++)///a changer pour rajouter tous les nuages 1 devient nbNuagesAleatoire
    {
        Nuage* nvNuage = new Nuage;
        m_nuages.push_back(nvNuage);
    }
    /*for (auto it : m_nuages)///test
    {
        it->afficherNuage();
    }*/
}

void Monde::initNuagesTest(int nbNuages)
{

    for (int i=0; i<nbNuages; i++)///a changer pour rajouter tous les nuages 1 devient nbNuagesAleatoire
    {
        Nuage* nvNuage = new Nuage;
        m_nuages.push_back(nvNuage);
    }
    /*for (auto it : m_nuages)///test
    {
        it->afficherNuage();
    }*/
}
