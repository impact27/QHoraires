//
//  TimeTable.h
//  HorairesExams
//
//  Created by Quentin Peter on 19.11.13.
//  Copyright (c) 2013 Quentin. All rights reserved.
//

#ifndef __HorairesExams__TimeTable__
#define __HorairesExams__TimeTable__

#include "Header.h"
#include "Date.h"


//Les oraux demandent de placer des etudiant dans des jours
//les etudiants veulent savoir quels jours ils ont combien d'examens
//Les oraux veulent savoir combien de personne il y a dans chaque jour

//template <class T>
//class TimeSlot {
//    string a_heure;
//    string a_duree;
//    T a_ptr;
//public:
//    TimeSlot(Time heure, string duree, T ptr = NULL);
//};


template <class T>
class DayTimeTable {
private:
    DayTimeTable(const DayTimeTable& day){
    }
    list<T> a_nonFixe;
    map<Time, T> a_listeSlots;
    
    
    //liste cours lieu ce jour
    //chaque cours a liste de personne a classer  et une liste de slots
    
    
    bool slotIsFree(Time heure){
        if (a_listeSlots.find(heure) == a_listeSlots.end()) {
            return true;
        }
        else if (a_listeSlots[heure]==NULL){
            return true;
        }
        return false;
        
    }
    
    
    
public:
    DayTimeTable(){}
    DayTimeTable(const DayTimeTable& aCopier, map<T, T> mapT){//Copy and replace old ptr with new ptr
        for (auto it(aCopier.a_nonFixe.begin()); it!=aCopier.a_nonFixe.end(); it++) {
            a_nonFixe.push_back(mapT.at(*it));
        }
        for (auto it(aCopier.a_listeSlots.begin()); it!=aCopier.a_listeSlots.end(); it++) {
            a_listeSlots[it->first]=mapT.at(it->second);
            
        }
    }
    
    
    
    const list<T>& getNonFixe(){
        return a_nonFixe;
    }
    
    const map<Time, T>& getListeSlots(){
        return a_listeSlots;
    }
    
    int numberFreeSlots(){
        int retour=0;
        for (auto it(a_listeSlots.begin()); it!=a_listeSlots.end(); ++it){
            if (it->second==NULL) {
                retour++;
            }
        }
        retour-=a_nonFixe.size();
        return retour;
    }
    
    bool FreeSlots(){//Si il y a plus de slots libres que d'étudiants
        int i=0;
        for (typename std::map<Time,T>::iterator it=a_listeSlots.begin(); it!=a_listeSlots.end(); ++it){
             if (it->second==NULL) {
                 i++;
             }
         }
        if (i>a_nonFixe.size()) {//Si il y en a plus
            return true;
        }
        return false;
    }
    
    vector<Time> getTime(){
        vector<Time> retour;
        for (auto it(a_listeSlots.begin()); it!=a_listeSlots.end(); it++) {
            retour.push_back(it->first);
        }
        return retour;
    }
    
    int nombreElement(){
        int i=0;
        for (typename std::map<Time,T>::iterator it=a_listeSlots.begin(); it!=a_listeSlots.end(); ++it){
            if (it->second!=NULL) {
                i++;
            }
        }
        return i + (int)a_nonFixe.size();
        
    }
    
    bool addNonFixe(T ptr){
        auto it = find(a_nonFixe.begin(), a_nonFixe.end(), ptr);
        if (it!=a_nonFixe.end()) {
            return false;
        }
        else{
            a_nonFixe.push_back(ptr);
            return true;
        }
        
    }
    
    T getNonFixeRandom(){//Delete from nonFixe
        int rand= hasard::uniforme(0,a_nonFixe.size());
        typename list<T>::iterator iterNew= a_nonFixe.begin();
        advance(iterNew,rand);
        T retour=(*iterNew);
        a_nonFixe.erase(iterNew);
        return retour;
    }
    
    Time premierePlaceLibre(){//On renvoie la première place libre
        for (typename std::map<Time,T>::iterator it=a_listeSlots.begin(); it!=a_listeSlots.end(); ++it){
            if (it->second==NULL) {
                return Time(it->first);
            }
        }
        return Time("25:00");//Error
    }
    
    bool isDistributed(){
        if (a_nonFixe.size()==0) {
            return true;
        }
        return false;
    }
    
    bool addFixe(Time time, T ptr){
        //Si le slot n'existe pas ou qu'il est NULL
        if (slotIsFree(time)) {
            a_listeSlots[time]= ptr;
            return true;
        }
        return false;
        
    }
    int nearerTime(Time heure){
        int retour(24*60);
        for (auto it(a_listeSlots.begin()); it!=a_listeSlots.end(); it++) {
            int dis(Time::distance(it->first, heure));
            if (dis<retour&&it->second!=NULL) {
                retour=dis;
            }
        }
        return retour;
        
    }
    
    bool placeNonFixeLoin(T ptr, Time heure,int distance){
        typename std::list<T>::iterator it;
        it = find(a_nonFixe.begin(), a_nonFixe.end(), ptr);
        if (it==a_nonFixe.end()) {
            return false;
        }
        else{
            //Si l'heure est plus loin que distance
            if (nearerTime(heure)>=distance) {
                a_nonFixe.erase(it);
                return addFixe(heure, ptr);
            }
            else{
                return false;
            }
            
            
    
        }
    }
    friend ostream& operator<<(ostream& str, const DayTimeTable<T>& jour){
        for (auto it(jour.a_listeSlots.begin()); it != jour.a_listeSlots.end(); it++) {
            str<<it->first;
            if (it->second) {//SI ca existe
                str<<"\t"<<it->second->name()<<endl;
            }
            else{
                str<<endl;
            }
            
        }
        return str;
    }
    
    
};

template <class T>
class TimeTable : public map<Date, DayTimeTable<T>* > {
	
public:
	
	int numberFreeSlots(){
		int retour(0);
		for (auto it(this->begin()); it!=this->end(); it++) {
			retour+=it->second->numberFreeSlots();
		}
		
		return retour;
	}
	
	void addDate(Date day){
		(*this)[day]= new DayTimeTable<T>();
	}
	bool addOnDateAndTime(T toAdd, Date date, Time time){
		return this->at(date)->addFixe(time, toAdd);
	}
	unsigned long size(){
		return map<Date, DayTimeTable<T>* >::size();
	}
	
};
//

#endif /* defined(__HorairesExams__TimeTable__) */
