// bcanvas.cpp
// A canvas in the page.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the GPL license.
//
// Hongwei Li lihw81@gmail.com
//

#include "bcanvas.h"

#include <PFoundation/pcontext.h>
#include <PFoundation/pquaternion.h>


P_OBJECT_DEFINE(BCanvas)

BCanvas::BCanvas(const pchar *name, PContext *context)
    : PScene(name, context)
{
    m_rotationEnabled = true;
    m_rotationSpeed   = pVector3(0, 0.0001f, 0);
    m_scalingEnabled  = true;
    m_firstPersonView = false;
}

BCanvas::~BCanvas()
{
}

void BCanvas::update()
{
    if (m_rotationEnabled)
    {
        pfloat32 dt = m_context->clock().deltaTime();
        PVector3 r = m_root->transform().rotation();
        r +=  m_rotationSpeed * dt;
	    m_root->transform().setRotation(r);   
    }
}
    
void BCanvas::setRotation(pfloat32 x, pfloat32 y, pfloat32 z)
{
    m_rotationSpeed.m_v[0] = x;
    m_rotationSpeed.m_v[1] = y;
    m_rotationSpeed.m_v[2] = z;
}
    
void BCanvas::setRotationEnabled(pbool enabled)
{
    m_rotationEnabled = enabled;
}

void BCanvas::useHand(pbool enabled)
{
    m_rotationEnabled = !enabled;
    if (!m_rotationEnabled)
    {
        // Save the current rotation.
        m_currentRotation = m_root->transform().rotation();
    }
}

void BCanvas::rotate(const PQuaternion &quat)
{
    PVector3 r;

    pQuaternionGetRotation(quat.m_q, r.m_v[0], r.m_v[1], r.m_v[2]);

    r += m_currentRotation;

    m_root->transform().setRotation(r);
}

void BCanvas::rotate(pfloat32 dx, pfloat32 dy)
{
    PASSERT(m_firstPersonView);

    PVector3 r;
    
    const pfloat32 rotSpeed = 5.0f;

    r.m_v[0] = -rotSpeed * dy;
    r.m_v[1] = -rotSpeed * dx;
    r.m_v[2] = 0.0f;
    
    r += m_root->transform().rotation();

    m_root->transform().setRotation(r);
}

void BCanvas::setScalingEnabled(pbool enabled)
{
    m_scalingEnabled = enabled;
}
