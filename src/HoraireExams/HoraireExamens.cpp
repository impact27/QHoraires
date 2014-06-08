//
//  HoraireExamens.cpp
//  HoraireExamens
//
//  Created by Quentin Peter on 19.11.13.
//  Copyright (c) 2013 Quentin. All rights reserved.
//

#include "HoraireExamens.h"

#include "GetInfoEPFL.h"

namespace QHString {
	
	bool verifierLigne(ifstream& flux,const string& ligne){
		string lecture;
		getline(flux, lecture);
		if (lecture!=ligne) {
			if (lecture != "") {//Sinon c'est normal
				cout << lecture << "!=" << ligne << endl;
			}
			return false;
		}
		return true;
	}
	
	
	void myReplace(std::string& str, const std::string& oldStr, const std::string& newStr)
	{
		size_t pos = 0;
		while((pos = str.find(oldStr, pos)) != std::string::npos)
		{
			str.replace(pos, oldStr.length(), newStr);
			pos += newStr.length();
		}
	}
	
	bool cmpNames(const std::string& name1, const std::string& name2){
		return name1.find(name2)!= string::npos || name2.find(name1)!= string::npos;
	}
	
}


HoraireExamens::HoraireExamens(){
    //hasard::init_hasard();//On initialise le hasard
}

HoraireExamens::HoraireExamens(const HoraireExamens& aCopier){
    (*this) = aCopier;
}

//ON CHARGE



void HoraireExamens::Charger(/*string fileStudent,*/ string fileExams, string fileRequest){
    
    /*
    //On charge les étudiants
    
    */
    
    //On charge les cours
    ifstream fOrals;
    fOrals.open(fileExams.c_str());
    if (fOrals.fail()) {
        cout<<"Le fichier Examens n'a pas pu être lu"<<endl;
        
    }
    else{
        cout<<"Chargement des Examens: "<<endl;
        //On peux lire les cours tant qu'il y a des changements on continue
        bool changements(true);
        while (changements) {
            changements= lireCour(fOrals);
			if (changements) {
				cout<<"."<<flush;
			}
        }
		cout<<endl;
        //Les etudiants et les classes sont maintenant reliées
    }
    
    //On ferme le flux
    fOrals.close();
    
    
    
    //On charge les requetes
    //On charge les cours
    ifstream fRequest;
    fRequest.open(fileRequest.c_str());
    if (fRequest.fail()) {
        cout<<"Le fichier Request n'a pas pu être lu"<<endl;
        
    }
    else{
        if(QHString::verifierLigne(fRequest, "Debut Requetes")){
			cout<<"Chargement des Requetes: "<<endl;
            bool continuer(true);
            while (continuer&& !fRequest.eof()) {
                string mail, cours, date;
                getline(fRequest, mail);
                if (mail=="Fin Requetes") {
                    continuer=false;
                }
                else{
                    getline(fRequest, cours);
                    getline(fRequest, date);
                    Oral* exa = FindOral(cours);
                    Student* asker = FindStudentMail(mail);
                    if (exa!=NULL && asker!=NULL) {
                        exa->requestDate(asker,Date(date));
						cout<<"."<<flush;
                    }
                    else{
                        cout<< "\n Requête incorrecte" <<endl;
                    }
                }
            }
			cout<<endl;
        }
        
        
    }
    //On Ferme le flux
    fRequest.close();

}



