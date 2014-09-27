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
    m_book->setCurrentPageNumber(3);
    
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

void BContext::onPanEnd(pint32 x, pint32 y, pint32 dx, pint32 dy)
{
    BPage *page = m_book->currentPage();
    if (page != P_NULL)
    {
        return page->onPanEnd(x, y, dx, dy);
    }
}
