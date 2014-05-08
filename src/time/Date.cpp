//
//  Date.cpp
//  HoraireExamens
//
//  Created by Quentin Peter on 20.11.13.
//  Copyright (c) 2013 Quentin. All rights reserved.
//

#include "Date.h"
int chiffre(char nbr){
    switch (nbr) {
        case '0':
            return 0;
            break;
        case '1':
            return 1;
            break;
        case '2':
            return 2;
            break;
        case '3':
            return 3;
            break;
        case '4':
            return 4;
            break;
        case '5':
            return 5;
            break;
        case '6':
            return 6;
            break;
        case '7':
            return 7;
            break;
        case '8':
            return 8;
            break;
        case '9':
            return 9;
            break;
            
        default:
            return -1;
            break;
    }
}

Date::Date(string date):a_jour(0), a_mois(0), a_annee(0){
    int* actuel(&a_jour);
    for (int i=0; i<date.size(); i++) {
        int nbr= chiffre(date[i]);
        if (nbr==-1) {
            if (actuel==&a_jour) {
                actuel=&a_mois;
            }
            else{
                actuel=&a_annee;
            }
        }
        else{
            (*actuel)*=10;
            (*actuel)+=nbr;
        }
        //Si on a un nombre
        
        
    }
}
Date::Date(int jour, int mois, int annee):a_jour(jour),a_mois(mois),a_annee(annee){}

bool Date::operator>(const Date& compare) const{
    if (a_annee>compare.a_annee) {
        return true;
    }
    else if(a_annee==compare.a_annee){
        if (a_mois>compare.a_mois) {
            return true;
        }
        else if(a_mois==compare.a_mois){
            if (a_jour>compare.a_jour) {
                return true;
            }
        }
       
        
    }
    return false;
}
bool Date::operator<(const Date& compare) const{
    if ((*this)>compare || (*this)==compare) {
        return false;
    }
    return true;
}
bool Date::operator==(const Date& compare) const{
    return a_annee==compare.a_annee && a_mois==compare.a_mois && a_jour==compare.a_jour;
}

ostream& operator<<(ostream& str, const Date& jour){
    if (jour.a_jour<10) {
        str<<"0";
    }
    str<<jour.a_jour<<'/';
    if (jour.a_mois<10) {
        str<<"0";
    }
    str<<jour.a_mois<<'/'<<jour.a_annee;
    return str;
}

int Date::distance(const Date& date1, const Date& date2){//Un peu approximatif, les mois ont 31 jours, les années 31*12
    int retour(0);
    retour += 31*12*(date1.a_annee-date2.a_annee);
    retour += 31*(date1.a_mois-date2.a_mois);
    retour += (date1.a_jour-date2.a_jour);
    return abs(retour);

}


Time::Time(string Time):a_heure(0), a_minute(0){
    int* actuel(&a_heure);
    for (int i=0; i<Time.size(); i++) {
        int nbr= chiffre(Time[i]);
        if (nbr==-1) {
            actuel=&a_minute;
        }
        else{
            (*actuel)*=10;
            (*actuel)+=nbr;
        }
        //Si on a un nombre
        
    }
}
bool Time::operator>(const Time& compare) const{
    if (a_heure>compare.a_heure) {
        return true;
    }
    else if(a_heure==compare.a_heure){
        if (a_minute>compare.a_minute) {
            return true;
        }
        
    }
    return false;
}
bool Time::operator<(const Time& compare) const{
    if ((*this)>compare || (*this)==compare) {
        return false;
    }
    return true;
}
bool Time::operator==(const Time& compare) const{
    return a_heure==compare.a_heure && a_minute==compare.a_minute ;
}

ostream& operator<<(ostream& str,const Time& heure){
    if (heure.a_heure<10) {
        str<<"0";
    }
    str<<heure.a_heure<<':';
    if (heure.a_minute<10) {
        str<<"0";
    }
    str<<heure.a_minute;
    return str;
}

int Time::distance(const Time& time1, const Time& time2){
    int retour(0);
    retour+= 60*(time1.a_heure-time2.a_heure);
    retour+= (time1.a_minute-time2.a_minute);
    return abs(retour);
}