bool HoraireExamens::lireCour(ifstream& fOrals){
    
    //pour chaque cours, on lit le nom, nomProf, Salle, Type
    string nom, nomProf, Salle, Type;
    
    if (!QHString::verifierLigne(fOrals,"Examen")) {
        return false;
    }
    
    getline(fOrals, nom);
    getline(fOrals, nomProf);
    getline(fOrals, Salle);
    getline(fOrals, Type);
    
    //On enregistre la durée
    
    string duration;
    getline(fOrals, duration);

    
    //On charge la liste d'étudiants
    list<Student*> listeEtudiants;
    
    
    if (QHString::verifierLigne(fOrals,"Liste Etudiants")) {//On regarde si il y a une liste d'étudiant (Pour les oraux, ils ne sont la que si ils ne sont pas fixés dans une liste)
        
        string nomEtudiant;
        getline(fOrals, nomEtudiant);
        bool continuer=true;
        while (continuer) {
            unsigned long empl(nomEtudiant.find_first_of(";"));
            string nomEtudiantActuel;
            if (string::npos==empl) {
                continuer=false;
                nomEtudiantActuel=nomEtudiant;
            }
            else{
                nomEtudiantActuel=(nomEtudiant.substr(0,empl));
                nomEtudiant=nomEtudiant.substr(empl+2);
            }
            listeEtudiants.push_back(addStudent(nomEtudiantActuel));
            
            
        }
    }
    
    //Si le Type est un Ecrit, on charge l'heure et la liste des étudiants
    
    if (Type=="Ecrit") {
        string date,time;
        getline(fOrals, date);
        getline(fOrals, time);
        
        //On crée l'écrit
        a_listEcrit.push_back(new Ecrit(Date(date),nom,nomProf,Salle, time,duration, listeEtudiants));
        
        
    }
    else if (Type=="Oral"){
        a_listOral.push_back(new Oral(nom,nomProf,Salle,duration,listeEtudiants));
        
        
        //On lis les horaires
        if (QHString::verifierLigne(fOrals,"Liste Horaires")){
            //Les elements de la liste d'horaire sont de la forme 07:12 (nom) ! 5 caractères
            bool contiuner(true);
            string day;
            while (contiuner) {
                string ligne, time, nameStudent;
                getline(fOrals, ligne);
                if (ligne=="Nouveau Jour") {//On met cette ligne a chaque debut de jour
                    getline(fOrals, day );//On récupere le jour
                    a_listOral.back()->addDay(day);
                }
                else if (ligne=="Fin Liste Horaires") {
                    contiuner=false;
                }
                else{
                    Student* etudiant(NULL);
                    if (ligne.size() < 7) {
                        time=ligne;
                    }
                    else{
                        time = ligne.substr(0,5);
                        nameStudent = ligne.substr(6);//Le nom commence après un espace
                        etudiant = addStudent(nameStudent);
                    }
                    //Si il y a un étudiant, on l'ajoute, sinon on Met un slot vide
                    a_listOral.back()->addTime(day, time, etudiant);
                    
                }
                
                
            }
            
            
        }
        
        
    }
	else{
		cout<<"Un examen n'est ni oral ni ecrit"<<endl;
		return false;
	}
    if(!QHString::verifierLigne(fOrals,"Fin Examen")){
        cout<<"Un examen n'à pas de balise de fin"<<endl;
		return false;
    }
    if (!a_listOral.back()->placesSuffisantes()) {
        cout<<"Un Oral n'a pas assez de Places"<<endl;
        return false;
    }
    return true;
    
    
}

Student* newStudentFromStream(ifstream& fInfo){
	string nom,email;
	int sciper(0);
	getline(fInfo, nom );
	getline(fInfo, email);
	fInfo >> sciper;
	fInfo.ignore(1000, '\n');
	if (nom == "") {
		return NULL;
	}
	else{
		return new Student(nom,email,sciper);
	}
}

Student* HoraireExamens::searchNewStudent(const string& nameStudent){
	
	Student* retour = GetInfoEPFL(nameStudent);
	
	if (retour) {
		if (QHString::cmpNames(retour->name(), nameStudent)) {//robuste si un des deux n'a pas de deuxième nom
			return retour;
			
		}
		else{
			cout<<"Le nom de EPFL ne correspond pas"<<endl;
			delete retour;
		}
	}
	return NULL;
}

//
//	string cmdStudent = nameStudent;
//	
//    QHString::myReplace(cmdStudent, "'", "\\'");//On doit enlever les ' car le système les interprète
//    
//    //GetInfoEPFL va crée INFO.txt avec les donées qu'il faut
//    
//    string commande= "./GetInfoEPFL "+cmdStudent;
//	
//    system(commande.c_str());
//    //On lit le fichier INFO.txt et on vérifie qu'on a le bon nom
//    ifstream fInfo;
//    fInfo.open("INFO.txt");
//    if (fInfo.fail()) {
//        cout<<"Le fichier Student n'a pas pu être lu"<<endl;
//        fInfo.close();
//        return NULL;
//        
//    }
//    else{
//		Student* retour = newStudentFromStream(fInfo);
//        
//		//On ferme et on supprime le fichier
//        fInfo.close();
//		remove("INFO.txt");
//		if (retour) {
//			if (QHString::cmpNames(retour->name(), nameStudent)) {//robuste si un des deux n'a pas de deuxième nom
//				return retour;
//				
//			}
//			else{
//				cout<<"Le nom de EPFL ne correspond pas"<<endl;
//				free(retour);
//			}
//		}
//		return NULL;
//    }
//}

