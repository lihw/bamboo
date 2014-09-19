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
    m_autoRotating = true;
}

BCanvas::~BCanvas()
{
}

void BCanvas::update()
{
    const pfloat32 xRotSpeed = -0.001f;
    const pfloat32 yRotSpeed = -0.0005f;

    if (m_autoRotating)
    {
 	    PVector3 r = m_root->transform().rotation();
	    r[0] += m_context->clock().deltaTime() * xRotSpeed;
        r[1] += m_context->clock().deltaTime() * yRotSpeed;
	    m_root->transform().setRotation(r);   
    }
}

void BCanvas::useHand(pbool enabled)
{
    m_autoRotating = !enabled;
    if (!m_autoRotating)
    {
        // Save the current rotation.
        m_currentRotation = m_root->transform().rotation();
    }
}

void BCanvas::rotate(const PQuaternion &quat)
{
    PVector3 r;

    pQuaternionGetRotation(quat.m_q, r.m_v[0], r.m_v[1], r.m_v[2]);
    PLOG_INFO("(%f, %f, %f)", r.m_v[0], r.m_v[1], r.m_v[2]);
 	    
    r += m_currentRotation;

    m_root->transform().setRotation(r);
}

void BCanvas::rotate(pfloat32 dx, pfloat32 dy)
{
    PVector3 r;
    
    const pfloat32 rotSpeed = 5.0f;

    r.m_v[0] = -rotSpeed * dy;
    r.m_v[1] = -rotSpeed * dx;
    r.m_v[2] = 0.0f;
    
    r += m_root->transform().rotation();

    m_root->transform().setRotation(r);
}

void BCanvas::enableAutoRotation(pbool enabled)
{
    m_autoRotating = enabled;
}
