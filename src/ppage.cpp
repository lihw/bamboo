// ppage.cpp
// A page of the book.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the GPL license.
//
// Hongwei Li lihw81@gmail.com
//


#include "ppage.h"

#include <Bamboo/pcanvas.h>

#include <PFoundation/pnew.h>


PPage::PPage(PBook *book, puint32 pageNo)
{
    m_book   = book;
    m_pageNo = pageNo;
    m_html   = P_NULL;
}

PPage::~PPage()
{
    clear();
}

void PPage::update()
{
    if (!m_scenesFile.isEmpty() && m_scenes.isEmpty())
    {
        for (puint32 i = 0; i < m_sceneFiles.count(); ++i)
        {
            pchar name[1024];
            psprintf(name, 1024, "scene%03d", i);
            PScene *scene = PNEW(PScene(name, m_book->context()));
            m_scenes.append(scene);

            if (!scene->load(m_sceneFiles[i]))
            {
                continue;
            }
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
        if (!archive->createInputStream(path, &inputStream))
        {
            PLOG_ERROR("Failed to read scene configuration file at %s", path);
            return false;
        }
        
        m_html = inputStream.readEntireFile();
    }

    return m_html;
}

void PPage::render(PRenderState *renderState)
{
    if (m_visible)
    {
        for (puint32 i = 0; i < m_scenes.count(); ++i)
        {
            m_scenes[i]->update(renderState);
        }
    }
}

void PPage::setVisiblity(pbool flag)
{
    m_visible = flag;
}

pbool PPage::unpack(const PXmlElement *xmlElement)
{
    // Sanity check.
    if (pstrcmp(xmlElement->name(), "page") != 0)
    {
        PLOG_ERROR("It is not a page element");
        return false;
    }
    
    // Destroy the old content.
    for (puint32 i = 0; i < m_scenes.count(); ++i)
    {
        PDELETE(m_scenes[i]);
    }
    for (puint32 i = 0; i < m_sceneFiles.count(); ++i)
    {
        PDELETEARRAY(m_sceneFiles[i]);
    }

    PXmlElement childElement = xmlElement->firstChild();
    while (childElement.isValid())
    {
        if (pstrcmp(childElement.name(), "html") == 0)
        {
            const pchar *htmlFileValue = childElement.attribute("file");
            PASSERT(htmlFileValue != P_NULL);
            m_htmlFile = pstrdup(htmlFile);
        }
        else if (pstrcmp(childElement.name(), "scene") == 0)
        {
            const pchar *sceneFileValue = childElement.attribute("file");
            PASSERT(sceneFileValue != P_NULL);
            m_sceneFiles.append(pstrdup(sceneFileValue));
        }
        else
        {
            PLOG_WARNING("Unknown element node (%s) in page.", childElement.name());
        }
    }

    m_visible = false;

    return true;
}
    
void PPage::clear()
{
    PDELETEARRAY(m_html);
    PDELETEARRAY(m_htmlFile);

    for (puint32 i = 0; i < m_scenes.count(); ++i)
    {
        PDELETE(m_canvas);
    }
    for (puint32 i = 0; i < m_sceneFiles.count(); ++i)
    {
        PDELETEARRAY(m_sceneFiles[i]);
    }
}
