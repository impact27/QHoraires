//
//  Examen.cpp
//  HoraireExamens
//
//  Created by Quentin Peter on 19.11.13.
//  Copyright (c) 2013 Quentin. All rights reserved.
//

#include "Examen.h"
#include "Student.h"

Examen::Examen(string nom, string nomProf, string Salle, string Type, string duration):a_name(nom),a_nomProf(nomProf),a_Room(Salle),a_type(Type),a_duration(duration){}




Ecrit::Ecrit(Date date,string nom, string nomProf, string Salle, Time  time, string duration, list<Student*> listeEtudiants):Examen(nom,nomProf,Salle,"Ecrit",duration),a_time(time),a_listeEtudiants(listeEtudiants),a_date(date){
    for (list<Student*>::iterator i(listeEtudiants.begin()); i!=listeEtudiants.end(); ++i) {
        // On ajoute la classe a tout les étudiants
        (*i)->addExam(date,time,this);
    }


}

Ecrit* Ecrit::copyEcritWithStudentMap(map<Student*,Student*> mapStudent){
    list<Student*> nouveauStudent;
    for (auto it(a_listeEtudiants.begin()) ; it!=a_listeEtudiants.end(); it++) {
        nouveauStudent.push_back(mapStudent.at(*it));
    }
    return new Ecrit(a_date, a_name, a_nomProf,a_Room,a_time,a_duration,nouveauStudent);
}


int Oral::placesRestantes(){
	return a_timeTable.numberFreeSlots();
	
	
}
Oral::Oral(string nom, string nomProf, string Salle,string duration, list<Student*> listeEtudiants):Examen(nom,nomProf,Salle,"Oral",duration),a_listEtudiantARepartir(listeEtudiants){}
bool Oral::placesSuffisantes(){
    if (a_listEtudiantARepartir.size()>placesRestantes()) {
        return false;
    }
    return true;
}
void Oral::addDay(Date day){
	a_timeTable.addDate(day);
    
}
void Oral::addTime(Date day,Time time,Student* etudiant){// si il y a plus de noms que de places, on envoie une exception.
    
    if(!a_timeTable.addOnDateAndTime(etudiant, day, time)){
        cout << name()<< " a déjà un étudiant à "<<time << endl;
    }
    //On ajoute pour l'étudiant aussi si il existe
    if (etudiant) {
        if(!etudiant->addExam(day, time, this)){
            cout << etudiant->name()<< "a déjà un examen à "<<time<<" le "<< day << endl;
        }
        //On enléve l'étudiant de la liste des étudians a placer
        auto it(find(a_listEtudiantARepartir.begin(), a_listEtudiantARepartir.end(), etudiant));
        if (it!=a_listEtudiantARepartir.end()) {
            a_listEtudiantARepartir.erase(it);
        }
    }
    
    
}

bool Oral::JourSeul(){
    bool modifie = false;
    //Si il y a un seul jour, on met tout les étudiants dans ce jour
    if (a_timeTable.size()==1) {
        list<Student*>::iterator it=a_listEtudiantARepartir.begin();
        while(it != a_listEtudiantARepartir.end()){
            
            //On ajoute a l'horaire
            a_timeTable.begin()->second->addNonFixe((*it));
            
            
            //On dit a l'étudiant de l'ajouter
            (*it)->addExam(a_timeTable.begin()->first, this);
            
            it=a_listEtudiantARepartir.erase(it);
            modifie=true;
        }
        
    }
    else if (a_timeTable.size()>1){
        //Si il y a plusieurs jours, on doit demander à chaque personne si elle a déjà des exas les autres jours
        
        //On boucle sur les étudiants au hasard
        list<Student*> listeAEvaluer = a_listEtudiantARepartir;
        //On supprime tout les etudiants de la liste pour les remettre après
        a_listEtudiantARepartir.clear();
        
        while (listeAEvaluer.size()!=0) {//Tant qu'il y a des étudiants, on en tire un au hasard et on le place ou il faut
            int randNum = hasard::uniforme(0, listeAEvaluer.size());
            list<Student*>::iterator it = listeAEvaluer.begin();
            advance(it, randNum);
            vector<Date> listeDate = listeDatesLibres();
            vector<int> listeNbrExa = (*it)->nbrExamenDate(listeDate);
            //Si une seule date va, on le met, sinon on passe au suivant
            int nbrZero=0;
            Date selection("0.0.0");
            for (int i=0; i<listeNbrExa.size(); i++) {
                if (listeNbrExa[i]==0) {
                    nbrZero++;
                    selection=listeDate[i];
                }
            }
            if (nbrZero==1) {
                a_timeTable.at(selection)->addNonFixe((*it));
                (*it)->addExam(selection, this);
                modifie=true;
            }
            else{
                a_listEtudiantARepartir.push_back((*it));
                
            }
            listeAEvaluer.erase(it);
            
            
        }
    }
    
    
    
    return modifie;
}

