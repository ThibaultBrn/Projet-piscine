#include <iostream>
#include "Classes/Avion.h"
#include "Classes/Aeroport.h"
#include "Classes/Monde.h"
#include <allegro.h>

using namespace std;

int main()
{
    BITMAP *imageChargee;
    allegro_init();
    install_keyboard();

    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,1100,600,0,0)!=0)
    {
        allegro_message("prb gfx mode");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    std::vector<Aeroport*> aeroports;
    Monde m("Fichiers_txt/Monde.txt");

    m.initialisationAeroport();

    m.afficherMonde();

    m.Dijkstra(m.getAeroports()[m.trouveIdentification("NYC")], m.getAeroports()[m.trouveIdentification("DUB")]);

    return 0;
}END_OF_MAIN();
