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


template <class T>
class DayTimeTable {
private:
	//Attributs
	list<T> a_nonFixe;
    map<Time, T> a_listeSlots;
	
	//Copie Pout éviter les copies
    DayTimeTable(const DayTimeTable& day){}
	DayTimeTable& operator=(DayTimeTable* toCopy){};
	
    //Vérifie si slot est libre
    bool slotIsFree(Time heure);
    
    
    
public:
	
	//Constructeur+ copie
    DayTimeTable(){}
    DayTimeTable(const DayTimeTable& aCopier, map<T, T> mapT){//Copy and replace old ptr with new ptr
		for (auto it(aCopier.a_nonFixe.begin()); it!=aCopier.a_nonFixe.end(); it++) {
			a_nonFixe.push_back(mapT.at(*it));
		}
		for (auto it(aCopier.a_listeSlots.begin()); it!=aCopier.a_listeSlots.end(); it++) {
			a_listeSlots[it->first]=mapT.at(it->second);
			
		}
	}
	DayTimeTable* copyDayTimeTableWithMap(map<T, T> myMap);
    
    //Getter pour attributs
    const list<T>& getNonFixe();
    const map<Time, T>& getListeSlots(){
		return a_listeSlots;
	}
	
	//Informations sur les slots
	int nombreElement();
	vector<Time> getTime();
	Time premierePlaceLibre();
	int nearerTime(Time heure);
	
	//Informations sur slots libres
    int numberFreeSlots();
    bool FreeSlots();
	
	//Infos générales
	bool isDistributed();
	
	//Ajouter des objets
    bool addNonFixe(T ptr);
    bool addFixe(Time time, T ptr);
	bool placeNonFixeLoin(T ptr, Time heure,int distance);
	
	//Hasard
    T getNonFixeRandom();
    
	//Affichage
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
	//No access to DayTimeTable directly
	DayTimeTable<T>*& operator[](Date date);
	
	//copy
	void FillWithTimeTableAndMap(const TimeTable<T>& toCopy, map<T, T> map);
	TimeTable<T> copyWithMap(map<T, T> map);
	TimeTable<T>& operator=(TimeTable<T> toCopy);
	
	//Infos
	int numberObjectsOnDate(Date date);
	bool hasDate(Date date);
	unsigned long numberOfDays();
	Date getDateAtIndex(unsigned long index);
	
	//Infos free slots/date
	vector<Date> listeDatesLibres();
	int numberFreeSlots();
	bool hasFreeSlotsOnDate(Date date);
	
	//Add date
	void addDate(Date day);
	void addOnDate(T toAdd, Date date);
	bool addOnDateAndTime(T toAdd, Date date, Time time);
	bool addOnDateFarFrom(T toAdd, Date date, Time time, int minDist);
	
	
	//Destructor
	~TimeTable();
	
	//Affichage
    friend ostream& operator<<(ostream& str, const TimeTable<T>& timeT){
		for (auto it(timeT.begin()); it!=timeT.end(); it++) {
			str<<"Nouveau Jour"<<endl;
			str<<it->first<<endl;
			str<<*(it->second);//Pas de endl car déjà a la fin de chaque nom
		}
		return str;
	}
	
	void afficher(ofstream& fSave);
	
	class iterator {
		typename map<Date, DayTimeTable<T> >::iterator a_mapIterator;
		
		
    public:
        iterator();
        iterator(const iterator&);
        ~iterator();
		
        iterator& operator=(const iterator&);
        bool operator==(const iterator&) const;
        bool operator!=(const iterator&) const;
		
        iterator& operator++();
		
        T& operator*() const;
        T* operator->() const;
    };
	class slot_iterator : public iterator{
		
		
	};
	class nonFixe_iterator : public iterator{
		typename list<T>::iterator a_nonFixIt;
		
	}
	;
	
};
#include "TimeTable.cpp"
#endif /* defined(__HorairesExams__TimeTable__) */
