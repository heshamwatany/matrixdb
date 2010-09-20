//
//  MXObject.h
//  MatrixDB
//
//  Created by PH on 4.6.10.
//  Copyright 2010 Codesign LLC. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "matrix.h"

@class MXStore;

@interface MXObject : NSObject {
	matrixdb::Matrix* matrix;
	MXStore* store;
}

@property (nonatomic, assign) MXStore* store;

- (id)initWithMatrix:(matrixdb::Matrix*)_matrix;
+ (id)matrixWithMatrix:(matrixdb::Matrix*)_matrix;
- (void)setValue:(id)value forKey:(NSString*)key;
- (id)valueForKey:(NSString*)key;
- (matrixdb::Matrix*)matrix;
- (BOOL)isInstantiated;

@end
