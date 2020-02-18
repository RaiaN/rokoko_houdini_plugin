// Copyright Peter Leontev 2020

#ifndef _PROP_TRACKER_INFO_H
#define _PROP_TRACKER_INFO_H

#include <UT/UT_Vector3.h>
#include <UT/UT_Quaternion.h>

struct PropTrackerInfo
{
public:
    std::string name;
    UT_Vector3 position;
    UT_Quaternion rotation;
};

#endif