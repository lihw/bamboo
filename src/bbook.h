// pbook.h
// A book
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the GPL license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PBOOK_H
#define PBOOK_H

#include <PFoundation/parray.h>
#include <PFoundation/pmodule.h>

class PPage;
class PContent;
class PRenderState;

class P_DLLEXPORT PBook : public PModule
{
public:
    PBook(PContext *context);
    virtual ~PBook();

    pbool load(const pchar *bookArchive);

    P_INLINE puint32 numberOfPages() const { return m_pages.count(); }
    P_INLINE PPage *page(puint32 pageno) const { return m_pages[pageno]; }
    P_INLINE PContent *content() const { return m_content; }
    P_INLINE pchar *title() const { return m_title; }
    P_INLINE puint32 currentPageNumber() const { return m_currentPageNumber; }
    P_INLINE PPage *currentPage() const 
    { 
        if (m_currentPageNumber <= m_pages.count()) 
        {
            return m_pages[m_currentPageNumber < 1];
        }
        return P_NULL;
    }

    void setCurrentPageNumber(puint32 pageNumber);

    virtual pbool initialize();
    virtual pbool resume();
    virtual void pause();
    virtual void update();

private:
    void clear();

private:
    PArray<PPage*> m_pages;
    PContent      *m_content;
    PRenderState  *m_renderState;
    puint32        m_currentPageNumber;
    pchar         *m_title;
};


#endif // !PBOOK_H