vector<Date> Oral::listeDatesLibres(){
    vector<Date> retour;
    for (auto it(a_timeTable.begin()); it!=a_timeTable.end(); it++) {
        if (it->second->FreeSlots()) {//Si il y a des places libres
            retour.push_back(it->first);
        }
    }
    return retour;
}

string Oral::name() const{
    return a_name;
}
bool Oral::requestDate(Student* asker,Date jour){
    if (IsDateExam(jour)) {//Si le jour est un jour d'examen
        if (a_timeTable.at(jour)->FreeSlots()) {//Si il y a de la Place
            auto it(find(a_listEtudiantARepartir.begin(), a_listEtudiantARepartir.end(), asker));
            if (it!=a_listEtudiantARepartir.end()) {//Si c'est un des étudiant du cours
                a_timeTable.at(jour)->addNonFixe(asker);
                a_listEtudiantARepartir.erase(it);
            }
            else{
				cout<<"\nL'étudiant "<<asker->name()<< " n'est pas inscrit dans "<< name()<<" ou à fait deux demandes."<<endl;
            }
        }
    }
    return false;
}

bool Oral::IsDateExam(Date jour){
    if (a_timeTable.find(jour) == a_timeTable.end()) {
        return false;
    }
    return true;
}

void Oral::placerReste(){
    
    while (a_listEtudiantARepartir.size()!=0) {//Tant qu'il y a des étudiants, on en tire un au hasard et on le place
        //On tire un étudiant au hasard
        int randNum = hasard::uniforme(0, a_listEtudiantARepartir.size());
        list<Student*>::iterator it = a_listEtudiantARepartir.begin();
        advance(it, randNum);
        vector<Date> listeDateOrdonee = listeDatesLibres();
        //On mélange l'orgre des dates
        vector<Date> listeDate;
        while (listeDateOrdonee.size()!=0) {
            int randNum = hasard::uniforme(0, listeDateOrdonee.size());
            auto itDate = listeDateOrdonee.begin();
            advance(itDate, randNum);
            listeDate.push_back(*itDate);
            listeDateOrdonee.erase(itDate);
            
        }
        vector<int> listeNbrExa = (*it)->nbrExamenDate(listeDate);
        //On le met dans la première place sans examens, ou dans la premiere place avec un, etc
        bool place = false;
        for (int i=0; !place&& i<11; i++) {//i est le nombre d'examens ce jour la (petit?)
            if (i>9) {
                cout<<"Erreur, on ne peut pas mettre plus de neuf examens le même jour"<<endl;
                throw 13;
            }
            for (int j=0; j<listeNbrExa.size(); j++) {//Sur toute les possibilités
                if (listeNbrExa[j]==i&&!place) {//On ne refait pas la boucle si on a fini
                    Date selection=listeDate[j];
                    a_timeTable.at(selection)->addNonFixe((*it));
                    (*it)->addExam(selection, this);
                    a_listEtudiantARepartir.erase(it);
                    place=true;
                    
                }
            }

        }
        
    }
}

