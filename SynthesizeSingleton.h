//
//  SynthesizeSingleton.h
//  Modified by CJ Hanson on 26/02/2010.
//  This version of Matt's code uses method_setImplementaiton() to dynamically
//  replace the +sharedInstance method with one that does not use @synchronized
//
//  Based on code by Matt Gallagher from CocoaWithLove
//
//  Created by Matt Gallagher on 20/10/08.
//  Copyright 2009 Matt Gallagher. All rights reserved.
//
//  Permission is given to use this source code file without charge in any
//  project, commercial or otherwise, entirely at your risk, with the condition
//  that any redistribution (in part or whole) of source code must retain
//  this copyright and permission notice. Attribution in compiled projects is
//  appreciated but not required.
//

#import <objc/runtime.h>

//Set this to 1 to wrap init
#define SINGLETON_IS_SYNCHRONIZED 1

#define SYNTHESIZE_SINGLETON_FOR_CLASS_HEADER(__CLASSNAME__)	\
	\
+ (__CLASSNAME__ *)sharedInstance;	\
+ (void)purgeSharedInstance;


#define SYNTHESIZE_SINGLETON_FOR_CLASS(__CLASSNAME__)	\
	\
static volatile __CLASSNAME__ *_sharedInstance = nil;	\
	\
+ (__CLASSNAME__ *)sharedInstanceNoSynch	\
{	\
	return (__CLASSNAME__ *)_sharedInstance;	\
}	\
	\
+ (__CLASSNAME__ *)sharedInstance	\
{	\
	@synchronized(self){	\
		if(_sharedInstance == nil){	\
			_sharedInstance = [[self alloc] init];	\
			if(_sharedInstance){	\
				method_setImplementation(class_getClassMethod(self, @selector(sharedInstance)), method_getImplementation(class_getClassMethod(self, @selector(sharedInstanceNoSynch))));	\
				\
				method_setImplementation(class_getInstanceMethod(self, @selector(retainCount)), class_getMethodImplementation(self, @selector(retainCountDoNothing)));	\
				method_setImplementation(class_getInstanceMethod(self, @selector(release)), class_getMethodImplementation(self, @selector(releaseDoNothing)));	\
				method_setImplementation(class_getInstanceMethod(self, @selector(autorelease)), class_getMethodImplementation(self, @selector(autoreleaseDoNothing)));	\
			}	\
		}else{	\
			NSAssert2(1==0, @"SynthesizeSingleton: %@ ERROR: +(%@ *)sharedInstance method did not get swizzled!!!", self, self);	\
		}	\
	}	\
	return (__CLASSNAME__ *)_sharedInstance;	\
}	\
	\
+ (id)allocWithZone:(NSZone *)zone	\
{	\
	@synchronized(self){	\
		if (_sharedInstance == nil){	\
			_sharedInstance = [super allocWithZone:zone];	\
		}	\
	}	\
		\
	return _sharedInstance;	\
}	\
	\
+ (void)purgeSharedInstance	\
{	\
	@synchronized(self){	\
		method_setImplementation(class_getInstanceMethod([self class], @selector(retainCount)), class_getMethodImplementation([self class], @selector(retainCountDoSomething)));	\
		method_setImplementation(class_getInstanceMethod([self class], @selector(release)), class_getMethodImplementation([self class], @selector(releaseDoSomething)));	\
		method_setImplementation(class_getInstanceMethod([self class], @selector(autorelease)), class_getMethodImplementation([self class], @selector(autoreleaseDoSomething)));	\
		[_sharedInstance release];	\
		_sharedInstance = nil;	\
	}	\
}	\
	\
- (id)copyWithZone:(NSZone *)zone	\
{	\
	return self;	\
}	\
	\
- (id)retain	\
{	\
	return self;	\
}	\
	\
- (NSUInteger)retainCount	\
{	\
	NSAssert1(1==0, @"SynthesizeSingleton: %@ ERROR: -(NSUInteger)retainCount method did not get swizzled!!!", self);	\
	return NSUIntegerMax;	\
}	\
	\
- (NSUInteger)retainCountDoNothing	\
{	\
	return NSUIntegerMax;	\
}	\
- (NSUInteger)retainCountDoSomething	\
{	\
	return [super retainCount];	\
}	\
	\
- (void)release	\
{	\
	NSAssert1(1==0, @"SynthesizeSingleton: %@ ERROR: -(void)release method did not get swizzled!!!", self);	\
}	\
	\
- (void)releaseDoNothing{}	\
	\
- (void)releaseDoSomething	\
{	\
	@synchronized(self){	\
		[super release];	\
	}	\
}	\
	\
- (id)autorelease	\
{	\
	NSAssert1(1==0, @"SynthesizeSingleton: %@ ERROR: -(id)autorelease method did not get swizzled!!!", self);	\
	return self;	\
}	\
	\
- (id)autoreleaseDoNothing	\
{	\
	return self;	\
}	\
	\
- (id)autoreleaseDoSomething	\
{	\
	return [super autorelease];	\
}

