// pbamboo_asset.cpp
// Manage the assets of book
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the GPL license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pbamboo_asset.h"

PBambooAsset::PBambooAsset(PContext* context)
    : PResourceManager(context)
{
}

PBambooAsset::~PBambooAsset()
{
}

pbool PBambooAsset::initialize()
{
    return true;
}

