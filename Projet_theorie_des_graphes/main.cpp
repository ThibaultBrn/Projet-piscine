#include <iostream>
#include <time.h>
#include "Classes/Avion.h"
#include "Classes/Aeroport.h"
#include "Classes/Monde.h"
#include "Classes/Menu.h"
#include <allegro.h>

using namespace std;

int main()
{

    allegro_init();
    install_keyboard();
    install_mouse();
    char test= '0';
    Menu a;

    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,1100,550,0,0)!=0)
    {
        allegro_message("prb gfx mode");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    while(test!=6)
    {
       test=a.gestionMenu();
    }


    return 0;
}END_OF_MAIN();
