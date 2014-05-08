/*
 *  Quentin Peter
 *
 */

#ifndef HASARD_H
#define HASARD_H
#include <cstdlib>
#include <cmath>
#include <ctime>


namespace hasard {
    void init_hasard(unsigned int graine=0);//Initialise le hasard
    double uniforme(double a, double b);//Tirage uniforme
    double gaussienne(const double moyenne, const double ecart_type);//Tirage gaussien

}
#endif

