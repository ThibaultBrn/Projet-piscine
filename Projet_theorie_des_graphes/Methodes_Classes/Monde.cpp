#include "../Classes/Monde.h"
#include <queue>
#include <vector>
#include <fstream>


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
        m_avion.push_back( new Avion(nomAvion,typeAvion, consommation, capacite_carburant, std::make_pair(0,0)));
    }

    ///--------------RECUPERATION DES IMAGES----------------------

    imageChargee=load_bitmap("images_allegro/carte.bmp",NULL);



    imageChargee=load_bitmap("images_allegro/carte.bmp",NULL);
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



    std::cout<<std::endl<<"Voici notre monde :" << std::endl;
    std::cout<<"Taille : "<<m_aeroports.size()<<std::endl;
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

    std::cout<<std::endl<<"\n\nVoici la carte de notre monde :\n" << std::endl;

    int x, y;
    bool afficheNom=false;
    for (y=0; y<21; y++){
        for (x=0; x<31; x++){
            afficheNom=false;
            for(auto s : m_aeroports){
                if(s->getCoordonnes().first==x && s->getCoordonnes().second==y)
                {
                    std::cout<<s->getNom();
                    afficheNom=true;
                }
            }
            if(afficheNom==false)
            {
                std::cout<<"|  ";
            }

            if(x==30)
            {
                std::cout<<std::endl;
                break;
            }
        }
    }



    while(!key[KEY_ESC])
    {
        blit(m_carte, screen, 0,0, (SCREEN_W-m_carte->w)/2, (SCREEN_H-m_carte->h)/2, m_carte->w, m_carte->h);

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

void Monde::Dijkstra(const Aeroport* Depart, const Aeroport* Arrivee)
{
    /// INITIALISATION DE L'ALGO ///
    const Aeroport* AeroportActuel = Depart;
    std::priority_queue<std::pair<int, const Aeroport*>, std::vector<std::pair<int, const Aeroport*>>, std::greater<std::pair<int, const Aeroport*>>> distanceSuccesseurs; ///PRIORITY QUEUE D'ARCS POUR FACILEMENT RETROUVER LE PROCHAIN SOMMET LE PLUS PROCHE DU SOMMET INTIAL.
    std::vector<bool> aeroportsMarques(m_aeroports.size(), false);
    std::vector<const Aeroport*> predecesseurs(m_aeroports.size(), NULL);
    std::vector<int> distancePlusCourtChemin(m_aeroports.size(), -1); ///-1 = PAS ENCORE TRAITE. SERAIT EGAL A INFINI SI ON SUIVAIT LE COURS.
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
    if(Depart == NULL || Arrivee == NULL)
    {
        std::cout << "Taille du plus court chemin : 0... Le sommet de depart et le sommet d'arrivee sont les memes." << std::endl;
    }
    else
    {
        AeroportActuel = Arrivee;
        std::cout << "Taille du plus court chemin : " << distancePlusCourtChemin[AeroportActuel->getIdentification()] << std::endl;
        while(AeroportActuel != Depart)
        {
            std::cout << AeroportActuel->getIdentification() << " <- ";
            AeroportActuel = predecesseurs[AeroportActuel->getIdentification()];
        }
        std::cout << Depart->getIdentification() << std::endl << std::endl;
    }
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
    int nbAvions=4;
    unsigned int compteur=0;
    std::vector<Avion*> lesAvions;
    std::vector<Aeroport*>airports;
    airports=m_aeroports;
    melangerAvion();
    for(unsigned int i=0;i<airports.size();i++)
    {
        m_aeroports.pop_back();
    }
    for(int i=0;i<nbAvions;i++)
    {
        if(airports[compteur]->getNbPlacesSol()!=0)
        {
            airports[compteur]->SetAvionSol(m_avion[i]);
            airports[compteur]->setNbPlacesSol((airports[compteur]->getNbPlacesSol())-1);
        }
        else
        {
            m_aeroports.push_back(airports[compteur]);
            airports.erase(airports.begin()+compteur);
        }
        compteur++;
        if(compteur==airports.size())
        {
            compteur=0;
        }
    }
    for(unsigned int i=0;i<airports.size();i++)
    {
        m_aeroports.push_back(airports[i]);
    }
    for(unsigned int i=0;i<m_aeroports.size();i++)
    {
        lesAvions=m_aeroports[i]->getAvionSol();
        for(unsigned int j=0;j<lesAvions.size();j++)
        {
            lesAvions[j]->setCoordonnees(m_aeroports[i]->getCoordonnes());
        }
    }
}

void Monde::deplacementAvion(Aeroport* _depart,Aeroport* _arrivee,Avion* _avion)
{
    float m=0.0;
    float p=0.0;
    std::pair<int,int>coordonneesAvion;
    coordonneesAvion=_avion->getCoordonnees();
    m=((_arrivee->getCoordonnes().second)-(_depart->getCoordonnes().second))/((_arrivee->getCoordonnes().first)-(_depart->getCoordonnes().second));
    p=_depart->getCoordonnes().second-m*_depart->getCoordonnes().first;
    if(_depart->getCoordonnes().first<=_arrivee->getCoordonnes().first)
    {
        coordonneesAvion.first=coordonneesAvion.first+10;
        coordonneesAvion.second=m*coordonneesAvion.first+p;
    }
    else
    {
        coordonneesAvion.first=coordonneesAvion.second-10;
        coordonneesAvion.second=m*coordonneesAvion.first+p;
    }
    _avion->setCarburant(_avion->getCarburant() - 10 * _avion->getConsomation());
}

void Monde::afficherMonde(Aeroport* _depart,Aeroport* _arrivee,Avion* _avion)
{
    BITMAP * page;
    page=create_bitmap(SCREEN_W,SCREEN_H);
    clear_bitmap(page);

    while (int i=0)
    {
        rectfill(page,_depart->getCoordonnes().first-1,_depart->getCoordonnes().second-1,_arrivee->getCoordonnes().first+1,_arrivee->getCoordonnes().second+1,makecol(255,0,0));
        rectfill(page,_avion->getCoordonnees().first-2,_avion->getCoordonnees().second-2,_avion->getCoordonnees().first+2,_avion->getCoordonnees().second+2,makecol(0,0,0));
        blit(page,screen,0,0,0,0,SCREEN_W,SCREEN_H);
        deplacementAvion(_depart,_arrivee,_avion);
        clear_bitmap(page);
    }

}
