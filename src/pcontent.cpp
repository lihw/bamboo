// pcontent.cpp
// The content of the book.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the GPL license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pcontent.h"

#include <Bamboo/pbook.h>

#include <PFoundation/pxmlelement.h>
#include <PFoundation/pconststring.h>


PContent::PContent(PBook *book)
{
    m_book = book;
}

PContent::~PContent()
{
    clear();
}
    
pbool PContent::unpack(const PXmlElement *xmlElement)
{
    // Sanity check.
    if (pstrcmp(xmlElement->name(), "content") != 0)
    {
        PLOG_ERROR("It is not a content element");
        return false;
    }

    PXmlElement childElement = xmlElement->firstChild();
    while (childElement.isValid())
    {
        if (pstrcmp(childElement.name(), "entry") == 0)
        {
            const pchar *levelValue      = childElement.attribute("level");
            const pchar *textValue       = childElement.attribute("text");
            const pchar *pageNumberValue = childElement.attribute("pageno");

            if (levelValue == P_NULL ||
                textValue == P_NULL ||
                pageNumberValue == P_NULL)
            {
                PLOG_WARNING("entry is incomplete.");
            }
            else
            {
                pint32 pageNumber;
                pint32 level;
                if (pStringUnpackInt(levelValue, &level) == P_NULL ||
                    pStringUnpackInt(pageNumberValue, &pageNumber) == P_NULL)
                {
                    PLOG_WARNING("entry information is incomplete.");
                }
                else
                {
                    if ((puint32)pageNumber > m_book->numberOfPages())
                    {
                        PLOG_ERROR("entry's page number is not valide");
                    }
                    else
                    {
                        PContentEntry *entry = PNEW(PContentEntry);
                        entry->level      = level;
                        entry->text       = pstrdup(textValue);
                        entry->pageNumber = pageNumber;
                        m_entries.append(entry);
                    }
                }
            }
        }
        else
        {
            PLOG_WARNING("Unknown element node (%s) in entry.", childElement.name());
        }

        childElement = childElement.nextSibling();
    }


    return true;
}

void PContent::clear()
{
    for (puint32 i = 0; i < m_entries.count(); ++i)
    {
        PDELETEARRAY(m_entries[i]->text);
        PDELETE(m_entries[i]);
    }
}
