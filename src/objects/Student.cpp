//
//  Student.cpp
//  HorairesExams
//
//  Created by Quentin Peter on 19.11.13.
//  Copyright (c) 2013 Quentin. All rights reserved.
//

#include "Student.h"
#include "Examen.h"

Student::Student(string name, string email, int sciper): a_name(name),a_email(email),a_sciper(sciper){}

string Student::name() const{
    return a_name;
}
string Student::eMail(){
    return a_email;
}
int Student::sciper(){
    return a_sciper;
}

bool Student::addExam(Date date,Time time, Examen* ptrExa){
	return a_timeTable.addOnDateAndTime(ptrExa, date, time);
}

void Student::addExam(Date date, Examen* ptr){
    a_timeTable.addOnDate(ptr, date);
}

bool Student::fixeExam(Date date,Time time, Examen* ptrExa, int minDist){
	return a_timeTable.addOnDateFarFrom(ptrExa, date, time , minDist);
}

vector<int> Student::nbrExamenDate(vector<Date> listDate){
    vector<int> retour;
    for (int i(0); i<listDate.size(); i++) {
        retour.push_back(a_timeTable.numberObjectsOnDate(listDate[i]));
    }
    return retour;
}

int Student::calculerScore(){
    //On calcule la distance entre les dates. si une date a plusieurs objets on récupère les crénaux horaires et on les note
    int score(0);
    Date precedant(a_timeTable.begin()->first);//La date est la première
    for (auto it(a_timeTable.begin()); it!= a_timeTable.end(); it++) {
        //On calcule le nombre de jours depuis le precedant
        int distanceJours(Date::distance(precedant, it->first));
        if (distanceJours<3) {//Si il y a plus de trois jours le score deviens stable
            score+=distanceJours;
        }
        else{
            score+=3;
        }
        precedant=it->first;
        
        
        //Maintenant on vérifie qu'il n'y aie pas trop d'exas le même jour
        
        int nombreExamen(it->second->nombreElement());
        if (nombreExamen>2) {
            score-=10000;//On tue le score si il y a plus de deux exas, Ca ne devrait pas arriver
        }
        else if (nombreExamen==2){
            score-=1000;
            //Si on a deux exas, il nous faut les heures
            vector<Time> heures(it->second->getTime());
            if (heures.size()==2) {
                int distanceHeure (Time::distance(heures[0], heures[1]));
                //Si la distance est moins que deux heures, l'étudiant ne pourra pas aller a son autre exa
                if (distanceHeure<2*60) {
                    score-=10000;
                }
                else if (distanceHeure<4*60){
                    //Si on a moins de 4 heures on perd 10 points
                    score-=10;
                }
                else if (distanceHeure<6*60){
                    score -=8;
                }
                else{
                    score-=5;
                }
                
                
            }
            else{
                cout << "Le score est calculé pour des exas non fixés" <<endl;
            }
        }
    }
    
    
    return score;
    
    
    
}

Student* Student::newStudent(){
    return new Student(a_name,a_email,a_sciper);
}

void Student::afficher(ofstream& fSave){
    fSave<<a_name<<endl;
    fSave<<a_email<<endl<<endl;
    for (auto it(a_timeTable.begin()); it!=a_timeTable.end(); it++) {
        fSave<<endl<<"\t  "<<it->first<<endl<<endl;
        fSave<<*(it->second);//Pas de endl car déjà a la fin de chaque nom
    }
}