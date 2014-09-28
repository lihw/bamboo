// bbook.h
// A book
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the GPL license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef BBOOK_H
#define BBOOK_H

#include <PFoundation/parray.h>
#include <PFoundation/pmodule.h>
#include <PFoundation/parcball.h>
#include <PFoundation/pinterpolatedvalue.h>

class BPage;
class BContent;

class PRenderState;


class P_DLLEXPORT BBook : public PModule
{
    BBook(const BBook &other) : PModule(P_NULL, P_NULL), m_value(0, 0, 0) {}
    void operator=(const BBook &other) {}

public:
    BBook(PContext *context);
    virtual ~BBook();

    pbool load(const pchar *bookArchive);

    P_INLINE puint32 numberOfPages() const { return m_pages.count(); }
    P_INLINE BPage *page(puint32 pageno) const { return m_pages[pageno]; }
    P_INLINE BContent *content() const { return m_content; }
    P_INLINE pchar *title() const { return m_title; }
    P_INLINE puint32 currentPageNumber() const { return m_currentPageNumber; }
    P_INLINE BPage *currentPage() const 
    { 
        if (m_currentPageNumber <= m_pages.count()) 
        {
            return m_pages[m_currentPageNumber - 1];
        }
        return P_NULL;
    }
    P_INLINE PArcball *arcball() { return &m_arcball; }
    P_INLINE PInterpolatedValue *value() { return &m_value; }

    void openPage(puint32 pageNumber);

    virtual pbool initialize();
    virtual pbool resume();
    virtual void pause();
    virtual void update();

private:
    void clear();

private:
    PArray<BPage*>      m_pages;
    BContent           *m_content;
    PRenderState       *m_renderState;
    puint32             m_currentPageNumber;
    pchar              *m_title;
    PArcball            m_arcball;
	PInterpolatedValue  m_value;    // scaleup-down transition value.
};


#endif // !BBOOK_H
