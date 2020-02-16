// Copyright Peter Leontev 2020

#include "SIM_RokokoFrontend.h"

#include <UT/UT_DSOVersion.h>
#include <GU/GU_PrimPoly.h>
#include <PRM/PRM_Include.h>
#include <SIM/SIM_DopDescription.h>
#include <UT/UT_NetStream.h>
#include <UT/UT_JSONValueMap.h>
#include <UT/UT_JSONValueArray.h>

#include "SIM_RokokoData.h"


SIM_RokokoFrontend::SIM_RokokoFrontend(const SIM_DataFactory* factory) : BaseClass(factory), SIM_OptionsUser(this)
{
    
}

SIM_RokokoFrontend::~SIM_RokokoFrontend()
{
    
}

SIM_Solver::SIM_Result SIM_RokokoFrontend::solveSingleObjectSubclass(
    SIM_Engine &engine,
    SIM_Object &object,
    SIM_ObjectArray &feedbacktoobjects,
    const SIM_Time &timestep,
    bool isNewObject
)
{
    SIM_Result result = SIM_SOLVER_FAIL;

    SIM_RokokoData* rokokoData = SIM_DATA_GET(object, "RokokoData", SIM_RokokoData);
    if (!rokokoData)
    {
        rokokoData = SIM_DATA_CREATE(object, "RokokoData", SIM_RokokoData, 0);
    }

    // Rebuild the snow data to the desired base level if this is a new object.
    if (rokokoData)
    {
        if (isNewObject)
        {

            result = SIM_SOLVER_SUCCESS;
        }
        else
        {
            if (rokokoData->readData())
            {
                animateObjects(object, rokokoData->getPropTrackers());
            }

            result = SIM_SOLVER_SUCCESS;
        }
    }

    return result;
}



void SIM_RokokoFrontend::animateObjects(SIM_Object &object, const UT_Array<PropTrackerInfo>& propTrackersInfo)
{
    for (const PropTrackerInfo& objInfo : propTrackersInfo)
    {
        
    }
}

const SIM_DopDescription* SIM_RokokoFrontend::getMyOwnSolverDescription()
{
    static PRM_Template theTemplates[] = {
        PRM_Template()
    };

    static SIM_DopDescription theDopDescription(
        true,
        "hdk_rokoko_frontend",
        "Rokoko Frontend",
        SIM_SOLVER_DATANAME,
        classname(),
        theTemplates
    );

    return &theDopDescription;
}

void initializeSIM(void*)
{
    IMPLEMENT_DATAFACTORY(SIM_RokokoFrontend);
    IMPLEMENT_DATAFACTORY(SIM_RokokoData);
}
