// Copyright Peter Leontev 2020

#ifndef _ROKOKO_RECEIVER_H
#define _ROKOKO_RECEIVER_H

#include <FS/FS_EventGenerator.h>
#include <UT/UT_Array.h>
#include <OP/OP_Director.h>
#include <OP/OP_Node.h>
#include <CH/CH_Manager.h>

#include "PropTrackerInfo.h"
#include "RokokoData.h"



class RokokoReceiver : public FS_EventGenerator {
public:
    RokokoReceiver(int inRate) : rate(inRate)
    {
        data = new RokokoData();
    }

    virtual ~RokokoReceiver()
    {
        delete data;
    }

    virtual const char* getClassName() const { return "RokokoReceiver"; }
    virtual int getPollTime() { return rate; }
    virtual int processEvents()
    {
        if (data && data->readData())
        {
            const UT_Array<PropTrackerInfo>& propTrackersInfo = data->getPropTrackers();

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
                    Node->setFloat("t", 0, CHgetEvalTime(), objInfo.position.z());
                    

                    /*Node->setFloat("r", 0, CHgetEvalTime(), objInfo.rotation.x());
                    Node->setFloat("r", 1, CHgetEvalTime(), objInfo.rotation.y());
                    Node->setFloat("r", 2, CHgetEvalTime(), objInfo.rotation.z());
                    Node->setFloat("r", 3, CHgetEvalTime(), objInfo.rotation.w());*/
                }
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

    RokokoData* data = nullptr;
    int rate = 1.0f;
};

#endif