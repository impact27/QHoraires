//
//  TimeTable.cpp
//  QHorairesXCode
//
//  Created by Quentin Peter on 08.05.14.
//  Copyright (c) 2014 Quentin. All rights reserved.
//

#include <stdio.h>
#include "TimeTable.h"



#pragma mark DayTimeTable Implementation
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
/////////////DayTimeTable Implementation
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

template <class T>
bool DayTimeTable<T>::slotIsFree(Time heure){
	if (a_listeSlots.find(heure) == a_listeSlots.end()) {
		return true;
	}
	else if (a_listeSlots[heure]==NULL){
		return true;
	}
	return false;
	
}

//template <class T>
//DayTimeTable<T>::DayTimeTable(const DayTimeTable& aCopier, map<T, T> mapT){//Copy and replace old ptr with new ptr
//	for (auto it(aCopier.a_nonFixe.begin()); it!=aCopier.a_nonFixe.end(); it++) {
//		a_nonFixe.push_back(mapT.at(*it));
//	}
//	for (auto it(aCopier.a_listeSlots.begin()); it!=aCopier.a_listeSlots.end(); it++) {
//		a_listeSlots[it->first]=mapT.at(it->second);
//		
//	}
//}

template <class T>
int DayTimeTable<T>::numberFreeSlots(){
	int retour=0;
	for (auto it(a_listeSlots.begin()); it!=a_listeSlots.end(); ++it){
		if (it->second==NULL) {
			retour++;
		}
	}
	retour-=a_nonFixe.size();
	return retour;
}

