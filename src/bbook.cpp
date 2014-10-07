// bbook.cpp
// A book
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the GPL license.
//
// Hongwei Li lihw81@gmail.com
//

#include "bbook.h"

#include <Bamboo/bcontent.h>
#include <Bamboo/bpage.h>

#include <Paper3D/presourcemanager.h>
#include <Paper3D/prenderstate.h>
#include <Paper3D/pscene.h>
#include <Paper3D/prenderpass.h>
#include <Paper3D/prendertarget.h>

#include <PFoundation/pcontext.h>
#include <PFoundation/parchivefile.h>
#include <PFoundation/pxmlelement.h>
#include <PFoundation/pxmldocument.h>
#include <PFoundation/pnew.h>
#include <PFoundation/plog.h>
#include <PFoundation/pconststring.h>


BBook::BBook(PContext *context)
    : PModule("book", context)
	, m_value(0, 1, 9)
{
    m_content           = P_NULL;
    m_currentPageNumber = 0xffffffff;
    m_renderState       = P_NULL;
    m_title             = P_NULL;
    m_clearRenderPass   = P_NULL;
    
    PScene::s_effectFactory.initialize();
    PScene::s_nodeFactory.initialize();
}

BBook::~BBook()
{
    clear();

    PDELETE(m_renderState);
    PDELETE(m_clearRenderPass);

    PScene::s_effectFactory.uninitialize();
    PScene::s_nodeFactory.uninitialize();
}
    
void BBook::openPage(puint32 pageNumber)
{
    m_currentPageNumber = pageNumber;
}

void BBook::closePage()
{
    m_currentPageNumber = 0xffffffff;
}

pbool BBook::load(const pchar *bookArchive)
{
    // Reload the book archive.
    PResourceManager *resourceManager = context()->module<PResourceManager>("resource-manager");
    PArchiveFile *archive = resourceManager->archive();
    if (!archive->load(bookArchive))
    {
        return false;
    }

    PInputStream inputStream;
    if (!archive->createInputStream("book.bmb", &inputStream))
    {
        PLOG_ERROR("Failed to read scene configuration file at book.bmb.");
        return false;
    }

    // Destroy the current content.
    clear();

    // Read the book information, title and page number.
    PXmlDocument xmlDocument;
    if (!xmlDocument.parse(inputStream))
    {
        PLOG_ERROR("The syntax is wrong in %s.", bookArchive);
        return false;
    }
        
    PXmlElement firstNode = xmlDocument.firstChildElement();

    if (pstrcmp(firstNode.name(), "book"))
    {
        PLOG_ERROR("The first child node of a book should be <book>.");
        return false;
    }

    const pchar *titleValue = firstNode.attribute("title");
    if (titleValue == P_NULL)
    {
        PLOG_ERROR("The title of the book is missing.");
        return false;
    }
    m_title = pstrdup(titleValue);

    const pchar *pageNumberValue = firstNode.attribute("pagenumber");
    if (pageNumberValue == P_NULL)
    {
        PLOG_ERROR("The book node doesn't have a number of pages.");
        return false;
    }
    pint32 pageNumber = 0;
    if (pStringUnpackInt(pageNumberValue, &pageNumber) == P_NULL)
    {
        PLOG_ERROR("The book node doesn't have an invalid page number.");
        return false;
    }
    m_pages.resize(pageNumber);
    for (pint32 i = 0; i < pageNumber; ++i)
    {
        m_pages[i] = PNEW(BPage(this, i + 1));
    }

    PXmlElement childElement = firstNode.firstChild();

    // Read the content of the book and each scene configuration per page.
    while (childElement.isValid())
    { 
        if (pstrcmp(childElement.name(), "content") == 0)
        {
            m_content = PNEW(BContent(this));
            if (!m_content->unpack(&childElement))
            {
                clear();
                return false;
            }
        }
        else if (pstrcmp(childElement.name(), "pages") == 0)
        {
            PXmlElement pageElement = childElement.firstChild();
            while (pageElement.isValid())
            {
                const pchar *numberValue = pageElement.attribute("no");
                if (numberValue == P_NULL)
                {
                    PLOG_ERROR("The page node doesn't have a page number.");
                    clear();
                    return false;
                }
                pint32 pageNumber = 0;
                if (pStringUnpackInt(numberValue, &pageNumber) == P_NULL)
                {
                    PLOG_ERROR("The page node doesn't have an valid page number attribute.");
                    clear();
                    return false;
                }
                const pchar *scenesValue = pageElement.attribute("scenes");
                if (scenesValue == P_NULL)
                {
                    PLOG_ERROR("The page node doesn't have a number of scenes.");
                    clear();
                    return false;
                }
                pint32 sceneNumber = 0;
                if (pStringUnpackInt(scenesValue, &sceneNumber) == P_NULL)
                {
                    PLOG_ERROR("The page node doesn't have an valid scene number attribute.");
                    clear();
                    return false;
                }

                m_pages[pageNumber - 1]->setNumberOfCanvases(sceneNumber);

                pageElement = pageElement.nextSibling();
            }
        }
        else
        {
            PLOG_WARNING("Unknown element node (%s) in book.", childElement.name());
        }

        childElement = childElement.nextSibling();
    }

    // FIXME: have to defer the initialization of clear pass as in the construction,
    // the viewport of context has not been ready.
    if (m_clearRenderPass == P_NULL)
    {
        m_clearRenderPass = PNEW(PRenderPass("clear", P_NULL));
        m_clearRenderPass->target()->setColorClearValue(P_COLOR_BLACK_TRANSPARENT);
        const puint32 *r = context()->rect();
        m_clearRenderPass->target()->setViewport(r[0], r[1], r[2], r[3]);
    }

    return true;
}
    
pbool BBook::initialize()
{
    m_renderState = PNEW(PRenderState(m_context->rect()));
    
    return true;
}

pbool BBook::resume()
{
    return true;
}

void BBook::pause()
{
}

void BBook::update()
{
    // Before render canvases, we should clear the current screen buffer.
    if (!m_pages.isEmpty())
    {
        m_clearRenderPass->render(m_renderState);
    
        if (m_currentPageNumber <= m_pages.count())
        {
            m_pages[m_currentPageNumber - 1]->update();
            m_renderState->beginDraw();
            m_pages[m_currentPageNumber - 1]->render(m_renderState);
            m_renderState->endDraw();
        }
    }
}
    
void BBook::clear()
{
    m_currentPageNumber = 0xffffffff;

    PDELETE(m_content);
    for (puint32 i = 0; i < m_pages.count(); ++i)
    {
        PDELETE(m_pages[i]);
    }
    PDELETEARRAY(m_title);
}


