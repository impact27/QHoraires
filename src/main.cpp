
//
//  main.cpp
//  HorairesExams
//
//  Created by Quentin Peter on 19.11.13.
//  Copyright (c) 2013 Quentin. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "HoraireExamens.h"

int main(int argc, const char * argv[])
{
	//On initialise le hasard
    hasard::init_hasard();
	
	//Déclaration des variables
	HoraireExamens meilleurHoraire, modeleHoraire;
	string fichierExa, fichierRequete;
	bool continuer (true);
    int meilleurScore(-10000000);
    int nombreTest(100);
	
	//Texte d'introduction
	
    cout<<"Bonjour et bienvenue dans le monde merveilleux de la création d'horaires d'oraux!\nCe magnifique programme a été écrit par Quentin Peter.\nNous allons maintenant calculer un horaire!\n"<<endl;
    
	//Verifie si les exames ont été précisés dans argv
	if (argc == 3) {
		fichierExa = argv[1];
		fichierRequete = argv[2];
	}
	else{//Si on ne les à pas reçu, on les demande
		cout<<"Ou sont les noms des fichiers d'examens et de requêtes?"<<endl;
		cin>>fichierExa;
		cin>>fichierRequete;
		cout<<endl;
	}

	//On charge les fichiers
	modeleHoraire.chargerEtudiants();
    modeleHoraire.Charger(fichierExa, fichierRequete);
	modeleHoraire.sauvegarderEtudiants();
    cout<<"Fichiers chargés"<<endl;
    
	//Boucle
    while (continuer) {
		for (int i(0); i<nombreTest; i++) {
			//Copie le modèle
			HoraireExamens horaireActuel(modeleHoraire);
			
			//Calcule l'horaire
			if (horaireActuel.CalculerHoraire()) {
				//Si c'est un succès, on enregistre
				//On calcule le score
				int score = horaireActuel.calculerScore();
				if (score>meilleurScore) {
					meilleurScore=score;
					meilleurHoraire=horaireActuel;
				}
				
			}
		}
		cout << "Meilleur Score:"<<setw (6)<<right<< meilleurScore<<flush;
		
		//On demande si il est suffisant
		cout <<"\trecommencer? (y/n)"<<flush;
		char reponse('n');
		cin>>reponse;
		if (reponse=='n') {
			continuer=false;
		}
        
		
		
    }

	//On enregistre le meilleur horaire
	meilleurHoraire.enregistrer();
    meilleurHoraire.HoraireEtudiant("HorairesEtudiants.txt");
	cout<<"Horaires enregistrés"<<endl;
    return 0;
}

