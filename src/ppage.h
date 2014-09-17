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
class PScene;
class PXmlElement;

class P_DLLEXPORT PPage
{
    friend class PBook;

public:
    PPage(PBook *book, puint32 pageNumber);
    ~PPage();

    void render(PRenderState *renderState);
    void update();
    void setVisiblity(pbool flag);
    void setNumberOfScenes(puint32 number);

    pchar *html();
    P_INLINE puint32 pageNumber() const { return m_pageNumber; }
    P_INLINE puint32 numberOfScenes() const { return m_scenes.count(); }
    P_INLINE PScene *scene(puint32 index) const { return m_scenes[index]; }

private:
    void clear();
    pbool unpack(const PXmlElement *xmlElement);

private:
    PBook            *m_book;
    puint32           m_pageNumber;
    pchar            *m_html;
    PArray<PScene *>  m_scenes;
    pbool             m_visible;
    pchar            *m_htmlFile;    // The html file path in the archive.
};


#endif // !PPAGE_H
