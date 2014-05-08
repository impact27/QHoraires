//
//  Date.h
//  HoraireExamens
//
//  Created by Quentin Peter on 20.11.13.
//  Copyright (c) 2013 Quentin. All rights reserved.
//

#ifndef __HoraireExamens__Date__
#define __HoraireExamens__Date__

#include "Header.h"

class Date {
    int a_jour;
    int a_mois;
    int a_annee;
    
public:
    Date(string date);
    Date(int jour, int mois, int annee);
    bool operator>(const Date& compare) const;
    bool operator<(const Date& compare) const;
    bool operator==(const Date& compare) const;
    friend ostream& operator<<(ostream& str, const Date& jour);
    static int distance(const Date& date1, const Date& date2);//Un peu approximatif, les mois ont 31 jours, les années 31*12
};


class Time {
    int a_heure;
    int a_minute;
    
public:
    Time(string time);
    bool operator>(const Time& compare) const;
    bool operator<(const Time& compare) const;
    bool operator==(const Time& compare) const;
    friend ostream& operator<<(ostream& str,const Time& heure);
    static int distance(const Time& time1, const Time& time2);
};
int chiffre(char nbr);

#endif /* defined(__HoraireExamens__Date__) */
