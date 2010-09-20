//
//  MXPerson.h
//  MatrixDB
//
//  Created by PH on 5.6.10.
//  Copyright 2010 Codesign LLC. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MXObject.h"

@interface MXPerson : MXObject {

}

@property (nonatomic, assign) NSString *firstName, *lastName;

- (id)initWithFirstName:(NSString*)_firstName lastName:(NSString*)_lastName;

@end