Student* HoraireExamens::addStudent(string nameStudent){
	
	//Si il y a un nom pas Compris, on écrit un message dans le log et on le rajoute
    for (int i=0; i<a_listStudent.size(); i++) {
        if (QHString::cmpNames(a_listStudent[i]->name(), nameStudent)) {//If name already in the list
            return a_listStudent[i];
        }
    }
	
    //Sinon, il nous faut crée un nouvel étudiant. Pour avoir les infos on demande au site de l'EPFL a travers GetInfoEPFL
	Student* newStudent = searchNewStudent(nameStudent);
	
	if (newStudent) {
		a_listStudent.push_back( newStudent);
	}
	return newStudent;
    
}


Student* HoraireExamens::FindStudentMail(string mail){
    for (int i=0; i<a_listStudent.size(); i++) {
        if (a_listStudent[i]->eMail()==mail) {
            return a_listStudent[i];
        }
    }
    return NULL;
}
Oral* HoraireExamens::FindOral(string oral){
    for (int i=0; i<a_listOral.size(); i++) {
        if (a_listOral[i]->name()==oral) {
            return a_listOral[i];
        }
    }
    return NULL;
}




bool HoraireExamens::CalculerHoraire(/*int nombreTest*/){//Les plusieurs essais sont fait dans une autre boucle
    //Dans la première partie on fixe les jours
    
    //1// On fixe les personnes qui ne peuvent absolument pas d'autres jours
    //////////////////////////////////////////////////////////////////////
    
    //TODO
    
    //OU alors on les met a une bonne position dans les requêtes
    
    //ON enregistre l'état actuel dans le slot pour recommencer de là
    //enregistrerEtat(Debut);
    
    
    //On commence la boucle
    //for (int test(0); test<nombreTest; test++) {
        
        //2 On fixe les jours qui n'ont qu'un choix
        //////////////////////////////////////////////////////////////////////
        
        //Si les exas font un jour, on fixe le jour
        //Si ils font plus et que l'éleve n'y qu'une possibilité, on fixe aussi
        //On doit continuer la boucle tant qu'il y a des changement car un exa fixé après peut influencer un précédant
        bool changements(true);
        while (changements) {
            changements=false;
            for (int i=0; i<a_listOral.size(); i++) {
                changements = a_listOral[i]->JourSeul()||changements ;
            }
        }
        
        
        //3// On répartit le reste des places
        //////////////////////////////////////////////////////////////////////
        
        for (int i=0; i<a_listOral.size(); i++) {//Peut être mieux de les tirer au hasard? non parceque le plus tot possible pour laisser de la place
            a_listOral[i]->placerReste();
        }
        
        
        //4//Répartir Cases
        /////////////////////////////////////////
        
        //Maintenent que tout les étudiant ont été mis dans la time table, on demande a repartir

        //Tout les étudiants qui ont plusieurs Exa le même jour vont demander a être placé de manière espacé
    
    //On demande a chaque exa de remplir ses cases maintenant qu'il n'y a plus d'overlap(Si il y en a le score sera trop bas)
    
    for (int i(0); i<a_listOral.size(); i++) {
        if(!a_listOral[i]->repartirCases()){
//            cout<<"Impossible de répartir tout les étudiants"<<endl;
            return false;
        }
    }
    
    //Si un des étudiants a des exas a moins de deux heures d'équart, le calcul est un échec
    //TODO
    
        
    //}
    // On calcule le score de cette répartition et on recommence. Le meilleur score l'emporte. Le score est écrit dans le log a chaque fois
   
        
    return true;
    
    
}




