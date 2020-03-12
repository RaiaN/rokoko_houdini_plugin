// Copyright Peter Leontev 2020

#include "RokokoDataParser.h"

#include <UT/UT_StringHolder.h>
#include <UT/UT_JSONParser.h>
#include <UT/UT_JSONValue.h>
#include <UT/UT_JSONValueMap.h>
#include <UT/UT_JSONValueArray.h>
#include <UT/UT_Set.h>


RokokoDataParser::RokokoDataParser()
{
    
}

RokokoDataParser::~RokokoDataParser()
{

}

void RokokoDataParser::parse(const std::string& data)
{
    propTrackers.clear();
    
    UT_AutoJSONParser parser(data.c_str(), data.length());

    UT_JSONValue value;
    if (value.parseValue(parser))
    {
        parseData(&value);
    }
}

const UT_Array<PropTrackerInfo>& RokokoDataParser::getPropTrackers()
{
    return propTrackers;
}

const UT_Array<ActorInfo>& RokokoDataParser::getActors()
{
    return actors;
}

void RokokoDataParser::parseData(const UT_JSONValue* jsonValue)
{
    if (!jsonValue)
    {
        return;
    }

    static const UT_StringRef PROPS_KEY("props");
    static const UT_StringRef TRACKERS_KEY("trackers");
    static const UT_StringRef FACES_KEY("faces");
    static const UT_StringRef ACTORS_KEY("actors");


    UT_JSONValueMap* jsonMap = jsonValue->getMap();
    if (!jsonMap)
    {
        // TODO: SET ERROR
        return;
    }

    UT_JSONValue* props = jsonMap->get(PROPS_KEY);
    if (props)
    {
        parsePropsOrTrackers(props);
    }

    UT_JSONValue* trackers = jsonMap->get(TRACKERS_KEY);
    if (trackers)
    {
        parsePropsOrTrackers(trackers);
    }

    UT_JSONValue* actors = jsonMap->get(ACTORS_KEY);
    if (actors)
    {
        parseActors(actors);
    }

    // TODO: faces
}

PropTrackerInfo RokokoDataParser::parsePropTracker(const UT_JSONValueMap* propTrackerAsMap) const
{
    static const UT_StringRef NAME_KEY("name");
    static const UT_StringRef POSITION_KEY("position");
    static const UT_StringRef ROTATION_KEY("rotation");


    PropTrackerInfo propTrackerInfo;

    const UT_JSONValue* nameValue = propTrackerAsMap->get(NAME_KEY);
    if (nameValue)
    {
        propTrackerInfo.name = nameValue->getS();
    }

    propTrackerInfo.transform.position = parsePosition(propTrackerAsMap->get(POSITION_KEY));
    propTrackerInfo.transform.rotation = parseRotation(propTrackerAsMap->get(ROTATION_KEY));

    return propTrackerInfo;
}

void RokokoDataParser::parsePropsOrTrackers(const UT_JSONValue* jsonValue)
{
    if (!jsonValue)
    {
        return;
    }

    UT_JSONValueArray* objects = jsonValue->getArray();
    if (!objects)
    {
        return;
    }

    for (int objInd = 0; objInd < objects->size(); ++objInd)
    {
        UT_JSONValue* obj = objects->get(objInd);
        if (obj)
        {
            UT_JSONValueMap* objAsMap = obj->getMap();
            if (objAsMap)
            {
                propTrackers.append(parsePropTracker(objAsMap));
            }
        }
    }
}


