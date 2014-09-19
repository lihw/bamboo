// basset.h
// Manage the assets of book
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the GPL license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef BASSET_H
#define BASSET_H

#include <Paper3D/presourcemanager.h>


class P_DLLEXPORT BAsset : public PResourceManager
{
public:
    BAsset(PContext* context);
    virtual ~BAsset();

    virtual pbool initialize();
};


#endif // !BASSET_H
