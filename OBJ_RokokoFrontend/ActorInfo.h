// Copyright Peter Leontev 2020

#ifndef _ACTOR_INFO_H
#define _ACTOR_INFO_H


#include <string>

#include <UT/UT_StringHolder.h>
#include <UT/UT_Array.h>

#include "TransformInfo.h"
#include "BoneInfo.h"

struct ActorInfo
{
public:
    std::string name;
    // std::string timestamp;
    UT_Array<BoneInfo> bones;
};

#endif