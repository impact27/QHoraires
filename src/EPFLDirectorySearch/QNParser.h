//
//  QNParser.h
//  GetInfoEPFL
//
//  Created by Quentin Peter on 07.05.14.
//  Copyright (c) 2014 Quentin. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface QNParser : NSObject
-(NSStringEncoding) encoding;
-(NSString*) string;
-(NSString*) firstEmail;
-(QNParser*)initWithURLString:(NSString*) urlString error:(NSError**) err;
-(QNParser*)initWithString:(NSString*) string encoding:(NSStringEncoding) encoding;
-(NSRange)rangeOfFirstOpeningBalise:(NSString*) balise;
-(NSUInteger)positionOfFirstOpeningBalise:(NSString*) balise;
-(NSUInteger)positionOfFirstClosingBalise:(NSString*) balise;
-(NSString*)getContentOfFirstBalise:(NSString*) balise;
-(NSDictionary*)getArgFromFirstBalise:(NSString*) balise;
@end


//NSRange QNMakeRange(NSUInteger from, NSUInteger to);