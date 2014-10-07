// bpage.h
// A page of the book.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the GPL license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef BPAGE_H
#define BPAGE_H

#include <PFoundation/parray.h>

class BBook;
class BCanvas;

class PRenderState;
class PXmlElement;


class P_DLLEXPORT BPage
{
    friend class BBook;

public:
    BPage(BBook *book, puint32 pageNumber);
    ~BPage();

    void render(PRenderState *renderState);
    void update();
    void setVisiblity(pbool flag);
    void setNumberOfCanvases(puint32 number);

    P_INLINE const pchar *html() { return m_html; }
    P_INLINE puint32 pageNumber() const { return m_pageNumber; }
    P_INLINE puint32 numberOfCanvases() const { return m_canvases.count(); }
    P_INLINE BCanvas *canvas(puint32 index) const { return m_canvases[index]; }
    
    void load();
    void unload();
    
    void onPanBegin(pint32 x, pint32 y);
    void onPan(pint32 x, pint32 y, pint32 dx, pint32 dy);
    void onPanEnd(pint32 x, pint32 y, pint32 dx, pint32 dy);
   
    void onPinchBegin(const pint32 *pt1, const pint32 *pt2);
    void onPinch(const pint32 *pt1, const pint32 *pt2, pfloat32 angle, pfloat32 scaling);
    void onPinchEnd();

    void onLongPress(pint32 dx, pint32 dy);
    
    pbool isPointInside(pint32 x, pint32 y);

private:
    void clear();
    pbool unpack(const PXmlElement *xmlElement);
    BCanvas *locateCanvas(pint32 x, pint32 y);

private:
    // The page can display canvas only, the mixture of
    // canvas and HTML5 and some state in between when
    // a canvas is scaling up or scaling down.
    enum 
    {
        ZOOMOUT,
        TRANSITION_SCALEUP,
        ZOOMIN,
        TRANSITION_SCALEDOWN,

        NUM_STATES,
        
        UNITIALIZED,
    };

    BBook              *m_book;
    puint32             m_pageNumber;
    pchar              *m_html;
    PArray<BCanvas*>    m_canvases;
    pbool               m_visible;
    pchar              *m_htmlFile;    // The html file path in the archive.
    BCanvas            *m_currentCanvas; // The current interacting canvas
    puint32             m_state;
    puint32             m_originalViewport[4]; // The original viewport of current canvas

};


#endif // !BPAGE_H
