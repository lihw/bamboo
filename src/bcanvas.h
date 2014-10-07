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

    // Set the scene to rotate continuosly. It is true by default.
    void setRotationEnabled(pbool enabled);
    P_INLINE pbool isRotationEnabled() const { return m_rotationEnabled; }
    // By default, the scene rotates with y axis.
    void setRotation(pfloat32 rx, pfloat32 ry, pfloat32 rz);
    // Only effective in 3rd-person view.
    void rotate(const PQuaternion &quat);
    // Only effective in first person view.
    void rotate(pfloat32 dx, pfloat32 dy);
    // Start top rotate with hand.
    void useHand(pbool enabled);

    // Is it the first person view? By default, it is false.
    P_INLINE pbool isFirstPersonView() const { return m_firstPersonView; }

    // The scene will be scaled to the fullscreen if this flag is set.
    // It is true by default.
    void setScalingEnabled(pbool enabled);
    P_INLINE pbool isScalingEnabled() const { return m_scalingEnabled; }

protected:
    pbool unpack(const PXmlElement *xmlElement);
    

private:
    pbool       m_rotationEnabled;
    PVector3    m_rotationSpeed;
    pbool       m_scalingEnabled;
    pbool       m_firstPersonView;
    PVector3    m_currentRotation;
};



#endif // !BCANVAS_H
