//
//  MXObject.m
//  MatrixDB
//
//  Created by PH on 4.6.10.
//  Copyright 2010 Codesign LLC. All rights reserved.
//

#import "MXObject.h"
#import "MXStore.h"

@implementation MXObject

@synthesize store;

- (id)initWithMatrix:(matrixdb::Matrix*)_matrix {
	if ((self = [super init])) {
		matrix = _matrix;
	}
	return self;
}

- (id)init {
	if ((self = [super init])) {
		matrix = new matrixdb::Matrix();
		matrix->set("__class__", string([NSStringFromClass([self class]) UTF8String]));
		matrix->__instantiated = true;
	}
	return self;
}

- (NSMethodSignature*)methodSignatureForSelector:(SEL)selector {
	NSMethodSignature* sig = [super methodSignatureForSelector: selector];
	if (sig == nil) {
		NSString* s = NSStringFromSelector(selector);
		if ([s rangeOfString: @":"].location == NSNotFound)
			return [super methodSignatureForSelector: @selector(dummy)];
		else return [super methodSignatureForSelector: @selector(setDummy:)];
	}
	return sig;
}

- (void)forwardInvocation:(NSInvocation*)invocation {
	NSString* s = NSStringFromSelector([invocation selector]);
	if (matrix->__instantiated == false) {
		[store instantiateMatrix: self];
	}
	if ([[invocation methodSignature] numberOfArguments] == 2) {
		id key = [NSString stringWithFormat: @"%@%@", [[s substringToIndex: 1] uppercaseString], [s substringFromIndex: 1]];
		//NSLog(@"## %@", key);
		id value = [self valueForKey: key];
		[invocation setReturnValue: &value];
	} else {
		id value, key = [s substringWithRange: NSMakeRange(3, [s length] - 4)];
		//NSLog(@"# %@", key);
		//id oldValue = [self valueForKey: key];
		[invocation getArgument: &value atIndex: 2];
		//if (value != oldValue) {
		//	[oldValue release];
			[self setValue: value forKey: key];
		//}
	}
}

- (id)dummy { return nil; }

- (void)setDummy:(id)value { }

- (BOOL)isInstantiated {
	return matrix->__instantiated;
}

+ (id)matrixWithMatrix:(matrixdb::Matrix*)_matrix {
	Class cls = NSClassFromString([NSString stringWithUTF8String: _matrix->getString("__class__")->c_str()]);
	return [[[cls alloc] initWithMatrix: _matrix] autorelease];
}

- (void)setValue:(id)value forKey:(NSString*)key {
	if ([value isKindOfClass: [NSString class]]) {
		matrix->set([key UTF8String], string([value UTF8String]));
	}
}

- (id)valueForKey:(NSString*)key {
	string* s = matrix->getString([key UTF8String]);
	return s != NULL ? [NSString stringWithUTF8String: s->c_str()] : nil;
}

- (matrixdb::Matrix*)matrix {
	return matrix;
}

- (void)dealloc {
	delete matrix;
	[super dealloc];
}

- (void)finalize {
	delete matrix;
	[super finalize];
}

@end
