// pbamboo_context.h
// A bamboo context.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the GPL license.
//
// Future Interface  lihw81@gmail.com
//

#ifndef PBAMBOO_CONTEXT_H
#define PBAMBOO_CONTEXT_H

#include <PFoundation/pcontext.h>
#include <PFoundation/pcontextproperties.h>
#include <PFoundation/parcball.h>

class PBook;

class PBambooContext : public PContext
{
public:
    PBambooContext(const PContextProperties &properties);
    ~PBambooContext();
    
    virtual pbool onInitialized();
    virtual pbool onUpdate();
    virtual void onDestroy();
    
    virtual pbool onKeyboard(PEvent *event);

    virtual pbool onPanBegin(PEvent *event);
    virtual pbool onPan(PEvent *event);
    virtual pbool onPanEnd(PEvent *event);
    
    virtual pbool onPinchBegin(PEvent *event);
    virtual pbool onPinch(PEvent *event);
    virtual pbool onPinchEnd(PEvent *event);

    P_INLINE PBook *book() const { return m_book; }

private:
    PBook    *m_book;
    PArcball  m_arcball;
};

#endif // PBAMBOO_CONTEXT_H
