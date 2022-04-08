#include <iostream>
#include "Classes/Avion.h"
#include "Classes/Aeroport.h"
#include "Classes/Monde.h"
#include <allegro.h>

using namespace std;

int main()
{
    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,1100,550,0,0)!=0)
    {
        allegro_message("prb gfx mode");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    std::vector<Aeroport*> aeroports;
    std::vector<Avion*> avion;

    Monde m("Fichiers_txt/Monde.txt");

    m.initialisationAeroport();
    aeroports=m.getAeroports();
    avion=m.getAvion();
    m.afficherMondeAllegro(aeroports[4],aeroports[10],avion[0]);
    m.afficherMonde();


    m.Dijkstra(m.getAeroports()[m.trouveIdentification("NYC")], m.getAeroports()[m.trouveIdentification("DUB")]);

    return 0;
}END_OF_MAIN();
