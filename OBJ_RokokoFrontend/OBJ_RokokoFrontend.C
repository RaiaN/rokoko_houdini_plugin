// Copyright Peter Leontev 2020

#include "OBJ_RokokoFrontend.h"

#include <SYS/SYS_Math.h>
#include <UT/UT_DSOVersion.h>
#include <PRM/PRM_Include.h>
#include <OP/OP_Operator.h>
#include <OP/OP_OperatorPair.h>
#include <OP/OP_OperatorTable.h>
#include <CH/CH_Manager.h>

#include "RokokoReceiver.h"


static PRM_Name Port_Param("port", "Port");
static PRM_Name IP_Param("ip", "IP");
static PRM_Name UpdateRate_Param("update_rate", "Update rate (FPS)");
static PRM_Name ResetBtnParam("reset_receiver", "Reset");

static PRM_Default PortDefault(14013);
static PRM_Default IPDefault(0, "127.0.0.1");
static PRM_Default UpdateRateDefault(60);

static PRM_Range PortRange(PRM_RANGE_UI, 1, PRM_RANGE_UI, 65535);
static PRM_Range UpdateRateRange(PRM_RANGE_UI, 1, PRM_RANGE_UI, 240);




static PRM_Template templatelist[] =
{
    PRM_Template(PRM_INT_E, 1, &Port_Param, &PortDefault, 0, &PortRange, &OBJ_RokokoFrontend::UI_OnIpOrPortChanged),
    PRM_Template(PRM_STRING, 1, &IP_Param, &IPDefault, 0, 0, &OBJ_RokokoFrontend::UI_OnIpOrPortChanged),
    PRM_Template(PRM_FLT_E, 1, &UpdateRate_Param, &UpdateRateDefault, 0, &UpdateRateRange, &OBJ_RokokoFrontend::UI_OnUpdateRateChanged),
    PRM_Template(PRM_CALLBACK, 1, &ResetBtnParam, 0, 0, 0, &OBJ_RokokoFrontend::UI_OnReset),

    // blank terminating Template.
    PRM_Template()
};

// Constructor for new object class
OBJ_RokokoFrontend::OBJ_RokokoFrontend(OP_Network* net, const char* name, OP_Operator* op) : OBJ_Geometry(net, name, op)
{
    const std::string ip = GET_IP();
    const int port = GET_PORT();
    const int updateRateMs = 1000 / GET_UPDATE_RATE();

    receiver = new RokokoReceiver(ip, port, updateRateMs);

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

static void copyParmWithInvisible(PRM_Template& src, PRM_Template& dest)
{
    PRM_Name* new_name;

    new_name = new PRM_Name(src.getToken(), src.getLabel(), src.getExpressionFlag());
    new_name->harden();

    dest.initialize(
        (PRM_Type)(src.getType() | PRM_TYPE_INVISIBLE),
        src.getTypeExtended(),
        src.exportLevel(),
        src.getVectorSize(),
        new_name,
        src.getFactoryDefaults(),
        src.getChoiceListPtr(),
        src.getRangePtr(),
        src.getCallback(),
        src.getSparePtr(),
        src.getParmGroup(),
        (const char *)src.getHelpText(),
        src.getConditionalBasePtr()
    );
}
// this function returns the OP_TemplatePair that combines the parameters
// of this object with those of its ancestors in the (object class hierarchy)
OP_TemplatePair* OBJ_RokokoFrontend::buildTemplatePair(OP_TemplatePair* prevstuff)
{
    OP_TemplatePair* rokokoTemplates;
    OP_TemplatePair* geo;

    // The parm templates here are not created as a static list because
    // if that static list was built before the OBJbaseTemplate static list
    // (which it references) then that list would be corrupt. Thus we have
    // to force our static list to be created after OBJbaseTemplate.
    static PRM_Template* theTemplate = 0;
    if (!theTemplate)
    {       
        PRM_Template* obj_template = OBJ_Geometry::getTemplateList(OBJ_PARMS_PLAIN);
        int size = PRM_Template::countTemplates(obj_template);
        theTemplate = new PRM_Template[size + 1];   // add +1 for sentinel
        
        for (int i = 0; i < size; i++)
        {
            theTemplate[i] = obj_template[i];
            copyParmWithInvisible(obj_template[i], theTemplate[i]);
        }
    }
    // Here, we have to "inherit" template pairs from geometry and beyond. To
    // do this, we first need to instantiate our template list, then add the
    // base class templates.
    rokokoTemplates = new OP_TemplatePair(templatelist, prevstuff);
    geo = new OP_TemplatePair(theTemplate, rokokoTemplates);
    return geo;
}

int OBJ_RokokoFrontend::UI_OnUpdateRateChanged(void* data, int index, fpreal t, const PRM_Template* templateParam)
{
    OBJ_RokokoFrontend* rokokoFrontend = static_cast<OBJ_RokokoFrontend*>(data);
    rokokoFrontend->OnUpdateRateChanged();

    return 1;
}


int OBJ_RokokoFrontend::UI_OnIpOrPortChanged(void* data, int index, fpreal t, const PRM_Template* templateParam)
{
    OBJ_RokokoFrontend* rokokoFrontend = static_cast<OBJ_RokokoFrontend*>(data);
    rokokoFrontend->OnIpOrPortChanged();

    return 1;
}


int OBJ_RokokoFrontend::UI_OnReset(void* data, int index, fpreal t, const PRM_Template* templateParam)
{
    OBJ_RokokoFrontend* rokokoFrontend = static_cast<OBJ_RokokoFrontend*>(data);
    rokokoFrontend->OnReset();

    return 1;
}

void OBJ_RokokoFrontend::OnUpdateRateChanged()
{
    const int updateRateMs = 1000 / GET_UPDATE_RATE();
    receiver->setUpdateRate(updateRateMs);
}

void OBJ_RokokoFrontend::OnIpOrPortChanged()
{
    const std::string newIp = GET_IP();
    const int newPort = GET_PORT();

    receiver->setIpAndPort(newIp, newPort);
}

void OBJ_RokokoFrontend::OnReset()
{
    receiver->reset();
}

int OBJ_RokokoFrontend::GET_PORT()
{
    return evalInt("port", 0, CHgetEvalTime());
}

int OBJ_RokokoFrontend::GET_UPDATE_RATE()
{
    return evalInt("update_rate", 0, CHgetEvalTime());
}

std::string OBJ_RokokoFrontend::GET_IP()
{
    UT_String value;
    evalString(value, "ip", 0, CHgetEvalTime());

    return value.c_str();
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
            "obj_rokoko_frontend",
            "Rokoko Frontend",
            OBJ_RokokoFrontend::myConstructor,
            OBJ_RokokoFrontend::buildTemplatePair(0),
            OBJ_RokokoFrontend::theChildTableName,
            0,
            1,
            nullptr
        )
    );
}