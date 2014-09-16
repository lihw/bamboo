// pbook.cpp
// A book
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the GPL license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pbook.h"

#include <PFoundation/pcontext.h>
#include <PFoundation/parchivefile.h>
#include <PFoundation/pxmlelement.h>
#include <PFoundation/pxmldocument.h>
//#include <PFoundation/pnew.h>
//#include <PFoundation/passert.h>
#include <PFoundation/plog.h>


PBook::PBook(PContext *context)
    : PModule("book", context)
{
    m_content       = P_NULL;
    m_currentPageNo = 0xffffffff;
    m_renderState   = P_NULL;
}

PBook::~PBook()
{
    clear();
}

pbool PBook::load(const pchar *bookArchive)
{
    // Reload the book archive.
    PResourceManager *resourceManager = context()->module<PResourceManager>("resource-manager");
    PArchiveFile *archive = resourceManager->archive();
    if (!archive->load(bookArhive))
    {
        return false;
    }

    PInputStream inputStream;
    if (!archive->createInputStream("book.bmb", &inputStream))
    {
        PLOG_ERROR("Failed to read scene configuration file at %s", path);
        return false;
    }
    

    // Destroy the current content.
    clear();

    // Read the book structure (content)
    PXmlDocument xmlDocument;
    if (!xmlDocument.parse(inputStream))
    {
        PLOG_ERROR("The syntax is wrong in %s.", path);
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
    m_title = pstrdup(tileValue);

    const pchar *pageNumberValue = firstNode.attribute("page-number");
    if (pageNumberValue == P_NULL)
    {
        PLOG_ERROR("The book node doesn't have a number of pages.");
        return false;
    }
    pint32 pageNumber = 0;
    if (pStringUnpackInt(pageNumberValue, &pageNumber) == P_NULL)
    {
        PLOG_ERROR("The book node doesn't tell an invalid page number.");
        return false;
    }
    m_pages.resize(pageNumber);


    pageNumber = 0;
    while (childElement.isValid())
    {
        if (pstrcmp(childElement.name(), "content") == 0)
        {
            m_content = PNEW(PContent);
            m_content->unpack(childElement);
        }
        else if (pstrcmp(childElement.name(), "page") == 0)
        {
            PPage *page = PNEW(PContent);
            m_pages[pageNumber++] = page
        }
        else
        {
            PLOG_WARNING("Unknown element node (%s) in book.", childElement.name());
        }
    }

    if (m_pages.count() != pageNumber)
    {
        PLOG_WARNING("The book doesn't have enough pages.");
        clear();
        return false;
    }

    return true;
}
    
pbool PBook::initialize()
{
    return true;
}

pbool PBook::resume()
{
    return true;
}

void PBook::pause()
{
}

void PBook::update()
{
    if (!m_pages.isEmpty())
    {
        m_pages[m_currentPageNo]->update();
        m_renderState->beginDraw();
        m_pages[m_currentPageNo]->render(m_renderState);
        m_renderState->endDraw();
    }
}
    
void PBook::clear()
{
    PDELETE(m_content);
    PDELETE(m_renderState);
    for (puint32 i = 0; i < m_pages.count(); ++i)
    {
        PDELETE(m_pages[i]);
    }
}
