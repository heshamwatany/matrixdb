//
//  MXStore.m
//  MatrixDB
//
//  Created by PH on 4.6.10.
//  Copyright 2010 Codesign LLC. All rights reserved.
//

#import "MXStore.h"

@implementation MXStore

- (id)initWithPath:(NSString*)path {
	return [self initWithPath: path autosync: NO];
}

- (id)initWithPath:(NSString*)path autosync:(BOOL)_autosync {
	if ((self = [super init])) {
		string p([path UTF8String]);
		store = new matrixdb::Store(p, _autosync);
	}
	return self;
}

- (BOOL)storeObject:(MXObject*)object {
	return store->save([object matrix]);
}

- (BOOL)removeObject:(MXObject*)object {
	return store->remove([object matrix]);
}

- (NSArray*)objectsOfClass:(Class)cls {
	return [self objectsOfClass: cls instantiate: YES];
}

- (void)instantiateMatrix:(MXObject*)matrix {
	matrixdb::Matrix* _matrix = [matrix matrix];
	store->instantiate(_matrix);
}

- (NSArray*)objectsOfClass:(Class)cls instantiate:(BOOL)instantiate {
	std::map<string, string> filter;
	filter["__class__"] = [NSStringFromClass(cls) UTF8String];
	std::vector<matrixdb::Matrix*>* vector = store->load(&filter, instantiate);
	NSMutableArray* objects = [NSMutableArray arrayWithCapacity: vector->size()];
	for(std::vector<matrixdb::Matrix*>::const_iterator it = vector->begin(); it != vector->end(); ++it) {
		matrixdb::Matrix* matrix = (matrixdb::Matrix*) *it;
		MXObject* object = [[cls alloc] initWithMatrix: matrix];
		object.store = self;
		[objects addObject: object];
		[object release];
	}
	delete vector;
	return objects;
}

- (void)indexField:(NSString*)fieldName {
	store->indexField([fieldName UTF8String]);
}

- (BOOL)performBlockInTransaction:(void(^)())block {
	[self beginTransaction];
	@try {
		block();
		[self commitTransaction];
		return YES;
	} @catch (NSException* exception) {
		[self rollbackTransaction];
	}
	return NO;
}

- (void)beginTransaction {
	store->beginTransaction();
}

- (void)commitTransaction {
	store->commit();
}

- (void)rollbackTransaction {
	store->rollback();
}

- (void)dealloc {
	delete store;
	[super dealloc];
}

- (void)finalize {
	delete store;
	[super finalize];
}

@end
