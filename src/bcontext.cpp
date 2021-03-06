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

    //m_book->openPage(1);
    //m_book->currentPage()->load();
    
    gestureManager()->setGestureEnabled(P_GESTURE_TYPE_PAN, true);
    gestureManager()->setGestureEnabled(P_GESTURE_TYPE_PINCH, true);
    gestureManager()->setGestureEnabled(P_GESTURE_TYPE_LONGPRESS, true);

    return true;
}

pbool BContext::onUpdate()
{
    return true;
}

void BContext::onDestroy()
{
}

void BContext::onKeyboard(puint32 state, puint32 key, puint32 scancode)
{
    if ((state & P_KEY_DEVICE_STATE_UP))
    {
        switch (scancode)
        {
            case P_KEY_ESC:
                quit();
                break;
        }
    }
}


void BContext::onPinchBegin(const pint32 *point1, const pint32 *point2)
{
    BPage *page = m_book->currentPage();
    if (page != P_NULL)
    {
        return page->onPinchBegin(point1, point2);
    }
}
void BContext::onPinch(const pint32 *point1, const pint32 *point2, pfloat32 angle, pfloat32 scaling)
{
    BPage *page = m_book->currentPage();
    if (page != P_NULL)
    {
        return page->onPinch(point1, point2, angle, scaling);
    }
}

void BContext::onPinchEnd()
{
    BPage *page =m_book->currentPage();
    if (page != P_NULL)
    {
        return page->onPinchEnd();
    }
}

void BContext::onLongPress(pint32 x, pint32 y)
{
    BPage *page = m_book->currentPage();
    if (page != P_NULL)
    {
        return page->onLongPress(x, y);
    }
}

void BContext::onPanBegin(pint32 x, pint32 y)
{
    BPage *page = m_book->currentPage();
    if (page != P_NULL)
    {
        return page->onPanBegin(x, y);
    }
}

void BContext::onPan(pint32 x, pint32 y, pint32 dx, pint32 dy)
{
    BPage *page = m_book->currentPage();
    if (page != P_NULL)
    {
        return page->onPan(x, y, dx, dy);
    }
}

void BContext::onPanEnd()
{
    BPage *page = m_book->currentPage();
    if (page != P_NULL)
    {
        return page->onPanEnd();
    }
}

pbool BContext::pointInside(pfloat32 x, pfloat32 y)
{
    // TODO: a ugly hack here.
    // A better solution is to let webview intercept the touch events
    // and propogate to opengl view after recognizing the gestures.
    BPage *page = m_book->currentPage();
    if (page != P_NULL)
    {
        return page->isPointInside((pint32)x, (pint32)y);
    }
    
    return false;
}
