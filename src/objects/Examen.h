//
//  Examen.h
//  HoraireExamens
//
//  Created by Quentin Peter on 19.11.13.
//  Copyright (c) 2013 Quentin. All rights reserved.
//

#ifndef __HoraireExamens__Examen__
#define __HoraireExamens__Examen__

#include "Header.h"
#include "TimeTable.h"
#include "Date.h"
class Student;
class Examen {
protected:
    string a_name;
    string a_nomProf;
    string a_Room;
    string a_type;
    string a_duration;
    
public:
    Examen(string nom, string nomProf, string Salle, string Type, string duration);
    string name(){
        return a_name;
    }
};

class Ecrit: public Examen {
    Date a_date;
    Time a_time;
    list<Student*> a_listeEtudiants;
    
public:
    Ecrit(Date date,string nom, string nomProf, string Salle, Time  time, string duration, list<Student*> listeEtudiants);
    Ecrit* copyEcritWithStudentMap(map<Student*,Student*> mapStudent);
};


class Oral : public Examen{
    list<Student*> a_listEtudiantARepartir;
	TimeTable<Student*> a_timeTable;
    
    int placesRestantes();
    
public:
    void afficher(ofstream& fSave);
    bool placesSuffisantes();
    void addDay(Date day);
    Oral* copyOralWithStudentMap(map<Student*,Student*> mapStudent);
    friend ostream& operator<<(ostream& str, const Oral& exa);
    
    //CONSTRUCTION
    Oral(string nom, string nomProf, string Salle,string duration, list<Student*> listeEtudiants);
    void addTime(Date day,Time time,Student* etudiant);// si il y a plus de noms que de places, on envoie une exception.
    vector<Date> listeDatesLibres();
    string name() const;
    
    bool requestDate(Student* asker,Date jour);
    
    //Distribution
    bool JourSeul();//On fixe les jours si il y a un seul choix sans recoupement (ou un seul choix seulement)
    
    bool IsDateExam(Date jour);
    
    void placerReste();//On place les étudians qui restent
    //pour chaque personne dans la liste a placer, regarde quels jours jouent pour elle. Si aucun la placer dans un des jours Pris (sauf si déjà deux. Si tout les jours déja deux aléatoire plus log). La placer dans un jour aléatoire
    
    bool repartirCases();//On répartit les étduiants dans des heures aléatoirement
    
    void addExamToStudent();
    
};

#endif /* defined(__HoraireExamens__Examen__) */
