//
//  getInfoEPFL.m
//  GetInfoEPFL
//
//  Created by Quentin Peter on 07.05.14.
//  Copyright (c) 2014 Quentin. All rights reserved.
//

#import "getInfoEPFL.h"
#import <Foundation/Foundation.h>
#import "QNParser.h"

Student* GetInfoEPFL(const string& StudentName){
	@autoreleasepool {
		//On charge les termes de recherches séparés par des +
		NSString* info = [[NSString alloc] initWithUTF8String:StudentName.c_str()];
		if (info == nil){
			NSLog(@"Impossible de lire le nom - l'encodage est il UTF8?");
			return nullptr;
		}
		info = [info stringByReplacingOccurrencesOfString:@" " withString:@"+"];//Remplace les espaces avec des plus
		
		NSError *err = nil;
		QNParser* page1Parser = [[QNParser alloc] initWithURLString:[@"http://m.epfl.ch/search/directory/?q=" stringByAppendingString:info] error:&err];
		
		//En cas d'erreur
		if(err)
		{
			NSLog(@"Impossible de charger la page: %@, %@", [err localizedDescription], [err localizedFailureReason]);
		}
		
		//En cas de réussite
		else{
			NSString* premierResultat= [page1Parser getContentOfFirstBalise:@"li"];//Les resultats sont dans li
			if (!premierResultat|| [premierResultat isEqualToString:@"Aucun résultat trouvé"]) {
				NSLog(@"Aucun Résultat trouvé pour le nom %@",info);
			}
			//si il existe
			else{
				QNParser* parserPremierResultat = [[QNParser alloc] initWithString:premierResultat encoding:[page1Parser encoding]];
				
				NSString* urlInfos = [[parserPremierResultat getArgFromFirstBalise:@"a"] objectForKey:@"href"];//Les infos de l'url se trouvent dans <a href=...
				
				NSString* name = [parserPremierResultat getContentOfFirstBalise:@"h3"];//Le nom se trouve dans le premier h3
				
				
				NSRange rangeUrl = NSMakeRange(18, [urlInfos rangeOfString:@"/?q="].location-18);
				
				NSString* sciper = [urlInfos substringWithRange:rangeUrl];
				
				
				//To get the mail, we need to load the url
				urlInfos=[urlInfos stringByReplacingOccurrencesOfString:@" " withString:@"+"];//Sinon ca charge pas
				
				QNParser* page2Parser = [[QNParser alloc] initWithURLString:[@"http://m.epfl.ch" stringByAppendingString:urlInfos] error:&err];
				
				if(err)
				{
					NSLog(@"Impossible de charger la deuxième page");
				}
				else{
					
					NSString* email = [page2Parser firstEmail];
					
					if (!email) {
						NSLog(@"Adresse Mail non trouvée pour %@", name);
						email = @"UNKNOWN@epfl.ch";
					}
					string cName([name cStringUsingEncoding:NSUTF8StringEncoding]);
					string cMail([email cStringUsingEncoding:NSUTF8StringEncoding]);
					int cSciper = [sciper intValue];
					
					return new Student(cName, cMail, cSciper);
//					
//					NSString* fichier = [NSString stringWithFormat:@"%@\n%@\n%@",name,email,sciper];
//					
//					[fichier writeToFile:@"INFO.txt" atomically:NO encoding:NSUTF8StringEncoding error:&err];
				}
				
			}
			
		}
		
	}
    return nullptr;
}