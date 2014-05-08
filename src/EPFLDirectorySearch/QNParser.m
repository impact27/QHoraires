//
//  QNParser.m
//  GetInfoEPFL
//
//  Created by Quentin Peter on 07.05.14.
//  Copyright (c) 2014 Quentin. All rights reserved.
//

#import "QNParser.h"

NSRange QNMakeRange(NSUInteger from, NSUInteger to){
	return NSMakeRange(from, to-from);
}

@implementation NSString (quotationExtention)

-(NSString*)stringByProtectingQuotationWithEncoding:(NSStringEncoding)encoding{
	NSMutableArray* myArray= [NSMutableArray arrayWithArray:[self componentsSeparatedByString:@"\""]];
	for (NSUInteger i =0; i<[myArray count]; i++) {
		if (i%2!=0) {//Si i est impaire, on remplace les espaces
			[myArray replaceObjectAtIndex:i withObject:[[myArray objectAtIndex:i] stringByAddingPercentEscapesUsingEncoding:encoding]];
		}
	}
	return [myArray componentsJoinedByString:@"\""];
}
-(NSString*)stringByRemovingQuotationAndProtectionWithEncoding:(NSStringEncoding)encoding{
	NSString* retour = [self stringByReplacingPercentEscapesUsingEncoding:encoding];
	return [retour stringByTrimmingCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"\""]];
}

@end




@implementation QNParser{
	NSString* a_string;
	NSStringEncoding a_encoding;
}

NSURL* urlFromString(NSString* string){
    return [NSURL URLWithString:[string stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding ]];
}




-(NSStringEncoding) encoding{
	return a_encoding;
}
-(NSString*) string{
	return a_string;
}
-(NSString*) firstEmail{
	NSRange findBegin = [a_string rangeOfString:@"\"mailto:"];
	if (findBegin.location == NSNotFound) {
		return nil;
	}
	else{
		NSRange findEnd = [ a_string rangeOfString:@"\"" options:NSLiteralSearch range:QNMakeRange(findBegin.location+findBegin.length, [a_string length])];
		
		NSRange subRange= QNMakeRange(findBegin.location+findBegin.length, findEnd.location);
		return [a_string substringWithRange:subRange];
	}
}

-(QNParser*)initWithURLString:(NSString*) urlString error:(NSError**) err{
	NSURL* adresseRecherche= urlFromString(urlString);
	
	//On charge la page web dans html
	NSStringEncoding encoding;
	NSString *html = [NSString stringWithContentsOfURL:adresseRecherche usedEncoding:&encoding error:err];
	
	
	return [self initWithString:html encoding:encoding];
}
-(NSRange)rangeOfFirstOpeningBalise:(NSString*) balise{
	NSRange startRange;
	NSRange noArgRange = [a_string rangeOfString:[[@"<" stringByAppendingString:balise] stringByAppendingString:@">"]];
	NSRange argRange =[a_string rangeOfString:[[@"<" stringByAppendingString:balise] stringByAppendingString:@" "]];
	if (noArgRange.location<argRange.location) {
		startRange = noArgRange;
	}
	else{
		startRange = argRange;
	}
	
	NSRange endRange = [a_string rangeOfString:@">" options:NSLiteralSearch range:QNMakeRange(startRange.location, [a_string length])];
	return QNMakeRange(startRange.location, endRange.location+endRange.length);
}
-(NSUInteger)positionOfFirstOpeningBalise:(NSString*) balise{
	NSRange retour = [self rangeOfFirstOpeningBalise:balise];
	return retour.location+retour.length;
	
	
}
-(NSUInteger)positionOfFirstClosingBalise:(NSString*) balise{
	NSRange range = [a_string rangeOfString:[[@"</" stringByAppendingString:balise] stringByAppendingString:@">"]];
	return range.location;
}

-(QNParser*)initWithString:(NSString*) string encoding:(NSStringEncoding) encoding{
	self = [super init];
	if (self) {
		a_string = string;
		a_encoding = encoding;
	}
	return self;
}

-(NSString*)getContentOfFirstBalise:(NSString*) balise{
	NSRange subRange = QNMakeRange([self positionOfFirstOpeningBalise:balise], [self positionOfFirstClosingBalise:balise]);
	if (subRange.location == NSNotFound) {
		return nil;
	}
	return [a_string substringWithRange:subRange];
	
}
-(NSDictionary*)getArgFromFirstBalise:(NSString*) balise{
	NSRange range = [self rangeOfFirstOpeningBalise:balise];
	
	NSString* argString = [[a_string substringWithRange:range] stringByProtectingQuotationWithEncoding:a_encoding];
	
	//On enlève le > final
	argString = [argString substringToIndex:[argString length]-1];
	
	if ([argString length]>0) {
		NSMutableDictionary* retour = [NSMutableDictionary dictionary];
		NSArray* array = [argString componentsSeparatedByString:@" "];//On separe les composants par espaces
		//on enlève le premier élément qu'est la balise
		array = [array subarrayWithRange:QNMakeRange(1, [array count])];
		for (id object in array){
			if ([object length]>0) {
				NSArray* newArg = [object componentsSeparatedByString:@"="];
				
				NSString* value = [[[newArg subarrayWithRange:QNMakeRange(1, [newArg count])] componentsJoinedByString:@"="]  stringByRemovingQuotationAndProtectionWithEncoding:a_encoding];
				[retour setObject: value forKey:[newArg objectAtIndex:0]];
			}
		}
		return retour;
		
		
	}
	return nil;
}

@end
