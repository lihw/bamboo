// ppage.h
// A page of the book.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the GPL license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PPAGE_H
#define PPAGE_H

#include <PFoundation/parray.h>

class PBook;
class PRenderState;

class P_DLLEXPORT PPage
{
    friend class PBook;

public:
    PPage(PBook *book, puint32 pageNo);
    ~PPage();

    void render(PRenderState *renderState);
    void update();
    void setVisiblity(pbool flag);

    pchar *html();
    P_INLINE puint32 pageNo() const { return m_pageNo; }
    P_INLINE puint32 numberOfScenes() const { return m_scenes.count(); }
    P_INLINE PCanvas scene(puint32 index) const { return m_scenes[index]; }

private:
    void clear();
    pbool unpack(const PXmlElement *xmlElement);

private:
    PBook            *m_book;
    puint32           m_pageNo;
    pchar            *m_html;
    PArray<PScene *>  m_scenes;
    pbool             m_visible;
    pchar            *m_htmlFile;    // The html file path in the archive.
    PArray<pchar *>   m_sceneFiles; // The scene file path in the archive.
};


#endif // !PPAGE_H
