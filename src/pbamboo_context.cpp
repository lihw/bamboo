// pbamboo_context.h
// A bamboo context.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the GPL license.
//
// Future Interface  lihw81@gmail.com
//

#include "pbamboo_context.h"

#include <Bamboo/pbook.h>

#include <Paper3D/paper3d.h>

#include <PFoundation/pfoundation.h>


PBambooContext::PBambooContext(const PContextProperties &properties)
    : PContext(properties)
{
    m_book = P_NULL;
}

PBambooContext::~PBambooContext()
{
}

pbool PBambooContext::onInitialized()
{
    m_book = module<PBook>("book");
    if (!m_book->load(properties()->m_archiveName.c_str()))
    {
        return false;
    }
    m_book->setCurrentPageNumber(1);

    return true;
}

pbool PBambooContext::onUpdate()
{
    return true;
}

void PBambooContext::onDestroy()
{
}

pbool PBambooContext::onKeyboard(PEvent *event)
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

pbool PBambooContext::onTouch(PEvent *event)
{    
    /*
    if (event->getType() == P_EVENT__TOUCH_DOWN)
    {
        m_arcball.restart();

        m_scene->setRotating(true);
    }
    else if (event->getType() == P_EVENT__TOUCH_MOVE)
    {
        puint32 x = event->parameter(P_EVENTPARAMETER__TOUCH_X).toInt();
        puint32 y = event->parameter(P_EVENTPARAMETER__TOUCH_Y).toInt();
        const puint32 *r = rect();
        pfloat32 xx = (pfloat32)x / (pfloat32)(r[2] - 1) * 2.0f - 1.0f;
        pfloat32 yy = (pfloat32)(r[3] - 1 - y) / (pfloat32)(r[3] - 1) * 2.0f - 1.0f;
        m_arcball.updateMouse(xx, yy);

        m_scene->rotate(m_arcball.getRotationMatrix());
    }
    else if (event->getType() == P_EVENT__TOUCH_UP)
    {
        m_scene->setRotating(false);
    }
    */

    return true;
}



