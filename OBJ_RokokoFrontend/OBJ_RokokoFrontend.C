// Copyright Peter Leontev 2020

#include "OBJ_RokokoFrontend.h"

#include <SYS/SYS_Math.h>
#include <UT/UT_DSOVersion.h>
#include <PRM/PRM_Include.h>
#include <OP/OP_Operator.h>
#include <OP/OP_OperatorPair.h>
#include <OP/OP_OperatorTable.h>

#include "RokokoReceiver.h"




// Constructor for new object class
OBJ_RokokoFrontend::OBJ_RokokoFrontend(OP_Network* net, const char* name, OP_Operator* op) : OBJ_Geometry(net, name, op)
{
    // TODO: use framerate param
    receiver = new RokokoReceiver(16);
}

// virtual destructor for new object class
OBJ_RokokoFrontend::~OBJ_RokokoFrontend()
{
    if (receiver)
    {
        receiver->stop();

        delete receiver;
        receiver = nullptr;
    }
}


OP_Node* OBJ_RokokoFrontend::myConstructor(OP_Network* net, const char* name, OP_Operator* op)
{
    return new OBJ_RokokoFrontend(net, name, op);
}


OP_ERROR OBJ_RokokoFrontend::cookMyObj(OP_Context& context)
{
    OP_ERROR errorstatus = OP_ERROR::UT_ERROR_NONE;
    
    if (receiver)
    {
        receiver->start();
    }

    return errorstatus;
}

// this function installs the new object in houdini's object table.
void newObjectOperator(OP_OperatorTable* table)
{
    table->addOperator(
        new OP_Operator(
            "obj_rokoko_frontend", "Rokoko Frontend",
            OBJ_RokokoFrontend::myConstructor,
            OBJ_Geometry::getTemplateList(OBJ_PARMS_PLAIN),
            OBJ_RokokoFrontend::theChildTableName,
            0, 
            1,
            0
        )
    );
}