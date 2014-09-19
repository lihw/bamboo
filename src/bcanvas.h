// pbamboo_canvas.h
// A canvas in the page.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the GPL license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PBAMBOO_CANVAS
#define PBAMBOO_CANVAS

#include <Paper3D/pscene.h>

class PMatrix3x3;


class P_DLLEXPORT PBambooCanvas : public PScene
{
    P_OBJECT

    PBambooCanvas(const PBambooCanvas &other) {}
    void operator=(const PBambooCanvas &other) {}

public:
    PBambooScene(const pchar *name, PContext *context);
    virtual ~PBambooScene();

    virtual void update();
    
    // Use hand to rotate the scene and disable the 
    // auto rotation
    void useHand(pbool enabled);
    // Rotate the scene with given matrix.
    void rotate(const PMatrix3x3 &matrix);
    // The auto rotation is enabled by default.
    void enableAutoRotation(pbool enabled);

private:
    pbool m_useHand;
    
};



#endif // !PBAMBOO_CANVAS
