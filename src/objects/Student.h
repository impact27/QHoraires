//
//  Student.h
//  HorairesExams
//
//  Created by Quentin Peter on 19.11.13.
//  Copyright (c) 2013 Quentin. All rights reserved.
//

#ifndef __HorairesExams__Student__
#define __HorairesExams__Student__
#include "Header.h"
#include "TimeTable.h"
#include "Date.h"

class Examen;
class Student {
private:
    string a_name;
    string a_email;
    int a_sciper;
	TimeTable<Examen*> a_timeTable;
    
    
public:
    Student(string name, string email, int sciper);
    string name() const;
    string eMail();
    int sciper();
    
    bool addExam(Date date,Time time, Examen* ptrExa);
    void addExam(Date date, Examen* ptr);
    bool fixeExam(Date date,Time time, Examen* ptrExa, int minDist);
    
    vector<int> nbrExamenDate(vector<Date> listDate);
    
    int calculerScore();
    Student* newStudent();
    void afficher(ofstream& fSave);
};



#endif /* defined(__HorairesExams__Student__) */
