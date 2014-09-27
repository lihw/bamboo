// bcontext.h
// A bamboo context.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the GPL license.
//
// Future Interface  lihw81@gmail.com
//

#ifndef BCONTEXT_H
#define BCONTEXT_H

#include <PFoundation/pcontext.h>
#include <PFoundation/pcontextproperties.h>
#include <PFoundation/parcball.h>

class BBook;

class BContext : public PContext
{
public:
    BContext(const PContextProperties &properties);
    ~BContext();
    
    virtual pbool onInitialized();
    virtual pbool onUpdate();
    virtual void onDestroy();
    
    virtual pbool onKeyboard(PEvent *event);

    P_INLINE BBook *book() const { return m_book; }
    
    virtual void onPanBegin(pint32 x, pint32 y);
    virtual void onPan(pint32 x, pint32 y, pint32 dx, pint32 dy);
    virtual void onPanEnd(pint32 x, pint32 y, pint32 dx, pint32 dy);
    virtual void onLongPress(pint32 x, pint32 y);
    virtual void onPinchBegin(const pint32 *point1, const pint32 *point2);
    virtual void onPinch(const pint32 *point1, const pint32 *point2, pfloat32 angle, pfloat32 scaling);
    virtual void onPinchEnd();

private:
    BBook    *m_book;
};

#endif // BCONTEXT_H
