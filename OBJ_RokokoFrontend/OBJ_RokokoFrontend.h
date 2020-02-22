// Copyright Peter Leontev 2020

#ifndef _OBJ_ROKOKO_FRONTEND_H
#define _OBJ_ROKOKO_FRONTEND_H


#include <OBJ/OBJ_Geometry.h>
#include <OP/OP_Error.h>
#include <OP/OP_Context.h>
#include <OP/OP_OperatorPair.h>


class OP_Network;
class OP_Operator;
class OP_TemplatePair;
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



    
protected:
    OP_ERROR cookMyObj(OP_Context& context) override;

private:
    int PORT(fpreal t)
    {
        return evalInt("port", 0, t);
    }

    int UPDATE_RATE(fpreal t)
    {
        return evalInt("update_rate", 0, t);
    }

    std::string IP(fpreal t)
    {
        UT_String value;
        evalString(value, "ip", 0, t);

        return value.c_str();
    }

private:
    RokokoReceiver* receiver;
};


#endif