int HoraireExamens::calculerScore(){//Renvoie le score de la répartition actiuelle

    int retour(0);
    for (auto it(a_listStudent.begin());it!=a_listStudent.end(); it++) {
        retour+= (*it)->calculerScore();
    }
    return retour;
/*
 
 Claculerscore
 
 somme score pour tout les étudiants
 
 /NombreEtudiants /(Nclasses-1) pour normaliser
 
 
 Score étudiant :
 
 1 pour jour consécutif, 2 pour 1 jour d'espace 3 pour 2 jours, 5 pour +3jours   -5*Ntrop pour exas le meme jour



*/}





template <class T>
void detruireVector(vector<T> vect) {
    for (int i(0); i<vect.size(); i++) {
        delete vect[i];
    }
}

void HoraireExamens::enregistrer(){
	
	for (auto it(a_listOral.begin()); it!=a_listOral.end(); it++) {
		//On charge les cours
		ofstream fSave;
		fSave.open((*it)->name().c_str());
		if (fSave.fail()) {
			cout<<"Echec d'écriture pour examen "<<(*it)->name()<<endl;
		}
		else{
			(*it)->afficher(fSave);
		}
		fSave.close();
	}
    
}

void HoraireExamens::enregistrer(string nomFichier){
    //On charge les cours
    ofstream fSave;
    fSave.open(nomFichier.c_str());
    if (fSave.fail()) {
        cout<<"On ne peut pas écrire ici"<<endl;
        
    }
    else{
        for (auto it(a_listOral.begin()); it!=a_listOral.end(); it++) {
            //fSave<<*(*it);
            (*it)->afficher(fSave);
            fSave<<endl<<endl<<endl;
        }
        
        
    }
    fSave.close();
}
void HoraireExamens::sauvegarderEtudiants(){
    ofstream fSave;
    fSave.open("listeEtudiant.txt");
    if (!fSave.fail()) {
        for (auto it(a_listStudent.begin()); it!=a_listStudent.end(); it++) {
			fSave<<(*it)->name()<<endl<<(*it)->eMail()<<endl<<(*it)->sciper()<<endl;
        }
        
        
    }
    fSave.close();
}
void HoraireExamens::chargerEtudiants(){
	ifstream fstudent;
    fstudent.open("listeEtudiant.txt");
    if (fstudent.fail()) {
        fstudent.close();
    }
    else{
        //On ajoute tout les étudiants
        bool continuer(true);
        while (continuer&& !fstudent.eof()) {
            //Tant qu'on peut lire des étudiants on en lit
            Student* actuel = newStudentFromStream(fstudent);
            if (actuel) {
                a_listStudent.push_back(actuel);
            }
            else
                continuer=false;
        }
        fstudent.close();
        
    }
}
void HoraireExamens::HoraireEtudiant(string nomFichier){
    //On charge les cours
    ofstream fSave;
    fSave.open(nomFichier.c_str());
    if (fSave.fail()) {
        cout<<"On ne peut pas écrire ici"<<endl;
        
    }
    else{
        for (auto it(a_listStudent.begin()); it!=a_listStudent.end(); it++) {
            (*it)->afficher(fSave);
            fSave<<endl<<endl<<endl;
        }
        
        
    }
    fSave.close();
}

void HoraireExamens::viderVecteurs(){
    detruireVector<Student*>(a_listStudent);
    a_listStudent.clear();
    detruireVector<Oral*>(a_listOral);
    a_listOral.clear();
    detruireVector<Ecrit*>(a_listEcrit);
    a_listEcrit.clear();
}

HoraireExamens::~HoraireExamens(){
    viderVecteurs();
}


void HoraireExamens::operator=(const HoraireExamens& aCopier){
    map<Student*, Student*> mapStudent;//On donne l'ancien pour avoir le nouveau
    viderVecteurs();
    for (int i(0); i<aCopier.a_listStudent.size(); i++) {
        a_listStudent.push_back(aCopier.a_listStudent[i]-> newStudent());
        mapStudent[aCopier.a_listStudent[i]]=a_listStudent.back();
    }
    mapStudent[NULL]=NULL;
    for (auto it(aCopier.a_listEcrit.begin()); it != aCopier.a_listEcrit.end(); it++) {
        a_listEcrit.push_back((*it)->copyEcritWithStudentMap(mapStudent));
    }
    for (auto it(aCopier.a_listOral.begin()); it != aCopier.a_listOral.end(); it++) {
        a_listOral.push_back((*it)->copyOralWithStudentMap(mapStudent));
    }
}
