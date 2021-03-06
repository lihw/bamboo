// bpage.cpp
// A page of the book.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the GPL license.
//
// Hongwei Li lihw81@gmail.com
//


#include "bpage.h"

#include <Bamboo/bpage.h>
#include <Bamboo/bbook.h>
#include <Bamboo/bcanvas.h>

#include <Paper3D/pscene.h>
#include <Paper3D/presourcemanager.h>
#include <Paper3D/prendertarget.h>

#include <PFoundation/pnew.h>
#include <PFoundation/pcontext.h>
#include <PFoundation/pinputstream.h>
#include <PFoundation/parchivefile.h>
#include <PFoundation/pxmlelement.h>
#include <PFoundation/pinterpolatedvalue.h>


BPage::BPage(BBook *book, puint32 pageNumber)
{
    m_book          = book;
    m_pageNumber    = pageNumber;
    m_html          = P_NULL;
    m_currentCanvas = P_NULL;
    m_state         = UNITIALIZED;
    m_visible       = true;

    pchar htmlFile[1024];
    psprintf(htmlFile, 1024, "page/page%04d.bmt", pageNumber);
    m_htmlFile = pstrdup(htmlFile);
}

BPage::~BPage()
{
    clear();
}

void BPage::update()
{
    if (m_state == UNITIALIZED)
    {
        return ;
    }
    
    for (puint32 i = 0; i < m_canvases.count(); ++i)
    {
        // We put the canvas loading here to avoid the multi-thread competition.
        // See comments in open().
        if (m_canvases[i] == P_NULL)
        {
            pchar name[1024];
            psprintf(name, 1024, "page/page%04d_%02d.bms", m_pageNumber, i + 1);
            m_canvases[i] = PNEW(BCanvas(name, m_book->context()));
            if (!m_canvases[i]->load(name))
            {
                continue;
            }
            switch (i)
            {
                case 0: m_canvases[i]->setBackgroundColor(P_COLOR_BLACK_TRANSPARENT); break;
                        //case 1: m_canvases[i]->setBackgroundColor(P_COLOR_GREEN); break;
                        //case 2: m_canvases[i]->setBackgroundColor(P_COLOR_BLUE); break;
            }
        }
            
        m_canvases[i]->update();
    }

    switch (m_state)
    {
        case TRANSITION_SCALEUP:
            {
                pfloat32 deltaTime = m_book->context()->clock().deltaTime();
                m_book->value()->update(deltaTime);    
                pfloat32 v = m_book->value()->value();

                if (v > (1.0f - 1e-4f))
                {
                    m_state = ZOOMIN;
                    m_currentCanvas->setViewport(P_NULL);
                }
                else
                {
                    const puint32 *r = m_book->context()->rect();

                    puint32 viewport[4];
                    viewport[0] = pLerp(m_originalViewport[0], r[0], v);
                    viewport[1] = pLerp(m_originalViewport[1], r[1], v);
                    viewport[2] = pLerp(m_originalViewport[2], r[2], v);
                    viewport[3] = pLerp(m_originalViewport[3], r[3], v);

                    m_currentCanvas->setViewport(viewport);
                }
            }

            break;
        case TRANSITION_SCALEDOWN:
            {
                PASSERT(m_currentCanvas != P_NULL);

                pfloat32 deltaTime = m_book->context()->clock().deltaTime();
                m_book->value()->update(deltaTime);    
                pfloat32 v = m_book->value()->value();

                if (v > (1.0f - 1e-4f))
                {
                    m_state = ZOOMOUT;
                    m_currentCanvas->setViewport(m_originalViewport);
                }
                else
                {
                    const puint32 *r = m_book->context()->rect();

                    puint32 viewport[4];
                    viewport[0] = pLerp(r[0], m_originalViewport[0], v);
                    viewport[1] = pLerp(r[1], m_originalViewport[1], v);
                    viewport[2] = pLerp(r[2], m_originalViewport[2], v);
                    viewport[3] = pLerp(r[3], m_originalViewport[3], v);

                    m_currentCanvas->setViewport(viewport);
                }
            }
            break;
    }
}
    
void BPage::render(PRenderState *renderState)
{
    if (m_visible && m_state != UNITIALIZED)
    {
        // Render each canvas.
        if (m_state == ZOOMIN)
        {
            m_currentCanvas->render(renderState);
        }
        else if (m_state == ZOOMOUT)
        {
            for (puint32 i = 0; i < m_canvases.count(); ++i)
            {
                m_canvases[i]->render(renderState);    
            }
        }
        else
        {
            for (puint32 i = 0; i < m_canvases.count(); ++i)
            {
                if (m_canvases[i] != m_currentCanvas)
                {
                    m_canvases[i]->render(renderState);    
                }
            }
            m_currentCanvas->render(renderState);
        }
    }
}

void BPage::setVisiblity(pbool flag)
{
    m_visible = flag;
}

void BPage::setNumberOfCanvases(puint32 number)
{
    for (puint32 i = 0; i < m_canvases.count(); ++i)
    {
        PDELETE(m_canvases[i]);
    }
    m_canvases.resize(number);
    for (puint32 i = 0; i < m_canvases.count(); ++i)
    {
        m_canvases[i] = P_NULL;
    }
}
    
