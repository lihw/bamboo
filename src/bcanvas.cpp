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
#include <PFoundation/pxmlelement.h>
#include <PFoundation/pconststring.h>


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
    
pbool BCanvas::unpack(const PXmlElement *xmlElement)
{
    if (!PScene::unpack(xmlElement))
    {
        return false;
    }

    const pchar *scalingEnabledValue = xmlElement->attribute("scaling_enabled");
    if (scalingEnabledValue != P_NULL && pstrcmp(scalingEnabledValue, "false") == 0)
    {
        m_scalingEnabled = false;
    }
    else 
    {
        m_scalingEnabled = true;
    }
    
    const pchar *rotationEnabledValue = xmlElement->attribute("rotation_enabled");
    if (rotationEnabledValue != P_NULL && pstrcmp(rotationEnabledValue, "false") == 0)
    {
        m_rotationEnabled = false;
    }
    else 
    {
        m_rotationEnabled = true;
    }
   
    const pchar *firstPersonViewValue = xmlElement->attribute("first_person_view");
    if (firstPersonViewValue != P_NULL && pstrcmp(firstPersonViewValue, "true") == 0)
    {
        m_firstPersonView = true;
    }
    else 
    {
        m_firstPersonView = false;
    }
    
    const pchar *rotationSpeedValue = xmlElement->attribute("rotation_speed");
    if (rotationSpeedValue != P_NULL) 
    {
        const pchar *p = rotationSpeedValue;
        if ((p = pStringUnpackFloat(p, &m_rotationSpeed.m_v[0])) == P_NULL ||
            (p = pStringUnpackFloat(p, &m_rotationSpeed.m_v[1])) == P_NULL ||
            (p = pStringUnpackFloat(p, &m_rotationSpeed.m_v[2])) == P_NULL)
        {
            PLOG_ERROR("The canvas node doesn't have a valid rotation speed.");
            return false;
        }
    }
    else 
    {
        m_rotationSpeed.m_v[0] = 0;
        m_rotationSpeed.m_v[1] = 0.0001f;
        m_rotationSpeed.m_v[2] = 0;
    }

    return true;
}
