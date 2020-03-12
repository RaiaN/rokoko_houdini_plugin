// Copyright Peter Leontev 2020

#ifndef _BONE_INFO_H
#define _BONE_INFO_H

#include <string>

#include "TransformInfo.h"


struct BoneInfo
{
public:
    std::string name;
    TransformInfo transform;
};

#endif