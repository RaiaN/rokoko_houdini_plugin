// Copyright Peter Leontev 2020

#ifndef _OBJ_ROKOKO_FRONTEND_H_
#define _OBJ_ROKOKO_FRONTEND_H_


#include <OBJ/OBJ_Geometry.h>
#include <OP/OP_Error.h>
#include <OP/OP_Context.h>
#include <OP/OP_OperatorPair.h>

class OP_Network;
class OP_Operator;
class OP_TemplatePair;
class PRM_Template;
class RokokoReceiver;


class OBJ_RokokoFrontend : public OBJ_Geometry
{
public:
    OBJ_RokokoFrontend(OP_Network* net, const char* name, OP_Operator* op);

    virtual ~OBJ_RokokoFrontend();

    static OP_Node* myConstructor(
        OP_Network* net,
        const char* name,
        OP_Operator* entry
    );

    static OP_TemplatePair* buildTemplatePair(OP_TemplatePair* prevstuff);

    static int UI_OnUpdateRateChanged(void* data, int index, fpreal t, const PRM_Template* templateParam);
    static int UI_OnIpOrPortChanged(void* data, int index, fpreal t, const PRM_Template* templateParam);
    static int UI_OnReset(void* data, int index, fpreal t, const PRM_Template* templateParam);

    void OnUpdateRateChanged();
    void OnIpOrPortChanged();
    void OnReset();

public:
    int GET_PORT();
    int GET_UPDATE_RATE();
    std::string GET_IP();
    
protected:
    OP_ERROR cookMyObj(OP_Context& context) override;

private:
    RokokoReceiver* receiver;
};


#endif
