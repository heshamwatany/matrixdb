//
//  MXNativeQuery.h
//  MatrixDB
//
//  Created by PH on 7.6.10.
//  Copyright 2010 Codesign LLC. All rights reserved.
//

#import <Foundation/Foundation.h>

#define MXQUERY(cls, expr, ...) [[[MXNativeQuery alloc] initWithClass: [NSString stringWithUTF8String: #cls]\
																block: ^BOOL(cls* obj) { return expr; }\
														   expression: [NSString stringWithUTF8String: #expr]\
														 varNamesList: [NSString stringWithUTF8String: #__VA_ARGS__], __VA_ARGS__] autorelease]

@interface MXNativeQuery : NSObject {
	Class cls;
	BOOL (^block)(id);
	NSString* expression;
	NSMutableDictionary* localVars;
}

- (id)initWithClass:(NSString*)_cls block:(BOOL(^)(id))_block expression:(NSString*)_expression varNamesList:(NSString*)varNamesList, ...;

@end
