// basset.cpp
// Manage the assets of book
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the GPL license.
//
// Hongwei Li lihw81@gmail.com
//

#include "basset.h"

BAsset::BAsset(PContext* context)
    : PResourceManager(context)
{
}

BAsset::~BAsset()
{
}

pbool BAsset::initialize()
{
    return true;
}

