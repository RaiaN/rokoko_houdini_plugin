// Copyright Peter Leontev 2020

#include "RokokoReceiver.h"

#include <UT/UT_Array.h>
#include <UT/UT_XformOrder.h>
#include <UT/UT_Vector3.h>
#include <OP/OP_Director.h>
#include <OP/OP_Node.h>
#include <CH/CH_Manager.h>

#include "RokokoSocketReader.h"
#include "RokokoDataParser.h"

#include "PropTrackerInfo.h"
#include "ActorInfo.h"



RokokoReceiver::RokokoReceiver(const std::string& inIp, int inPort, int inRate) : rate(inRate)
{
    socketReader = new RokokoSocketReader(inIp, inPort);
    dataParser = new RokokoDataParser();
}

RokokoReceiver::~RokokoReceiver()
{
    delete socketReader;
    delete dataParser;
}

const char* RokokoReceiver::getClassName() const
{
    return "RokokoReceiver";
}

int RokokoReceiver::getPollTime()
{
    return rate;
}

int RokokoReceiver::processEvents()
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

    dataParser->parse(data);

    const UT_Array<PropTrackerInfo>& propTrackers = dataParser->getPropTrackers();

    for (int objInd = 0; objInd < propTrackers.size(); ++objInd)
    {
        const PropTrackerInfo& objInfo = propTrackers[objInd];

        const std::string objName("/obj/" + objInfo.name);

        OP_Node* Node = OPgetDirector()->findNode(objName.c_str());
        if (Node)
        {
            const fpreal evalTime = CHgetEvalTime();

            // TODO: Rokoko coordinate system?

            Node->setFloat("t", 0, evalTime, objInfo.transform.position.x());
            Node->setFloat("t", 1, evalTime, objInfo.transform.position.y());
            Node->setFloat("t", 2, evalTime, objInfo.transform.position.z());

            UT_XformOrder order;
            UT_Vector3 EulerRotations = objInfo.transform.rotation.computeRotations(order);

            Node->setFloat("r", 0, evalTime, EulerRotations[0]);
            Node->setFloat("r", 1, evalTime, EulerRotations[1]);
            Node->setFloat("r", 2, evalTime, EulerRotations[2]);
        }
    }

    const UT_Array<ActorInfo>& actors = dataParser->getActors();

    for (int actorInd = 0; actorInd < actors.size(); ++actorInd)
    {
        const ActorInfo& actorInfo = actors[actorInd];

        for (int boneInd = 0; boneInd < actorInfo.bones.size(); ++boneInd)
        {
            const BoneInfo& boneInfo = actorInfo.bones[boneInd];

            const std::string boneName("/obj/Root/" + boneInfo.name + "_bone");

            OP_Node* Node = OPgetDirector()->findNode(boneName.c_str());
            if (Node)
            {
                const fpreal evalTime = CHgetEvalTime();

                // TODO: Rokoko coordinate system?

                Node->setFloat("t", 0, evalTime, boneInfo.transform.position.x() * 10);
                Node->setFloat("t", 1, evalTime, boneInfo.transform.position.y() * 10);
                Node->setFloat("t", 2, evalTime, boneInfo.transform.position.z() * 10);

                UT_XformOrder order;
                UT_Vector3 EulerRotations = boneInfo.transform.rotation.computeRotations(order);

                Node->setFloat("r", 0, evalTime, EulerRotations[0]);
                Node->setFloat("r", 1, evalTime, EulerRotations[1]);
                Node->setFloat("r", 2, evalTime, EulerRotations[2]);
            }
        }
    }

    return 1;
}

void RokokoReceiver::start()
{
    if (!bInstalled)
    {
        installGenerator();
        bInstalled = true;
    }
}

void RokokoReceiver::stop()
{
    uninstallGenerator();
    bInstalled = false;
}

void RokokoReceiver::setUpdateRate(int inRate)
{
    rate = inRate;
}

void RokokoReceiver::setIpAndPort(const std::string& inIp, int inPort)
{
    socketReader->setIpAndPort(inIp, inPort);
}

void RokokoReceiver::reset()
{
    socketReader->reset();
}
