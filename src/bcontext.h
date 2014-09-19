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

private:
    BBook    *m_book;
};

#endif // BCONTEXT_H
