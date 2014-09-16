// pcanvas.h
// A canvas in a page
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the GPL license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PCANVAS_H
#define PCANVAS_H

class P_DLLEXPORT PCanvas
{
public:
    PCanvas(PPage* page, const puint32 *viewport);
    ~PCanvas();

    void render(PRenderState *renderState);

private:
    puint32 m_viewport[4];
    PScene *scene;
};


#endif // !PCANVAS_H
