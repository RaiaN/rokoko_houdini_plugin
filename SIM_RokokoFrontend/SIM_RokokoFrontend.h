// Copyright Peter Leontev 2020

#ifndef _SIM_ROKOKO_FRONTEND_H
#define _SIM_ROKOKO_FRONTEND_H

#include <SIM/SIM_SingleSolver.h>
#include <SIM/SIM_Solver.h>
#include <SIM/SIM_OptionsUser.h>
#include <SIM/SIM_Data.h>
#include <SIM/SIM_DataFactory.h>
#include <SIM/SIM_Object.h>
#include <SIM/SIM_ObjectArray.h>
#include <SIM/SIM_Time.h>
#include <SIM/SIM_Geometry.h>
#include <SIM/SIM_GeometryCopy.h>
#include <SIM/SIM_Engine.h>
#include <SIM/SIM_DataUtils.h>
#include <UT/UT_JSONValue.h>


class SIM_RokokoFrontend : public SIM_SingleSolver, public SIM_OptionsUser
{
public:
    explicit SIM_RokokoFrontend(const SIM_DataFactory *factory);
    virtual ~SIM_RokokoFrontend();

protected:
    virtual SIM_Result solveSingleObjectSubclass(
        SIM_Engine &engine,
        SIM_Object &object,
        SIM_ObjectArray &feedbacktoobjects,
        const SIM_Time &timestep,
        bool newobject
    );

    void solveForObject(const UT_JSONValue& jsonValue);

private:
    static const SIM_DopDescription* getMyOwnSolverDescription();
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_RokokoFrontend, SIM_SingleSolver, "Rokoko Frontent Solver", getMyOwnSolverDescription());
};

#endif
