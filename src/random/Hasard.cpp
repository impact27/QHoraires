//
//  Head.cpp
//
//  Quentin Peter, Loïc Reymond
//  Version du 29 mai 2011
//
//  Contenu: Choses utiles 
//

//On Initalise le temps
#define TEMPS_CC//On met en place le temps
#include "Hasard.h"

void hasard::init_hasard(unsigned int graine) {
    if (graine == 0) {//On met le hasard en place
        srand((unsigned int)time(0));
    } else {
        srand(graine);
    }
}

double hasard::uniforme(double a, double b) {
    return a + (rand() / double(RAND_MAX)) * (b - a);//On tire un nombre
}

double hasard::gaussienne(const double moyenne, const double ecart_type){
    //Loi de probabilité gaussienne
    double v1;
    double v2;
    double s ;
    
    do {
        v1 = uniforme(-1.0, 1.0);
        v2 = uniforme(-1.0, 1.0);
        s = v1*v1 + v2*v2;
    } while ((s >= 1.0) or (s == 0.0));
    
    double x(sqrt(-2.0 * log(s) / s));
    if (uniforme(0.0, 1.0) > 0.5)
        x *= v1;
    else
        x *= v2;
    
    return moyenne + ecart_type * x;
}