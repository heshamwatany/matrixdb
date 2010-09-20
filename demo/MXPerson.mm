//
//  MXPerson.m
//  MatrixDB
//
//  Created by PH on 5.6.10.
//  Copyright 2010 Codesign LLC. All rights reserved.
//

#import "MXPerson.h"

@implementation MXPerson

@dynamic firstName, lastName;

- (id)initWithFirstName:(NSString*)_firstName lastName:(NSString*)_lastName {
	if ((self = [super init])) {
		self.firstName = _firstName;
		self.lastName = _lastName;
	}
	return self;
}

- (NSString*)description {
	return [NSString stringWithFormat: @"%@ %@", self.firstName, self.lastName];
}

@end
