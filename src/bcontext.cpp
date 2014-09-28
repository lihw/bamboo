// bcontext.h
// A bamboo context.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the GPL license.
//
// Future Interface  lihw81@gmail.com
//

#include "bcontext.h"

#include <Bamboo/bbook.h>
#include <Bamboo/bpage.h>

#include <Paper3D/paper3d.h>

#include <PFoundation/pfoundation.h>


class LongPressHandler : public PGestureLongPressHandler
{
public: 
    LongPressHandler(void *data)
        : PGestureLongPressHandler(data)
    {
    }

    virtual void onLongPress(pint32 x, pint32 y)
    {
        BPage *page = reinterpret_cast<BBook *>(m_data)->currentPage();
        if (page != P_NULL)
        {
            return page->onLongPress(x, y);
        }
    }
};

class PanHandler : public PGesturePanHandler
{
public:
    PanHandler(void *data)
        : PGesturePanHandler(data)
    {
    }
    virtual void onPanBegin(pint32 x, pint32 y)
    {
        BPage *page = reinterpret_cast<BBook *>(m_data)->currentPage();
        if (page != P_NULL)
        {
            return page->onPanBegin(x, y);
        }
    }
    virtual void onPan(pint32 x, pint32 y, pint32 dx, pint32 dy)
    {
        BPage *page = reinterpret_cast<BBook *>(m_data)->currentPage();
        if (page != P_NULL)
        {
            return page->onPan(x, y, dx, dy);
        }
    }
    virtual void onPanEnd(pint32 x, pint32 y, pint32 dx, pint32 dy)
    {
        BPage *page = reinterpret_cast<BBook *>(m_data)->currentPage();
        if (page != P_NULL)
        {
            return page->onPanEnd(x, y, dx, dy);
        }
    }
};

class PinchHandler : public PGesturePinchHandler
{
public:
    PinchHandler(void *data)
        : PGesturePinchHandler(data)
    {
    }
    virtual void onPinchBegin(const pint32 *point1, 
                              const pint32 *point2)
    {
        BPage *page = reinterpret_cast<BBook *>(m_data)->currentPage();
        if (page != P_NULL)
        {
            return page->onPinchBegin(point1, point2);
        }
    }
    virtual void onPinch(const pint32 *point1, 
                         const pint32 *point2,
                         pfloat32 angle,
                         pfloat32 scaling)
    {
        BPage *page = reinterpret_cast<BBook *>(m_data)->currentPage();
        if (page != P_NULL)
        {
            return page->onPinch(point1, point2, angle, scaling);
        }
    }
    virtual void onPinchEnd()
    {
        BPage *page = reinterpret_cast<BBook *>(m_data)->currentPage();
        if (page != P_NULL)
        {
            return page->onPinchEnd();
        }
    }
};

BContext::BContext(const PContextProperties &properties)
    : PContext(properties)
{
    m_book = P_NULL;
}

BContext::~BContext()
{
}

pbool BContext::onInitialized()
{
    m_book = module<BBook>("book");
    if (!m_book->load(properties()->m_archiveName.c_str()))
    {
        return false;
    }
    m_book->openPage(3);
    
    gestureManager()->addHandler(PNEW(LongPressHandler(m_book)));
    gestureManager()->addHandler(PNEW(PinchHandler(m_book)));
    gestureManager()->addHandler(PNEW(PanHandler(m_book)));

    return true;
}

pbool BContext::onUpdate()
{
    return true;
}

void BContext::onDestroy()
{
}

pbool BContext::onKeyboard(PEvent *event)
{
    pint32 key = event->parameter(P_EVENTPARAMETER__KEY_SCANCODE).toInt();
    pint32 type = event->getType();
    if (type == P_EVENT__KEYUP)
    {
        switch (key)
        {
            case P_KEY_ESC:
                quit();
                return true; 
        }
    }
    return true;
}