void BPage::load()
{
    if (m_state == UNITIALIZED)
    {
        // Load HTML text.
        PResourceManager *resourceManager = m_book->context()->module<PResourceManager>("resource-manager");
        PArchiveFile *archive = resourceManager->archive();

        PInputStream inputStream;
        if (!archive->createInputStream(m_htmlFile, &inputStream))
        {
            PLOG_ERROR("Failed to read scene configuration file at %s.", m_htmlFile);
            return ;
        }

        puint8 *htmlText;
        inputStream.readAllBytesAsString(htmlText);
        m_html = (pchar *)htmlText;
    
        // We deferred the asset loading to update() to reduce the execution time
        // of this function as this function is in the webview thread and should 
        // be light-weighted. 
        // FIXME: m_state will be read in main thread while written in webview thread.
        // Better to add lock for mutual exclusion.
        m_state = ZOOMOUT;
    }
}

void BPage::unload()
{
    if (m_state != UNITIALIZED)
    {
        m_state = UNITIALIZED;
    
        PDELETEARRAY(m_html);

        for (puint32 i = 0; i < m_canvases.count(); ++i)
        {
            PDELETE(m_canvases[i]);
        }
    }
}

void BPage::clear()
{
    PDELETEARRAY(m_html);
    PDELETEARRAY(m_htmlFile);

    for (puint32 i = 0; i < m_canvases.count(); ++i)
    {
        PDELETE(m_canvases[i]);
    }

    m_state = ZOOMOUT;
}
    
void BPage::onPanBegin(pint32 x, pint32 y)
{
    // Check if the touch fall into one of scenes.
    if (m_visible)
    {
        m_currentCanvas = locateCanvas(x, y);
        if (m_currentCanvas != P_NULL)
        {
            m_currentCanvas->useHand(true);

            if (!m_currentCanvas->isFirstPersonView())
            {
                m_book->arcball()->restart();
                PLOG_INFO("\n\n");
            }
        }
    }
}

void BPage::onPan(pint32 x, pint32 y, pint32 dx, pint32 dy)
{
    if (m_visible && m_currentCanvas != P_NULL)
    {
        if (m_currentCanvas->isFirstPersonView())
        {
            const puint32 *viewport = m_currentCanvas->viewport();
            pfloat32 xx = (pfloat32)dx / (pfloat32)viewport[2];
            pfloat32 yy = (pfloat32)dy / (pfloat32)viewport[3];

            m_currentCanvas->rotate(xx, yy);
        }
        else
        {
            y = m_book->context()->rect()[3] - 1 - y;

            const puint32 *viewport = m_currentCanvas->viewport();
            pfloat32 xx = (pfloat32)(x - viewport[0]) / (pfloat32)(viewport[2] - 1) * 2.0f - 1.0f;
            pfloat32 yy = (pfloat32)(y - viewport[1]) / (pfloat32)(viewport[3] - 1) * 2.0f - 1.0f;

            m_book->arcball()->updateMouse(xx, yy);
            m_currentCanvas->rotate(m_book->arcball()->deltaRotation());
        }
    }
}

void BPage::onPanEnd()
{
    if (m_visible && m_currentCanvas != P_NULL)
    {
        m_currentCanvas->useHand(false);
    }
}
    
void BPage::onPinchBegin(const pint32 *pt1, const pint32 *pt2)
{
    if (m_visible)
    {
        /*
        y = m_book->context()->rect()[3] - 1 - y;

        PASSERT(m_currentCanvas == P_NULL);

        for (puint32 i = 0; i < m_canvases.count(); ++i)
        {
            const puint32 *viewport = m_canvases[i]->viewport();
            if (x > viewport[0] && 
                y > viewport[1] &&
                x < viewport[0] + viewport[2] &&
                y < viewport[1] + viewport[3])
            {
                m_currentCanvas = m_canvases[i];
                //m_book->arcball()->restart();
                m_currentCanvas->useHand(true);
                return true;
            }
        }
        */
    }
}

void BPage::onPinch(const pint32 *pt1, const pint32 *pt2, pfloat32 angle, pfloat32 scaling)
{
    if (m_visible)
    {
        PLOG_INFO("pinch scaling: %f", scaling);
    }
}

void BPage::onPinchEnd()
{
}
    
void BPage::onLongPress(pint32 x, pint32 y)
{
    if (m_visible)
    {
        if (m_state == ZOOMIN || m_state == ZOOMOUT)
        {
            m_currentCanvas = locateCanvas(x, y);
            if (m_currentCanvas != P_NULL && m_currentCanvas->isScalingEnabled())
            {
                if (m_state == ZOOMOUT)
                {
                    const puint32 *viewport = m_currentCanvas->viewport();
                    m_originalViewport[0] = viewport[0];
                    m_originalViewport[1] = viewport[1];
                    m_originalViewport[2] = viewport[2];
                    m_originalViewport[3] = viewport[3];
                }

                m_state = (m_state + 1) % NUM_STATES;

                m_book->value()->setValue(0, true);
            }
        }
    }
}
    
BCanvas *BPage::locateCanvas(pint32 x, pint32 y)
{
    if (m_state == ZOOMOUT)
    {
        y = m_book->context()->rect()[3] - 1 - y;

        for (puint32 i = 0; i < m_canvases.count(); ++i)
        {
            const puint32 *viewport = m_canvases[i]->viewport();
            if ((puint32)x > viewport[0] && 
                (puint32)y > viewport[1] &&
                (puint32)x < viewport[0] + viewport[2] &&
                (puint32)y < viewport[1] + viewport[3])
            {
                return m_canvases[i];
            }
        }
    
        return P_NULL;
    }

    return m_currentCanvas;
}

pbool BPage::isPointInside(pint32 x, pint32 y)
{
    return locateCanvas(x, y) != P_NULL;
}
