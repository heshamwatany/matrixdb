//
//  MXStore.h
//  MatrixDB
//
//  Created by PH on 4.6.10.
//  Copyright 2010 Codesign LLC. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MXObject.h"
#import "store.h"

@interface MXStore : NSObject {
	matrixdb::Store* store;
}

- (id)initWithPath:(NSString*)path;
- (id)initWithPath:(NSString*)path autosync:(BOOL)_autosync;
- (BOOL)storeObject:(MXObject*)object;
- (BOOL)removeObject:(MXObject*)object;
- (NSArray*)objectsOfClass:(Class)cls;
- (NSArray*)objectsOfClass:(Class)cls instantiate:(BOOL)instantiate;
- (BOOL)performBlockInTransaction:(void(^)())block;
- (void)beginTransaction;
- (void)commitTransaction;
- (void)rollbackTransaction;
- (void)instantiateMatrix:(MXObject*)matrix;
- (void)indexField:(NSString*)fieldName;

@end