ActorInfo RokokoDataParser::parseActor(const UT_JSONValueMap* actorAsMap) const
{
    static const UT_StringRef NAME_KEY("name");
    static const UT_StringRef TIMESTAMP_KEY("timestamp");
    static const UT_StringRef POSITION_KEY("position");
    static const UT_StringRef ROTATION_KEY("rotation");

    static const UT_Set<UT_StringRef> BONES_KEYS =
    {
        UT_StringRef("hip"), UT_StringRef("spine"), UT_StringRef("neck"), UT_StringRef("head"),

        UT_StringRef("leftShoulder"), UT_StringRef("leftUpperArm"), UT_StringRef("leftLowerArm"), UT_StringRef("leftHand"),
        UT_StringRef("rightShoulder"), UT_StringRef("rightUpperArm"), UT_StringRef("rightLowerArm"), UT_StringRef("rightHand"),
        UT_StringRef("leftUpLeg"), UT_StringRef("leftLeg"), UT_StringRef("leftToe"), UT_StringRef("leftToeEnd"),
        UT_StringRef("rightUpLeg"), UT_StringRef("rightLeg"), UT_StringRef("rightToe"), UT_StringRef("rightToeEnd"),

        UT_StringRef("leftThumbProximal"), UT_StringRef("leftThumbMedial"), UT_StringRef("leftThumbDistal"), UT_StringRef("leftThumbTip"),
        UT_StringRef("leftIndexProximal"), UT_StringRef("leftIndexMedial"), UT_StringRef("leftIndexDistal"), UT_StringRef("leftIndexTip"),
        UT_StringRef("leftMiddleProximal"), UT_StringRef("leftMiddleMedial"), UT_StringRef("leftMiddleDistal"), UT_StringRef("leftMiddleTip"),
        UT_StringRef("leftRingProximal"), UT_StringRef("leftRingMedial"), UT_StringRef("leftRingDistal"), UT_StringRef("leftRingTip"),
        UT_StringRef("leftLittleProximal"), UT_StringRef("leftLittleMedial"), UT_StringRef("leftLittleDistal"), UT_StringRef("leftLittleTip"),

        UT_StringRef("rightThumbProximal"), UT_StringRef("rightThumbMedial"), UT_StringRef("rightThumbDistal"), UT_StringRef("rightThumbTip"),
        UT_StringRef("rightIndexProximal"), UT_StringRef("rightIndexMedial"), UT_StringRef("rightIndexDistal"), UT_StringRef("rightIndexTip"),
        UT_StringRef("rightMiddleProximal"), UT_StringRef("rightMiddleMedial"), UT_StringRef("rightMiddleDistal"), UT_StringRef("rightMiddleTip"),
        UT_StringRef("rightRingProximal"), UT_StringRef("rightRingMedial"), UT_StringRef("rightRingDistal"), UT_StringRef("rightRingTip"),
        UT_StringRef("rightLittleProximal"), UT_StringRef("rightLittleMedial"), UT_StringRef("rightLittleDistal"), UT_StringRef("rightLittleTip"),
    };


    ActorInfo actorInfo;

    const UT_JSONValue* nameValue = actorAsMap->get(NAME_KEY);
    if (nameValue)
    {
        actorInfo.name = nameValue->getS();
    }

    for (UT_StringRef boneName : BONES_KEYS)
    {
        const UT_JSONValue* boneValue = actorAsMap->get(boneName);
        if (boneValue)
        {
            UT_JSONValueMap* boneAsMap = boneValue->getMap();
            if (boneAsMap)
            {
                BoneInfo bone;
                bone.name = boneName.c_str();
                bone.transform.position = parsePosition(boneAsMap->get(POSITION_KEY));
                bone.transform.rotation = parseRotation(boneAsMap->get(ROTATION_KEY));

                actorInfo.bones.append(bone);
            }
        }
    }

    return actorInfo;
}

void RokokoDataParser::parseActors(const UT_JSONValue* jsonValue)
{
    if (!jsonValue)
    {
        return;
    }

    UT_JSONValueArray* actorsArray = jsonValue->getArray();
    for (int actorInd = 0; actorInd < actorsArray->size(); ++actorInd)
    {
        UT_JSONValue* actor = actorsArray->get(actorInd);
        if (actor)
        {
            UT_JSONValueMap* actorAsMap = actor->getMap();
            if (actorAsMap)
            {
                actors.append(parseActor(actorAsMap));
            }
        }
    }
}

UT_Vector3 RokokoDataParser::parsePosition(const UT_JSONValue* jsonValue) const
{
    UT_Vector3 position(0.0, 0.0, 0.0);

    UT_JSONValueMap* positionValueMap = jsonValue->getMap();
    if (positionValueMap)
    {
        const UT_JSONValue* xValue = positionValueMap->get("x");
        const UT_JSONValue* yValue = positionValueMap->get("y");
        const UT_JSONValue* zValue = positionValueMap->get("z");

        if (xValue)
        {
            position.x() = xValue->getF();
        }

        if (yValue)
        {
            position.y() = yValue->getF();
        }

        if (zValue)
        {
            position.z() = zValue->getF();
        }
    }

    return position;
}

UT_Quaternion RokokoDataParser::parseRotation(const UT_JSONValue* jsonValue) const
{
    UT_Quaternion rotation;

    UT_JSONValueMap* rotationValueMap = jsonValue->getMap();
    if (rotationValueMap)
    {
        const UT_JSONValue* xValue = rotationValueMap->get("x");
        const UT_JSONValue* yValue = rotationValueMap->get("y");
        const UT_JSONValue* zValue = rotationValueMap->get("z");
        const UT_JSONValue* wValue = rotationValueMap->get("w");

        if (xValue)
        {
            rotation.x() = xValue->getF();
        }

        if (yValue)
        {
            rotation.y() = yValue->getF();
        }

        if (zValue)
        {
            rotation.z() = zValue->getF();
        }

        if (wValue)
        {
            rotation.w() = wValue->getF();
        }
    }

    return rotation;
}

