#include <iostream>
#include "Classes/Avion.h"
#include "Classes/Aeroport.h"
#include "Classes/Monde.h"


using namespace std;

int main()
{
    Monde m{"Monde.txt"};
    m.afficherMonde();
    return 0;
}
