//
//  MXNativeQuery.m
//  MatrixDB
//
//  Created by PH on 7.6.10.
//  Copyright 2010 Codesign LLC. All rights reserved.
//

#import "MXNativeQuery.h"

@implementation MXNativeQuery

- (id)initWithClass:(NSString*)_cls block:(BOOL(^)(id))_block expression:(NSString*)_expression varNamesList:(NSString*)varNamesList, ... {
	if ((self = [super init])) {
		cls = NSClassFromString(_cls);
		block = [_block copy];
		expression = [_expression retain];
		NSArray* vars = [varNamesList componentsSeparatedByString: @","];
		va_list(args);
		va_start(args, varNamesList);
		localVars = [[NSMutableDictionary alloc] init];
		for (NSString* var in vars) {
			var = [var stringByTrimmingCharactersInSet: [NSCharacterSet whitespaceCharacterSet]];
			NSString* arg = va_arg(args, NSString*);
			[localVars setObject: arg forKey: var];
		}
		va_end(args);
	}
	return self;
}

- (NSString*)description {
	return [NSString stringWithFormat: @"%@ %@ %@", NSStringFromClass(cls), expression, localVars];
}

- (void)dealloc {
	[block release];
	[expression release];
	[localVars release];
	[super dealloc];
}

@end
