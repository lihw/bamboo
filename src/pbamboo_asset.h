// pbamboo_asset.h
// Manage the assets of book
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the GPL license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PBAMBOO_KASSET_H
#define PBAMBOO_KASSET_H

#include <Paper3D/presourcemanager.h>


class P_DLLEXPORT PBambooAsset : public PResourceManager
{
public:
    PBambooAsset(PContext* context);
    virtual ~PBambooAsset();

    virtual pbool initialize();
};


#endif // !PAMBOO_ASSET_H