bool Oral::repartirCases(){
    if (a_listEtudiantARepartir.size()!=0) {
        cout << "Il reste des étudiants a répartir dans "<<name()<<endl;
        return false;
    }
    for (auto it(a_timeTable.begin()); it!=a_timeTable.end(); it++) {
        //Si ce jour n'est pas complet, on fait des trucs:
        if (it->second->getNonFixe().size()!=0) {
            //Tant qu'il reste des non fixe on les tire au hasard et on relance
            //On calcule le nombre maximum d'essais qu'on fait:
            unsigned long Max(2*it->second->getNonFixe().size());
            unsigned long nbrAbandons(0);
            
            while (!it->second->isDistributed()&&nbrAbandons<Max) {//Tant qu'il reste des gens a répartir et qu'on a pas abandonné
                Student* aPlacer=it->second->getNonFixeRandom();
                //Si la première place lui convient on le met, sinon on le remet dedans. on abandonne après avoir remis 2 fois tous les étudiants
                Time heure=it->second->premierePlaceLibre();
                
                if (heure==Time("25:00")) {
                    cout<<"IMPOSSIBLE de placer l'étudiant "<< aPlacer->name()<<endl;
                    return false;
                }
                else{
                    bool reussite(aPlacer->fixeExam(it->first, heure, this,2*60));
                    if (reussite) {
                        if (!it->second->addFixe(heure, aPlacer)) {
                            cout<<"La place libre ne l'était pas vraiment"<<endl;
                            return false;
                        }
                    }
                    else{
                        it->second->addNonFixe(aPlacer);
                        nbrAbandons++;//Si on arrive pas a le placer on le remet dans la pile et on en prend un autre
                    }
                }
            }
            if (nbrAbandons==Max) {
                return false;
            }
        }
        
    }
    return true;
}
/*
 on demande a chaque oral de voir si il y a des éleves qui ne peuvent qu'un jour et on remplis le jour tant qu'il y a de la place. Si il y a plus de place on écrit dans le log et on continue
 
 Si l'oral fait un jour on fixe. si il y a plus de place on met un mot dans le log et on envoie une exception
 
 */


ostream& operator<<(ostream& str, const Oral& exa){
    str<<"Examen"<<endl;
    str<<exa.a_name<<endl;
    str<<exa.a_nomProf<<endl;
    str<<exa.a_Room<<endl;
    str<<"Oral"<<endl;
    str<<exa.a_duration<<endl;
    str<<"Pas Liste Etudiants"<<endl;
    str<<"Liste Horaires"<<endl;
    for (auto it(exa.a_timeTable.begin()); it!=exa.a_timeTable.end(); it++) {
        str<<"Nouveau Jour"<<endl;
        str<<it->first<<endl;
        str<<*(it->second);//Pas de endl car déjà a la fin de chaque nom
    }
    
    
    str<<"Fin Liste Horaires"<<endl;
    str<<"Fin Examen"<<endl;
    
    return str;
}

void Oral::afficher(ofstream& fSave){
    fSave<<"Nom Cours:\t"<<a_name<<" (Oral)"<<endl;
    fSave<<"Professeur:\t"<<a_nomProf<<endl;
    fSave<<"Salle:\t\t"<<a_Room<<endl;
    fSave<<"Durée:\t\t"<<a_duration<<endl;
    for (auto it(a_timeTable.begin()); it!=a_timeTable.end(); it++) {
        fSave<<endl<<"\t  "<<it->first<<endl<<endl;
        fSave<<*(it->second);//Pas de endl car déjà a la fin de chaque nom
    }
}
Oral* Oral::copyOralWithStudentMap(map<Student*,Student*> mapStudent){
    list<Student*> newListARepartir;
    for (auto it(a_listEtudiantARepartir.begin()); it!=a_listEtudiantARepartir.end(); it++) {
        newListARepartir.push_back(mapStudent.at(*it));
    }
    Oral* retour= new Oral(a_name, a_nomProf, a_Room,a_duration, newListARepartir);
    //on remplis la map jour par jour
    for (auto it(a_timeTable.begin()); it!=a_timeTable.end(); it++) {
        retour->a_timeTable[it->first]=new DayTimeTable<Student*>(*(it->second),mapStudent);
        retour->fixerStudent();
    }
    
    return retour;
}
void Oral::fixerStudent(){
    for (auto jour(a_timeTable.begin()); jour != a_timeTable.end(); jour++) {
        list<Student*> nonFixJour =jour->second->getNonFixe();
        for (auto it(nonFixJour.begin()); nonFixJour.end()!=it; it++) {
            (*it)->addExam(jour->first, this);
        }
        auto mapFixJour(jour->second->getListeSlots());
        for (auto it(mapFixJour.begin()); it!=mapFixJour.end(); it++) {
            //On vérifie les pointeurs nuls
            if (it->second) {
                it->second->addExam(jour->first, it->first, this);
            }
            
        }
        
    }
}
Oral::~Oral(){
    for (auto it(a_timeTable.begin()); it!=a_timeTable.end(); it++) {
        delete it->second;
    }
}
