//
//  HoraireExamens.h
//  HoraireExamens
//
//  Created by Quentin Peter on 19.11.13.
//  Copyright (c) 2013 Quentin. All rights reserved.
//

#ifndef __HoraireExamens__HoraireExamens__
#define __HoraireExamens__HoraireExamens__

#include "Header.h"
#include "Student.h"
#include "Examen.h"
#include "TimeTable.h"

enum Enregistrement {
    Debut = 0,
    Meilleur = 1
    };

class HoraireExamens {
    vector<Student*> a_listStudent;
    vector<Oral*> a_listOral;
    vector<Ecrit*> a_listEcrit;
    
public:
    HoraireExamens();
    HoraireExamens(const HoraireExamens& aCopier);
   
    void Charger(/*string fileStudent,*/ string fileExams, string fileRequest);
    bool lireCour(ifstream& fOrals);
    Student* addStudent(string nameStudent); //Si il y a un nom pas Compris, on écrit un message dans le log et on le rajoute
	Student* searchNewStudent(const string& nameStudent);
    
    Student* FindStudentMail(string mail);
    Oral* FindOral(string oral);
    
    
    
    bool CalculerHoraire(/*int nombreTest*/);//on calcule un horaire si ca marche on renvoie true
    int calculerScore();//Renvoie le score de la répartition actiuelle
    //void enregistrerEtat(Enregistrement num);
    //Student* lireEtudiant(ifstream& fEtudiant);
    void enregistrer(string nomFichier);
	void enregistrer();
    void HoraireEtudiant(string nomFichier);
	void sauvegarderEtudiants();
	void chargerEtudiants();
    void viderVecteurs();
     ~HoraireExamens();
    
    void operator=(const HoraireExamens& aCopier);
    
};


#endif /* defined(__HoraireExamens__HoraireExamens__) */
