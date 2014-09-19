// bcanvas.h
// A canvas in the page.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the GPL license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef BCANVAS_H
#define BCANVAS_H

#include <Paper3D/pscene.h>

struct PQuaternion;


class P_DLLEXPORT BCanvas : public PScene
{
    P_OBJECT

    BCanvas(const BCanvas &other) : PScene(P_NULL, P_NULL) {}
    void operator=(const BCanvas &other) {}

public:
    BCanvas(const pchar *name, PContext *context);
    virtual ~BCanvas();

    virtual void update();
    
    // Use hand to rotate the scene and disable the 
    // auto rotation
    void useHand(pbool enabled);
    // Rotate the scene with given matrix.
    void rotate(const PQuaternion &quat);
    void rotate(pfloat32 dx, pfloat32 dy);
    // The auto rotation is enabled by default.
    void enableAutoRotation(pbool enabled);

private:
    pbool      m_autoRotating;
    PVector3   m_currentRotation;
};



#endif // !BCANVAS_H
