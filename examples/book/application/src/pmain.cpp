// pmain.cpp
// "Book" example
//
// Copyright 2012 - 2014 Future Interface. All rights reserved.
//
// Future Interface  lihw81@gmail.com
//

#include <Bamboo/bamboo.h>

#include <Paper3D/paper3d.h>

#include <PFoundation/pfoundation.h>


void pMain(int argc, char* argv[])
{
    PASSERT(PActivity::s_activity != P_NULL);

    if (PActivity::s_activity != P_NULL)
    {
        PContextProperties contextProperties;
        contextProperties.m_contextName = PString("book");
        contextProperties.m_archiveName = PString("book.par");
#if defined P_WIN32
        contextProperties.m_windowWidth = 1280;
        contextProperties.m_windowHeight = 720;
        contextProperties.m_multisamples = 2;
        contextProperties.m_maxFps = 30;
#elif defined P_ANDROID
		contextProperties.m_windowWidth = 0xffffffff;
        contextProperties.m_windowHeight = 0xffffffff;
#endif

        PContext* context = PNEW(BContext(contextProperties));
		context->addModule(PNEW(BAsset(context)));
		context->addModule(PNEW(BBook(context)));
		// TODO: initialize more modules here.

        PActivity::s_activity->addContext(context);
    }
    else
    {
        PLOG_ERROR("No running activity.");
    }
}