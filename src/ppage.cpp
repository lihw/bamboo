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
    
pchar *PPage::html()
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
