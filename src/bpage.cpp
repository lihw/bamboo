// ppage.cpp
// A page of the book.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the GPL license.
//
// Hongwei Li lihw81@gmail.com
//


#include "ppage.h"

#include <Bamboo/ppage.h>
#include <Bamboo/pbook.h>

#include <Paper3D/pscene.h>
#include <Paper3D/presourcemanager.h>

#include <PFoundation/pnew.h>
#include <PFoundation/pcontext.h>
#include <PFoundation/pinputstream.h>
#include <PFoundation/parchivefile.h>
#include <PFoundation/pxmlelement.h>


PPage::PPage(PBook *book, puint32 pageNumber)
{
    m_book         = book;
    m_pageNumber   = pageNumber;
    m_html         = P_NULL;

    pchar htmlFile[1024];
    psprintf(htmlFile, 1024, "page/page%04d.bmh", pageNumber);
    m_htmlFile     = pstrdup(htmlFile);
}

PPage::~PPage()
{
    clear();
}

void PPage::update()
{
    for (puint32 i = 0; i < m_scenes.count(); ++i)
    {
        if (m_scenes[i] == P_NULL)
        {
            pchar name[1024];
            psprintf(name, 1024, "page/page%04d_%02d.bms", m_pageNumber, i + 1);
            m_scenes[i] = PNEW(PScene(name, m_book->context()));
            if (!m_scenes[i]->load(name))
            {
                continue;
            }
        }
        else
        {
            m_scenes[i]->update();
        }
    }
}
    
const pchar *PPage::html()
{
    if (m_html == P_NULL && m_htmlFile != P_NULL)
    {
        PResourceManager *resourceManager = m_book->context()->module<PResourceManager>("resource-manager");

        PArchiveFile *archive = resourceManager->archive();
        PInputStream inputStream;
        if (!archive->createInputStream(m_htmlFile, &inputStream))
        {
            PLOG_ERROR("Failed to read scene configuration file at %s.", m_htmlFile);
            return false;
        }
        
        puint8 *htmlText = (puint8 *)m_html;
        inputStream.readAllBytesAsString(htmlText);
    }

    return m_html;
}

void PPage::render(PRenderState *renderState)
{
    if (m_visible)
    {
        for (puint32 i = 0; i < m_scenes.count(); ++i)
        {
            m_scenes[i]->render(renderState);
        }
    }
}

void PPage::setVisiblity(pbool flag)
{
    m_visible = flag;
}

void PPage::setNumberOfScenes(puint32 number)
{
    for (puint32 i = 0; i < m_scenes.count(); ++i)
    {
        PDELETE(m_scenes[i]);
    }
    m_scenes.resize(number);
    for (puint32 i = 0; i < m_scenes.count(); ++i)
    {
        m_scenes[i] = P_NULL;
    }
}

void PPage::clear()
{
    PDELETEARRAY(m_html);
    PDELETEARRAY(m_htmlFile);

    for (puint32 i = 0; i < m_scenes.count(); ++i)
    {
        PDELETE(m_scenes[i]);
    }
}
    
pbool PPage::onPanBegin(PEvent *event)
{
    // Check if the touch fall into one of scenes.
    if (m_visible)
    {
        puint32 x = event->parameter(P_EVENTPARAMETER__TOUCH_X).toInt();
        puint32 y = event->parameter(P_EVENTPARAMETER__TOUCH_Y).toInt();

        PASSERT(m_currentScene == P_NULL);

        for (puint32 i = 0; i < m_scenes.count(); ++i)
        {
            const puint32 *viewport = m_scenes[i]->viewport();
            if (x > viewport[0] && 
                y > viewport[1] &&
                x < viewport[0] + viewport[2] &&
                y < viewport[1] + viewport[3])
            {
                m_currentScene = m_scenes[i];
                m_book->arcball()->restart();
                m_scenes[i]->useHand(true);
                break;
            }
        }
    }
}

pbool PPage::onPan(PEvent *event)
{
    if (m_visible && m_currentScene != P_NULL)
    {
        puint32 x = event->parameter(P_EVENTPARAMETER__TOUCH_X).toInt();
        puint32 y = event->parameter(P_EVENTPARAMETER__TOUCH_Y).toInt();

        const puint32 *viewport = m_currentScene->viewport();
        pfloat32 xx = (pfloat32)x / (pfloat32)(viewport[2] - 1) * 2.0f - 1.0f;
        pfloat32 yy = (pfloat32)(viewport[3] - 1 - y) / (pfloat32)(viewport[3] - 1) * 2.0f - 1.0f;
        
        m_book->arcball()->updateMouse(xx, yy);

        m_currentScene->rotate(m_book->arcball()->rotationMatrix())
    }
}

pbool PPage::onPanEnd(PEvent *event)
{
    if (m_visibile && m_currentScene != P_NULL)
    {
        m_currentScene->useHand(false);
        m_currentScene = P_NULL;
    }
}
    
pbool PPage::onPinchBegin(PEvent *event)
{
}

pbool PPage::onPinch(PEvent *event)
{
}

pbool PPage::onPinchEnd(PEvent *event)
{
}
