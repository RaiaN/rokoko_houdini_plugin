// Copyright Peter Leontev 2020

#ifndef _ROKOKO_RECEIVER_H_
#define _ROKOKO_RECEIVER_H_

#include <FS/FS_EventGenerator.h>
#include <UT/UT_Array.h>
#include <UT/UT_XformOrder.h>
#include <UT/UT_Vector3.h>
#include <OP/OP_Director.h>
#include <OP/OP_Node.h>
#include <CH/CH_Manager.h>

#include "PropTrackerInfo.h"
#include "RokokoSocketReader.h"
#include "RokokoDataParser.h"


class RokokoSocketReader;
class RokokoDataParser;


class RokokoReceiver : public FS_EventGenerator 
{

public:
    RokokoReceiver(const std::string& inIp, int inPort, int inRate) : rate(inRate)
    {
        socketReader = new RokokoSocketReader(inIp, inPort);
        dataParser = new RokokoDataParser();
    }

    virtual ~RokokoReceiver()
    {
        delete socketReader;
        delete dataParser;
    }

    virtual const char* getClassName() const { return "RokokoReceiver"; }
    virtual int getPollTime() { return rate; }
    virtual int processEvents()
    {
        if (!socketReader || !dataParser)
        {
            return 1;
        }

        std::string data;
        if (!socketReader->read(data))
        {
            return 1;
        }

        const UT_Array<PropTrackerInfo>& propTrackersInfo = dataParser->parse(data);
        for (int objInd = 0; objInd < propTrackersInfo.size(); ++objInd)
        {
            const PropTrackerInfo& objInfo = propTrackersInfo[objInd];

            const std::string objName("/obj/" + objInfo.name);

            OP_Node* Node = OPgetDirector()->findNode(objName.c_str());
            if (Node)
            {
                // TODO: Rokoko coordinate system??

                Node->setFloat("t", 0, CHgetEvalTime(), objInfo.position.x());
                Node->setFloat("t", 1, CHgetEvalTime(), objInfo.position.y());
                Node->setFloat("t", 2, CHgetEvalTime(), objInfo.position.z());

                UT_XformOrder order;
                UT_Vector3 EulerRotations = objInfo.rotation.computeRotations(order);

                Node->setFloat("r", 0, CHgetEvalTime(), EulerRotations[0]);
                Node->setFloat("r", 1, CHgetEvalTime(), EulerRotations[1]);
                Node->setFloat("r", 2, CHgetEvalTime(), EulerRotations[2]);
            }
        }

        return 1;
    }

public:
    void start()
    {
        if (!bInstalled)
        {
            installGenerator();
            bInstalled = true;
        }
    }

    void stop()
    {
        uninstallGenerator();
        bInstalled = false;
    }    

private:
    bool bInstalled = false;

    RokokoSocketReader* socketReader;
    RokokoDataParser* dataParser;

    int rate;
};

#endif