#pragma once
#include <easy2d/easy2d.h>
#include <unordered_map>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <thread>
#include <string>
#include <mutex>
#include <ctime>
#include "json/json.h"
#include "Environment.hpp"
#include "SceneControl.hpp"
#include "ObjectOperation.hpp"
// 发射子弹，father 中有需要的内容
BaseObject* NewBullet(BaseObject* father, const std::string& name)
{

}

BaseObject* NewWeapon(const std::string& name, float fx, float fy, float angle)
{
    Json::Value objectData = readJsonFromFile(SHIPS);
    const Json::Value object = objectData["ships"][name];
    if (object == NULL) return nullptr;
    BaseObject* newObject = new BaseObject(10, false, nullptr, spriteMap[name], nullptr, nullptr, false);
    // sprite
    newObject->mass = object["sprite"]["mass"].asFloat();
    newObject->health = object["sprite"]["health"].asFloat();
    newObject->maxSpeedAngle = object["sprite"]["maxSpeedAngle"].asFloat();
    newObject->frictionAngle = object["sprite"]["frictionAngle"].asFloat();
    newObject->angle = angle;
    newObject->fatherX = fx;
    newObject->fatherY = fy;
    // variable
    newObject->variable["damage"] = object["sprite"]["damage"].asFloat();
    newObject->variable["fireSpeed"] = object["sprite"]["fireSpeed"].asFloat();
    newObject->variable["rotateAcceleration"] = object["sprite"]["rotateAcceleration"].asFloat();
    newObject->variable["target"] = object["sprite"]["target"].asBool();

    return newObject;
}

BaseObject* NewShip(const std::string& name, float x, float y, float angle)
{
    Json::Value objectData = readJsonFromFile(SHIPS);
    const Json::Value object = objectData["ships"][name];
    if (object == NULL) return nullptr;
    BaseObject* newObject = new BaseObject(10, false, nullptr, spriteMap[name], nullptr, nullptr, false, x, y);
    // sprite
    newObject->mass = object["sprite"]["mass"].asFloat();
    newObject->health = object["sprite"]["health"].asFloat();
    newObject->maxSpeed = object["sprite"]["maxSpeed"].asFloat();
    newObject->maxSpeedAngle = object["sprite"]["maxSpeedAngle"].asFloat();
    newObject->friction = object["sprite"]["friction"].asFloat();
    newObject->frictionAngle = object["sprite"]["frictionAngle"].asFloat();
    newObject->angle = angle;
    newObject->x = x;
    newObject->y = y;
    // variable
    newObject->variable["rotateAcceleration"] = object["sprite"]["rotateAcceleration"].asFloat();
    newObject->variable["forceAcceleration"] = object["sprite"]["forceAcceleration"].asFloat();
    newObject->variable["maxForce"] = object["sprite"]["maxForce"].asFloat();
    // collisionBox
    for (const Json::Value point : object["collisionBox"])
        easy2d::Point(point[0].asFloat(), point[1].asFloat());
    newObject->Init(); // 初始化
    // weapons
    for (const Json::Value weapon : object["weapons"])
    {
        BaseObject* temp = NewWeapon(weapon["name"].asString(), weapon["position"][0].asFloat(), weapon["position"][0].asFloat(), weapon["angle"].asFloat());
        newObject->childlist.push_back(temp);
        temp->fatherObject = newObject;
        temp->Init();
    }
    // particles
    for (const Json::Value weapon : object["weapons"])
    {
        BaseObject* temp = NewWeapon(weapon["name"].asString(), weapon["position"][0].asFloat(), weapon["position"][0].asFloat(), weapon["angle"].asFloat());
        newObject->childlist.push_back(temp);
        temp->fatherObject = newObject;
        temp->Init();
    }
}