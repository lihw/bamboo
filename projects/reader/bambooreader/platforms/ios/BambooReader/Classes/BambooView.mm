// BambooView.mm
// The BambooReader OpenGL native view.
//
// Copyright 2012 - 2014 Future Interface.
// All rights reserved.
//
// Hongwei Li lihw81@gmail.com
//

#import "BambooView.h"

#include <Bamboo/bcontext.h>

#include <PFoundation/pcontext.h>


@implementation BambooView

- (BOOL)pointInside:(CGPoint)point withEvent:(UIEvent *)event
{
    // Return YES when the point is inside any canvas of the
    // the current page. The hit test only happens at the touch-began
    // phase; See http://smnh.me/hit-testing-in-ios/
    
    BContext *bcontext = (BContext *)_pcontext;
    if (bcontext->pointInside(point.x, point.y))
    {
        return YES;
    }
    
    return NO;
}

@end