template <class T>
bool DayTimeTable<T>::FreeSlots(){//Si il y a plus de slots libres que d'étudiants
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


template <class T>
vector<Time> DayTimeTable<T>::getTime(){
	vector<Time> retour;
	for (auto it(a_listeSlots.begin()); it!=a_listeSlots.end(); it++) {
		retour.push_back(it->first);
	}
	return retour;
}


template <class T>
int DayTimeTable<T>::nombreElement(){
	int i=0;
	for (typename std::map<Time,T>::iterator it=a_listeSlots.begin(); it!=a_listeSlots.end(); ++it){
		if (it->second!=NULL) {
			i++;
		}
	}
	return i + (int)a_nonFixe.size();
	
}

template <class T>
bool DayTimeTable<T>::addNonFixe(T ptr){
	auto it = find(a_nonFixe.begin(), a_nonFixe.end(), ptr);
	if (it!=a_nonFixe.end()) {
		return false;
	}
	else{
		a_nonFixe.push_back(ptr);
		return true;
	}
	
}


template <class T>
T DayTimeTable<T>::getNonFixeRandom(){//Delete from nonFixe
	int rand= hasard::uniforme(0,a_nonFixe.size());
	typename list<T>::iterator iterNew= a_nonFixe.begin();
	advance(iterNew,rand);
	T retour=(*iterNew);
	a_nonFixe.erase(iterNew);
	return retour;
}

template <class T>
Time DayTimeTable<T>::premierePlaceLibre(){//On renvoie la première place libre
	for (typename std::map<Time,T>::iterator it=a_listeSlots.begin(); it!=a_listeSlots.end(); ++it){
		if (it->second==NULL) {
			return Time(it->first);
		}
	}
	return Time("25:00");//Error
}

template <class T>
bool DayTimeTable<T>::isDistributed(){
	if (a_nonFixe.size()==0) {
		return true;
	}
	return false;
}

template <class T>
bool DayTimeTable<T>::addFixe(Time time, T ptr){
	//Si le slot n'existe pas ou qu'il est NULL
	if (slotIsFree(time)) {
		a_listeSlots[time]= ptr;
		return true;
	}
	return false;
	
}

template <class T>
const list<T>& DayTimeTable<T>::getNonFixe(){
	return a_nonFixe;
}

//template <class T>
//const map<Time, T>& DayTimeTable<T>::getListeSlots(){
//	return a_listeSlots;
//}

template <class T>
int DayTimeTable<T>::nearerTime(Time heure){
	int retour(24*60);
	for (auto it(a_listeSlots.begin()); it!=a_listeSlots.end(); it++) {
		int dis(Time::distance(it->first, heure));
		if (dis<retour&&it->second!=NULL) {
			retour=dis;
		}
	}
	return retour;
	
}

template <class T>
bool DayTimeTable<T>::placeNonFixeLoin(T ptr, Time heure,int distance){
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

template <class T>
DayTimeTable<T>* DayTimeTable<T>::copyDayTimeTableWithMap(map<T, T> myMap){
	return new DayTimeTable<T>(*this,myMap);
}


#pragma mark TimeTable Implementation
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
/////////////TimeTable Implementation
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

template <class T>
DayTimeTable<T>*&  TimeTable<T>::operator[](Date date){
	addDate(date);
	return map<Date, DayTimeTable<T>* >::operator[](date);
}

template <class T>
void TimeTable<T>::setDayTimeTableOnDate(DayTimeTable<T>* dtt, Date date){
	if (hasDate(date)){
		delete this->map<Date, DayTimeTable<T>* >::operator[](date);
	}
	this->map<Date, DayTimeTable<T>* >::operator[](date)= dtt;
}


template <class T>
int  TimeTable<T>::numberFreeSlots(){
	int retour(0);
	for (auto it(this->begin()); it!=this->end(); it++) {
		retour+=it->second->numberFreeSlots();
	}
	
	return retour;
}

template <class T>
int TimeTable<T>::numberObjectsOnDate(Date date){
	if (!hasDate(date)) {
		return 0;
	}
	return this->at(date)->nombreElement();
}

template <class T>
void TimeTable<T>::addDate(Date day){
	if (!hasDate(day)) {
		this->map<Date, DayTimeTable<T>* >::operator[](day)= new DayTimeTable<T>();
	}
	
}

template <class T>
bool TimeTable<T>::addOnDateAndTime(T toAdd, Date date, Time time){
	return (*this)[date]->addFixe(time, toAdd);
}

template <class T>
bool TimeTable<T>::hasDate(Date date){
	return this->count(date)==1;
}

template <class T>
void TimeTable<T>::addOnDate(T toAdd, Date date){
	(*this)[date]->addNonFixe(toAdd);
}

template <class T>
unsigned long TimeTable<T>::numberOfDays(){
	return map<Date, DayTimeTable<T>* >::size();
}

template <class T>
TimeTable<T>::~TimeTable(){
	for (auto it(this->begin()); it!=this->end(); it++) {
		delete it->second;//TODO:PROBLEM
	}
}

template <class T>
Date TimeTable<T>::getDateAtIndex(unsigned long index){
	if (index<numberOfDays()) {
		auto it = this->begin();
		advance(it,index);
		return it->first;
	}
	return Date("");
}

template <class T>
vector<Date> TimeTable<T>::listeDatesLibres(){
	vector<Date> retour;
	for (auto it(this->begin()); it!=this->end(); it++) {
		if (it->second->FreeSlots()) {//Si il y a des places libres
			retour.push_back(it->first);
		}
	}
	return retour;
}

template <class T>
bool TimeTable<T>::hasFreeSlotsOnDate(Date date){
	if(hasDate(date)){
		return this->at(date)->FreeSlots();
	}
	return false;
}

template <class T>
TimeTable<T> TimeTable<T>::copyWithMap(map<T, T> map){
	TimeTable<T> retour;
	for (auto it(this->begin()); it!=this->end(); it++) {
		retour.setDayTimeTableOnDate(it->second->copyDayTimeTableWithMap(map),it->first);
	}
	return retour;
}

template <class T>
bool TimeTable<T>::addOnDateFarFrom(T toAdd, Date date, Time time, int minDist){
	return (*this)[date]->placeNonFixeLoin(toAdd, time,minDist);//On veut le placer a plus de deux heures d'un autre exa
}

template <class T>
void TimeTable<T>::afficher(ofstream& fSave){
	for (auto it(this->begin()); it!=this->end(); it++) {
		fSave<<endl<<"\t  "<<it->first<<endl<<endl;
		fSave<<*(it->second);//Pas de endl car déjà a la fin de chaque nom
	}
}



